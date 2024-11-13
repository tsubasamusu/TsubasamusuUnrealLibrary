#include "Google/AsyncActionGetGoogleAccessToken.h"

#define UI UI_ST
THIRD_PARTY_INCLUDES_START
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
THIRD_PARTY_INCLUDES_END
#undef UI

UAsyncActionGetGoogleAccessToken* UAsyncActionGetGoogleAccessToken::AsyncGetGoogleAccessToken(UObject* WorldContextObject, const FString& PrivateKey, const FString& KeyId, const FString& ServiceAccountMailAddress, const TArray<FString>& Scopes)
{
	UAsyncActionGetGoogleAccessToken* Action = NewObject<UAsyncActionGetGoogleAccessToken>();

	Action->PrivateKey = PrivateKey;
	Action->KeyId = KeyId;
	Action->ServiceAccountMailAddress = ServiceAccountMailAddress;
	Action->Scopes = Scopes;

	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}

void UAsyncActionGetGoogleAccessToken::Activate()
{
	GetGoogleCloudJwt();
}

FString UAsyncActionGetGoogleAccessToken::GetGoogleAccessTokenHeader()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

	JsonObject->SetStringField(TEXT("alg"), TEXT("RS256"));
	JsonObject->SetStringField(TEXT("typ"), TEXT("JWT"));
	JsonObject->SetStringField(TEXT("kid"), KeyId);

	FString JsonString;

	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);

	if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter)) return JsonString;

	OnFailed(TEXT("create a JSON header"));

	return TEXT("");
}

FString UAsyncActionGetGoogleAccessToken::GetGoogleAccessTokenPayload()
{
	IssuedUnixTime = FDateTime::UtcNow().ToUnixTimestamp();

	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

	JsonObject->SetStringField(TEXT("iss"), ServiceAccountMailAddress);
	JsonObject->SetStringField(TEXT("scope"), FString::Join(Scopes, TEXT(" ")));
	JsonObject->SetStringField(TEXT("aud"), TEXT("https://oauth2.googleapis.com/token"));
	JsonObject->SetNumberField(TEXT("exp"), IssuedUnixTime + 3600);
	JsonObject->SetNumberField(TEXT("iat"), IssuedUnixTime);

	FString JsonString;

	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);

	if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter)) return JsonString;

	OnFailed(TEXT("create a JSON payload"));

	return TEXT("");
}

FString UAsyncActionGetGoogleAccessToken::GetGoogleCloudJwt()
{
	TArray<char> PrivateKeyCharArray = UTsubasamusuStringConvertLibrary::ConvertToCharArray(PrivateKey.Replace(TEXT("\\n"),TEXT("\n")));

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

void UAsyncActionGetGoogleAccessToken::OnCompleted(const FString& Message)
{
	Completed.Broadcast(Message, IssuedUnixTime, true);

	SetReadyToDestroy();
}

void UAsyncActionGetGoogleAccessToken::OnFailed(const FString& TriedThing, const FString& Message)
{
	UTsubasamusuLogLibrary::LogError(TEXT("Failed to ") + TriedThing + TEXT(" to get an access token for Google Cloud."));

	Completed.Broadcast(Message, -1, false);

	SetReadyToDestroy();
}