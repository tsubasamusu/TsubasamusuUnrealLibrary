#include "Slack/AsyncActionSendFileToSlack.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "Http/TsubasamusuUrlLibrary.h"
#include "Debug/TsubasamusuLogLibrary.h"

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
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

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
                UTsubasamusuLogLibrary::LogError(TEXT("The JSON response is \"") + JsonResponse + TEXT("\"."));

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

                UploadFileToSlack(UploadUrl, FileId);

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

void UAsyncActionSendFileToSlack::UploadFileToSlack(const FString& UploadUrl, const FString& FileId)
{
    FString Boundary = FString::Printf(TEXT("----WebKitFormBoundary%08X%08X"), FMath::Rand(), FMath::Rand());
    FString BeginBoundary = TEXT("--") + Boundary + TEXT("\r\n");
    FString EndBoundary = TEXT("\r\n--") + Boundary + TEXT("--\r\n");
    FString ContentDisposition = TEXT("Content-Disposition: form-data; name=\"file\"; filename=\"") + FileName + TEXT("\"\r\n");
    FString ContentType = TEXT("Content-Type: application/octet-stream\r\n\r\n");

    TArray<uint8> Payload;

    Payload.Append((uint8*)TCHAR_TO_UTF8(*BeginBoundary), BeginBoundary.Len());
    Payload.Append((uint8*)TCHAR_TO_UTF8(*ContentDisposition), ContentDisposition.Len());
    Payload.Append((uint8*)TCHAR_TO_UTF8(*ContentType), ContentType.Len());
    Payload.Append(FileData);
    Payload.Append((uint8*)TCHAR_TO_UTF8(*EndBoundary), EndBoundary.Len());

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetURL(UploadUrl);
    HttpRequest->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + Token);
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data; boundary=") + Boundary);
    HttpRequest->SetContent(Payload);

    HttpRequest->OnProcessRequestComplete().BindLambda([this, FileId](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bSucceeded)
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

            FString ResponseString = HttpResponsePtr->GetContentAsString();

            if (ResponseString.Contains(TEXT("OK")))
            {
                CompleteUploadFileToSlack(FileId);

                return;
            }

            UTsubasamusuLogLibrary::LogError(TEXT("The HTTP response is \"") + ResponseString + TEXT("\"."));

            OnFailed(TEXT("upload the file"));
        });

    if (!HttpRequest->ProcessRequest()) OnFailed(TEXT("process a HTTP request"));
}

void UAsyncActionSendFileToSlack::CompleteUploadFileToSlack(const FString& FileId)
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

    FString Url = TEXT("https://slack.com/api/files.completeUploadExternal");

    TMap<FString, FString> QueryParameters =
    {
        {
            TEXT("files"),
            TEXT("[{\"id\":\"") + FileId + TEXT("\", \"title\":\"") + FileName + TEXT("\"}]")
        },
        {
            TEXT("channel_id"),
            ChannelId
        },
        {
            TEXT("initial_comment"),
            Message
        }
    };

    HttpRequest->SetURL(UTsubasamusuUrlLibrary::AddQueryParameters(Url, QueryParameters));
    HttpRequest->SetVerb(TEXT("POST"));
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
                UTsubasamusuLogLibrary::LogError(TEXT("The JSON response is \"") + JsonResponse + TEXT("\"."));

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
                OnSucceeded();

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

void UAsyncActionSendFileToSlack::OnSucceeded()
{
    Succeeded.Broadcast();

    SetReadyToDestroy();
}

void UAsyncActionSendFileToSlack::OnFailed(const FString& TriedThing)
{
    UTsubasamusuLogLibrary::LogError(TEXT("Failed to ") + TriedThing + TEXT(" to send a file to Slack."));

    Failed.Broadcast();

    SetReadyToDestroy();
}