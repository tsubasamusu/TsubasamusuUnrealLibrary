#include "Slack/UploadFile/AsyncActionUploadFileToSlack.h"
#include "Debug/TsubasamusuLogLibrary.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

UAsyncActionUploadFileToSlack* UAsyncActionUploadFileToSlack::AsyncUploadFileToSlack(UObject* WorldContextObject, const FString& Token, const FString& UploadURL, const FString& FileName, const TArray<uint8>& FileData)
{
	UAsyncActionUploadFileToSlack* Action = NewObject<UAsyncActionUploadFileToSlack>();

	Action->Token = Token;
	Action->UploadURL = UploadURL;
	Action->FileName = FileName;
	Action->FileData = FileData;

    Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}

void UAsyncActionUploadFileToSlack::Activate()
{
    FHttpModule* HttpModule = &FHttpModule::Get();

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = HttpModule->CreateRequest();

    HttpRequest->SetVerb(TEXT("POST"));

    if (UploadURL.IsEmpty())
    {
        OnFailed(TEXT("The \"UploadURL\" is empty."));

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

    HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bSuccess)
        {
            const FString Response = HttpResponsePtr->GetContentAsString();

            if (!bSuccess)
            {
                OnFailed(TEXT("send a HTTP request"), Response);

                return;
            }

            if (!HttpResponsePtr.IsValid())
            {
                OnFailed(TEXT("get a FHttpResponsePtr"), Response);

                return;
            }

            if (!Response.Contains(TEXT("OK")))
            {
                OnFailed(TEXT("upload a file to Slack"), Response);

                return;
            }

            OnCompleted(true, Response);
        });

    if (!HttpRequest->ProcessRequest()) OnFailed(TEXT("process a HTTP request"));
}

void UAsyncActionUploadFileToSlack::OnCompleted(const bool& bSuccess, const FString& Response)
{
    Completed.Broadcast(bSuccess, Response);

    SetReadyToDestroy();
}

void UAsyncActionUploadFileToSlack::OnFailed(const FString& TriedThing, const FString& Response)
{
    UTsubasamusuLogLibrary::LogError(TEXT("Failed to ") + TriedThing + TEXT(" to upload a file to Slack."));

    OnCompleted(false, Response);
}