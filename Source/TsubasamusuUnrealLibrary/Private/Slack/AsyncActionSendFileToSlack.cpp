#include "Slack/AsyncActionSendFileToSlack.h"

UAsyncActionSendFileToSlack* UAsyncActionSendFileToSlack::AsyncSendFileToSlack(UObject* WorldContextObject, const FString& Token, const FString& FileName, const FString& ChannelId, const FString& Message, const TArray<uint8>& FileData)
{
	UAsyncActionSendFileToSlack* Action = NewObject<UAsyncActionSendFileToSlack>();

	Action->Token = Token;
	Action->FileName = FileName;
	Action->ChannelId = ChannelId;
	Action->Message = Message;
	Action->FileData = FileData;

	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}

void UAsyncActionSendFileToSlack::Activate()
{
    FHttpModule* HttpModule = &FHttpModule::Get();

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = HttpModule->CreateRequest();

    FString Url = TEXT("https://slack.com/api/files.getUploadURLExternal");

    TMap<FString, FString> QueryParameters =
    {
        {
            TEXT("filename"),
            FileName
        },
        {
            TEXT("length"),
            FString::FromInt(FileData.Num())
        }
    };

    HttpRequest->SetURL(UTsubasamusuUrlLibrary::AddQueryParameters(Url, QueryParameters));
    HttpRequest->SetVerb(TEXT("GET"));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
    HttpRequest->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + Token);

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

            bool bOk;

            if (!JsonObject->TryGetBoolField(TEXT("ok"), bOk))
            {
                OnFailed(TEXT("get bool result field from the JSON response"));

                return;
            }

            if (bOk)
            {
                FString UploadUrl, FileId;

                if (!JsonObject->TryGetStringField(TEXT("upload_url"), UploadUrl))
                {
                    UTsubasamusuLogLibrary::LogError(TEXT("The JSON response is \"") + JsonResponse + TEXT("\"."));

                    OnFailed(TEXT("get an upload URL from the JSON response"));

                    return;
                }

                if (!JsonObject->TryGetStringField(TEXT("file_id"), FileId))
                {
                    UTsubasamusuLogLibrary::LogError(TEXT("The JSON response is \"") + JsonResponse + TEXT("\"."));

                    OnFailed(TEXT("get a file ID from the JSON response"));

                    return;
                }

                CompleteUploadFileToSlack(UploadUrl, FileId);

                return;
            }

            FString ErrorMessage;

            if (JsonObject->TryGetStringField(TEXT("error"), ErrorMessage))
            {
                UTsubasamusuLogLibrary::LogError(TEXT("The error is \"") + ErrorMessage + TEXT("\"."));

                OnFailed(TEXT("get a successful result"));

                return;
            }

            UTsubasamusuLogLibrary::LogError(TEXT("The JSON response is \"") + JsonResponse + TEXT("\"."));

            OnFailed(TEXT("get any field from the JSON response"));
        });

    if (!HttpRequest->ProcessRequest()) OnFailed(TEXT("process a HTTP request"));
}

void UAsyncActionSendFileToSlack::CompleteUploadFileToSlack(const FString& UploadUrl, const FString& FileId)
{

}

void UAsyncActionSendFileToSlack::OnSucceeded()
{
    Completed.Broadcast(true);

    SetReadyToDestroy();
}

void UAsyncActionSendFileToSlack::OnFailed(const FString& TriedThing)
{
    UTsubasamusuLogLibrary::LogError(TEXT("Failed to ") + TriedThing + TEXT(" to send a file to Slack."));

    Completed.Broadcast(false);

    SetReadyToDestroy();
}