#include "Slack/CompleteUpload/CompleteUploadToSlackAsyncAction.h"
#include "Debug/TsubasamusuLogLibrary.h"
#include "Http/TsubasamusuUrlLibrary.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonObjectConverter.h"

UCompleteUploadToSlackAsyncAction* UCompleteUploadToSlackAsyncAction::AsyncCompleteUploadFileToSlack(const FString& Token, const FString& FileID, const FString& FileName, const FString& ChannelID, const FString& Message)
{
	UCompleteUploadToSlackAsyncAction* CompleteUploadToSlackAsyncAction = NewObject<UCompleteUploadToSlackAsyncAction>();

	CompleteUploadToSlackAsyncAction->Token = Token;
	CompleteUploadToSlackAsyncAction->FileID = FileID;
	CompleteUploadToSlackAsyncAction->FileName = FileName;
	CompleteUploadToSlackAsyncAction->ChannelID = ChannelID;
	CompleteUploadToSlackAsyncAction->Message = Message;

	return CompleteUploadToSlackAsyncAction;
}

void UCompleteUploadToSlackAsyncAction::Activate()
{
    FHttpModule* HttpModule = &FHttpModule::Get();

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = HttpModule->CreateRequest();

    const FString URL = TEXT("https://slack.com/api/files.completeUploadExternal");

    TMap<FString, FString> QueryParameters =
    {
        {
            TEXT("files"),
            TEXT("[{\"id\":\"") + FileID + TEXT("\", \"title\":\"") + FileName + TEXT("\"}]")
        },
        {
            TEXT("channel_id"),
            ChannelID
        },
        {
            TEXT("initial_comment"),
            Message
        }
    };

    HttpRequest->SetURL(UTsubasamusuUrlLibrary::AddQueryParameters(URL, QueryParameters));

    HttpRequest->SetVerb(TEXT("POST"));

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

            FSlackCompleteUploadResponse Response;

            if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &Response, 0, 0))
            {
                OnFailed(TEXT("convert a FJsonObject to a struct"));

                return;
            }

            if (!Response.ok)
            {
                OnFailed(TEXT("get a success response from Slack API"), Response);

                return;
            }

            OnCompleted(Response);
        });

    if (!HttpRequest->ProcessRequest()) OnFailed(TEXT("process a HTTP request"));
}

void UCompleteUploadToSlackAsyncAction::OnCompleted(const FSlackCompleteUploadResponse& Response)
{
	Completed.Broadcast(Response);
}

void UCompleteUploadToSlackAsyncAction::OnFailed(const FString& TriedThing, const FSlackCompleteUploadResponse& Response)
{
	UTsubasamusuLogLibrary::LogError(TEXT("Failed to ") + TriedThing + TEXT(" to complete uploading a file to Slack."));

	OnCompleted(Response);
}