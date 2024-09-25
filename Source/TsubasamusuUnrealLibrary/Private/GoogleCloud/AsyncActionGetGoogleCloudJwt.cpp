#include "GoogleCloud/AsyncActionGetGoogleCloudJwt.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Debug/TsubasamusuLogLibrary.h"

UAsyncActionGetGoogleCloudJwt* UAsyncActionGetGoogleCloudJwt::AsyncGetGoogleCloudJwt(UObject* WorldContextObject, const FString& GoogleCloudRunUrl, const FString& PrivateKey, const FString& ServiceAccountEmailAddress, const TArray<FString>& Scopes)
{
    UAsyncActionGetGoogleCloudJwt* Action = NewObject<UAsyncActionGetGoogleCloudJwt>();

    Action->GoogleCloudRunUrl = GoogleCloudRunUrl;
    Action->PrivateKey = PrivateKey;
    Action->ServiceAccountEmailAddress = ServiceAccountEmailAddress;
    Action->Scopes = Scopes;

    Action->RegisterWithGameInstance(WorldContextObject);

    return Action;
}

void UAsyncActionGetGoogleCloudJwt::Activate()
{
    FHttpModule* HttpModule = &FHttpModule::Get();

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = HttpModule->CreateRequest();

    HttpRequest->SetURL(GoogleCloudRunUrl);

    HttpRequest->SetVerb(TEXT("POST"));

    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

    JsonObject->SetStringField(TEXT("privateKey"), PrivateKey);
    JsonObject->SetStringField(TEXT("serviceAccountEmailAddress"), ServiceAccountEmailAddress);
    JsonObject->SetStringField(TEXT("scopes"), FString::Join(Scopes, TEXT(" ")));

    FString JsonString;

    TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);

    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

    HttpRequest->SetContentAsString(JsonString);

    HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bSuccess)
        {
            if (!bSuccess)
            {
                OnFailed(TEXT("send a HTTP request"));

                return;
            }

            if (!HttpResponsePtr.IsValid())
            {
                OnFailed(TEXT("get a FHttpResponsePtr"));

                return;
            }

            OnCompleted(HttpResponsePtr->GetContentAsString());
        });

    if (!HttpRequest->ProcessRequest()) OnFailed(TEXT("process a HTTP request"));
}

void UAsyncActionGetGoogleCloudJwt::OnCompleted(const FString& Message)
{
	Completed.Broadcast(Message, FDateTime::UtcNow().ToUnixTimestamp());

	SetReadyToDestroy();
}

void UAsyncActionGetGoogleCloudJwt::OnFailed(const FString& TriedThing, const FString& Message)
{
	UTsubasamusuLogLibrary::LogError(TEXT("Failed to ") + TriedThing + TEXT(" to get a JSON Web Token for Google Cloud."));

	OnCompleted(Message);
}