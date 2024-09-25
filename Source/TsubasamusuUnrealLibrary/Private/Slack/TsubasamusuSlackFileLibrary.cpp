#include "Slack/TsubasamusuSlackFileLibrary.h"

UTsubasamusuSlackFileLibrary* UTsubasamusuSlackFileLibrary::AsyncSendFileToSlack(UObject* WorldContextObject, const FString& Token, const FString& FileName, const FString& ChannelID, const FString& Message, const TArray<uint8>& FileData)
{
    UTsubasamusuSlackFileLibrary* Action = NewObject<UTsubasamusuSlackFileLibrary>();

    Action->Token = Token;
    Action->FileName = FileName;
    Action->ChannelID = ChannelID;
    Action->Message = Message;
    Action->FileData = FileData;

    Action->RegisterWithGameInstance(WorldContextObject);

    return Action;
}

void UTsubasamusuSlackFileLibrary::Activate()
{
    FHttpModule* HttpModule = &FHttpModule::Get();

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = HttpModule->CreateRequest();

    const FString URL = TEXT("https://slack.com/api/files.getUploadURLExternal");

    TMap<FString, FString> QueryParameters =
    {
        {TEXT("filename"), FileName},
        {TEXT("length"), FString::FromInt(FileData.Num())}
    };

    HttpRequest->SetURL(UTsubasamusuUrlLibrary::AddQueryParameters(URL, QueryParameters));

    HttpRequest->SetVerb(TEXT("GET"));

    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
    HttpRequest->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + Token);

    HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bSuccess)
        {
            if (!HttpResponsePtr.IsValid())
            {
                OnFailed(TEXT("get a FHttpResponsePtr"));

                return;
            }

            if (!bSuccess)
            {
                OnFailed(TEXT("send a HTTP request"), HttpResponsePtr->GetContentAsString());

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
                OnFailed(TEXT("get a success response from Slack API"), Response.error);

                return;
            }

            AsyncUploadFileToSlack(Response.upload_url, Response.file_id);
        });

    if (!HttpRequest->ProcessRequest()) OnFailed(TEXT("process a HTTP request"));
}

void UTsubasamusuSlackFileLibrary::AsyncUploadFileToSlack(const FString& UploadURL, const FString& FileID)
{
    FHttpModule* HttpModule = &FHttpModule::Get();

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = HttpModule->CreateRequest();

    HttpRequest->SetVerb(TEXT("POST"));

    if (UploadURL.IsEmpty())
    {
        OnFailed(TEXT("get a URL"));

        return;
    }

    HttpRequest->SetURL(UploadURL);

    HttpRequest->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + Token);

    FString Boundary = FString::Printf(TEXT("----WebKitFormBoundary%08X%08X"), FMath::Rand(), FMath::Rand());

    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data; boundary=") + Boundary);

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

    HttpRequest->SetContent(Payload);

    HttpRequest->OnProcessRequestComplete().BindLambda([this, FileID](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bSuccess)
        {
            if (!HttpResponsePtr.IsValid())
            {
                OnFailed(TEXT("get a FHttpResponsePtr"));

                return;
            }

            const FString HttpResponseMessage = HttpResponsePtr->GetContentAsString();

            if (!bSuccess)
            {
                OnFailed(TEXT("send a HTTP request"), HttpResponseMessage);

                return;
            }

            if (!HttpResponseMessage.Contains(TEXT("OK")))
            {
                OnFailed(TEXT("upload a file to Slack"), HttpResponseMessage);

                return;
            }

            AsyncCompleteUploadFileToSlack(FileID);
        });

    if (!HttpRequest->ProcessRequest()) OnFailed(TEXT("process a HTTP request"));
}

void UTsubasamusuSlackFileLibrary::AsyncCompleteUploadFileToSlack(const FString& FileID)
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
            if (!HttpResponsePtr.IsValid())
            {
                OnFailed(TEXT("get a FHttpResponsePtr"));

                return;
            }

            if (!bSuccess)
            {
                OnFailed(TEXT("send a HTTP request"), HttpResponsePtr->GetContentAsString());

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
                OnFailed(TEXT("get a success response from Slack API"), Response.error);

                return;
            }

            OnSucceeded();
        });

    if (!HttpRequest->ProcessRequest()) OnFailed(TEXT("process a HTTP request"));
}

void UTsubasamusuSlackFileLibrary::OnFailed(const FString& TriedThing, const FString& Response)
{
    FString ErrorMessage = TEXT("Failed to ") + TriedThing + TEXT(" to send a file to Slack.");

    if (!Response.IsEmpty()) ErrorMessage = ErrorMessage + TEXT(" The error message is \"") + Response + TEXT("\".");

    UTsubasamusuLogLibrary::LogError(ErrorMessage);

    OnCompleted(false, ErrorMessage);
}

void UTsubasamusuSlackFileLibrary::OnSucceeded(const FString& Response)
{
    OnCompleted(true, Response);
}

void UTsubasamusuSlackFileLibrary::OnCompleted(bool bSuccess, const FString& Response)
{
    Completed.Broadcast(bSuccess, Response);

    SetReadyToDestroy();
}