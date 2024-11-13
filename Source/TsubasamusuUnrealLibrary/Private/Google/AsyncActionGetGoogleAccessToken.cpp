#include "Google/AsyncActionGetGoogleAccessToken.h"
#include "Interfaces/IHttpRequest.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

#define UI UI_ST
THIRD_PARTY_INCLUDES_START
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
THIRD_PARTY_INCLUDES_END
#undef UI

UAsyncActionGetGoogleAccessToken* UAsyncActionGetGoogleAccessToken::AsyncGetGoogleAccessToken(UObject* WorldContextObject, const FString& PrivateKey, const FString& ServiceAccountMailAddress, const TArray<FString>& Scopes)
{
	UAsyncActionGetGoogleAccessToken* Action = NewObject<UAsyncActionGetGoogleAccessToken>();

	Action->PrivateKey = PrivateKey;
	Action->ServiceAccountMailAddress = ServiceAccountMailAddress;
	Action->Scopes = Scopes;

	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}

void UAsyncActionGetGoogleAccessToken::Activate()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

	FString Url = TEXT("https://oauth2.googleapis.com/token");

	HttpRequest->SetURL(Url);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(GetGoogleCloudJsonContent());

	HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bSucceeded)
		{
			if (!bSucceeded)
			{
				OnFailed(TEXT("send a HTTP request"));

				return;
			}

			if (!HttpResponsePtr.IsValid())
			{
				OnFailed(TEXT("get a FHttpResponsePtr"));

				return;
			}

			FString JsonResponse = HttpResponsePtr->GetContentAsString();

			TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonResponse);

			TSharedPtr<FJsonObject> JsonObject;

			if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
			{
				OnFailed(TEXT("deserialize the JSON response"));

				return;
			}

			FString AccessToken;

			if (JsonObject->TryGetStringField(TEXT("access_token"), AccessToken))
			{
				OnSucceeded(AccessToken);

				return;
			}

			FString ErrorMessage, ErrorDescription;

			if (JsonObject->TryGetStringField(TEXT("error"), ErrorMessage) && JsonObject->TryGetStringField(TEXT("error_description"), ErrorDescription))
			{
				UTsubasamusuLogLibrary::LogError(TEXT("The error is \"") + ErrorMessage + TEXT("\"."));
				UTsubasamusuLogLibrary::LogError(TEXT("The error description is \"") + ErrorDescription + TEXT("\"."));

				OnFailed(TEXT("get string value of \"access_token\" field"));

				return;
			}

			OnFailed(TEXT("get any field of the JSON response"));
		});

	if (!HttpRequest->ProcessRequest()) OnFailed(TEXT("process a HTTP request"));
}

FString UAsyncActionGetGoogleAccessToken::GetGoogleAccessTokenHeader()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

	JsonObject->SetStringField(TEXT("alg"), TEXT("RS256"));
	JsonObject->SetStringField(TEXT("typ"), TEXT("JWT"));

	FString JsonString;

	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);

	if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter)) return JsonString;

	OnFailed(TEXT("create a JSON header"));

	return TEXT("");
}

FString UAsyncActionGetGoogleAccessToken::GetGoogleAccessTokenPayload()
{
	int64 IssuedUnixTime = FDateTime::UtcNow().ToUnixTimestamp();
	ExpirationIssuedUnixTime = IssuedUnixTime + 3600;

	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

	JsonObject->SetStringField(TEXT("iss"), ServiceAccountMailAddress);
	JsonObject->SetStringField(TEXT("scope"), FString::Join(Scopes, TEXT(" ")));
	JsonObject->SetStringField(TEXT("aud"), TEXT("https://oauth2.googleapis.com/token"));
	JsonObject->SetNumberField(TEXT("exp"), ExpirationIssuedUnixTime);
	JsonObject->SetNumberField(TEXT("iat"), IssuedUnixTime);

	FString JsonString;

	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);

	if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter)) return JsonString;

	OnFailed(TEXT("create a JSON payload"));

	return TEXT("");
}

FString UAsyncActionGetGoogleAccessToken::GetGoogleCloudJwt()
{
	TArray<char> PrivateKeyCharArray = UTsubasamusuStringConvertLibrary::ConvertToCharArray(PrivateKey.Replace(TEXT("\\n"), TEXT("\n")));

	BIO* Bio = BIO_new_mem_buf((void*)PrivateKeyCharArray.GetData(), -1);

	if (!Bio)
	{
		OnFailed(TEXT("create a BIO"));

		return TEXT("");
	}

	RSA* Rsa = PEM_read_bio_RSAPrivateKey(Bio, nullptr, nullptr, nullptr);

	BIO_free(Bio);

	if (!Rsa)
	{
		OnFailed(TEXT("create a RSA"));

		return TEXT("");
	}

	FString HeaderBase64 = UTsubasamusuStringConvertLibrary::ConvertToBase64(GetGoogleAccessTokenHeader());
	FString PayloadBase64 = UTsubasamusuStringConvertLibrary::ConvertToBase64(GetGoogleAccessTokenPayload());

	FString Message = HeaderBase64 + TEXT(".") + PayloadBase64;

	TArray<uint8> MessageDigest = UTsubasamusuStringConvertLibrary::ConvertToUtf8(Message);

	uint8 Digest[SHA256_DIGEST_LENGTH];

	SHA256(MessageDigest.GetData(), MessageDigest.Num(), Digest);

	uint8 Signature[256];

	unsigned int SignatureLen;

	if (RSA_sign(NID_sha256, Digest, SHA256_DIGEST_LENGTH, Signature, &SignatureLen, Rsa) != 1)
	{
		RSA_free(Rsa);

		ERR_print_errors_fp(stderr);

		return TEXT("");
	}

	RSA_free(Rsa);

	return Message + TEXT(".") + FBase64::Encode(Signature, SignatureLen);
}

FString UAsyncActionGetGoogleAccessToken::GetGoogleCloudJsonContent()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

	JsonObject->SetStringField(TEXT("grant_type"), TEXT("urn:ietf:params:oauth:grant-type:jwt-bearer"));
	JsonObject->SetStringField(TEXT("assertion"), GetGoogleCloudJwt());

	FString JsonString;

	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);

	if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter)) return JsonString;

	OnFailed(TEXT("create a JSON content"));

	return TEXT("");
}

void UAsyncActionGetGoogleAccessToken::OnSucceeded(const FString& Message)
{
	Completed.Broadcast(Message, ExpirationIssuedUnixTime, true);

	SetReadyToDestroy();
}

void UAsyncActionGetGoogleAccessToken::OnFailed(const FString& TriedThing, const FString& Message)
{
	UTsubasamusuLogLibrary::LogError(TEXT("Failed to ") + TriedThing + TEXT(" to get an access token for Google Cloud."));

	Completed.Broadcast(Message, -1, false);

	SetReadyToDestroy();
}