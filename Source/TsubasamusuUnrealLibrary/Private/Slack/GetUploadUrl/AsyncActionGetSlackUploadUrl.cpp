#include "Slack/GetUploadUrl/AsyncActionGetSlackUploadUrl.h"
#include "Http/TsubasamusuUrlLibrary.h"
#include "Debug/TsubasamusuLogLibrary.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonObjectConverter.h"
#include "Slack/GetUploadUrl/SlackUploadUrlResponse.h"

UAsyncActionGetSlackUploadUrl* UAsyncActionGetSlackUploadUrl::AsyncGetUrlForUploadFileToSlack(const FString& Token, const FString& FileName, const int32 FileSize)
{
	UAsyncActionGetSlackUploadUrl* GetSlackUploadUrlAsyncAction = NewObject<UAsyncActionGetSlackUploadUrl>();

	GetSlackUploadUrlAsyncAction->Token = Token;
	GetSlackUploadUrlAsyncAction->FileName = FileName;
	GetSlackUploadUrlAsyncAction->FileSize = FileSize;

	return GetSlackUploadUrlAsyncAction;
}

void UAsyncActionGetSlackUploadUrl::Activate()
{
    FHttpModule* HttpModule = &FHttpModule::Get();

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = HttpModule->CreateRequest();

    const FString URL = TEXT("https://slack.com/api/files.getUploadURLExternal");

    TMap<FString, FString> QueryParameters =
    {
        {TEXT("filename"), FileName},
        {TEXT("length"), FString::FromInt(FileSize)}
    };

    HttpRequest->SetURL(UTsubasamusuUrlLibrary::AddQueryParameters(URL, QueryParameters));

    HttpRequest->SetVerb(TEXT("GET"));

    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
    HttpRequest->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + Token);

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

            FString JsonResponse = HttpResponsePtr->GetContentAsString();

            TSharedPtr<FJsonObject> JsonObject;

            TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonResponse);

            if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
            {
                OnFailed(TEXT("deserialize a JSON response"));

                return;
            }

            FSlackUploadUrlResponse Response;

            if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &Response, 0, 0))
            {
                OnFailed(TEXT("convert a FJsonObject to a struct"));

                return;
            }

            if (!Response.ok || Response.upload_url.IsEmpty())
            {
                OnFailed(TEXT("get a success response from Slack API"), Response);

                return;
            }

            OnCompleted(Response);
        });

    if (!HttpRequest->ProcessRequest()) OnFailed(TEXT("process a HTTP request"));
}

void UAsyncActionGetSlackUploadUrl::OnCompleted(const FSlackUploadUrlResponse& Response)
{
    Completed.Broadcast(Response);
}

void UAsyncActionGetSlackUploadUrl::OnFailed(const FString& TriedThing, const FSlackUploadUrlResponse& Response)
{
    UTsubasamusuLogLibrary::LogError(TEXT("Failed to ") + TriedThing + TEXT(" to get a URL for upload a file to Slack."));

    OnCompleted(Response);
}