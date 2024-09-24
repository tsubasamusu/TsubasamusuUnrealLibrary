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
                OnFailed(TEXT("get a success response from Slack API"), Response.error);

                return;
            }

            OnSucceeded();
        });

    if (!HttpRequest->ProcessRequest()) OnFailed(TEXT("process a HTTP request"));
}

void UTsubasamusuSlackFileLibrary::OnCompleted(bool bSuccess, const FString& Response)
{
	Completed.Broadcast(bSuccess, Response);

	SetReadyToDestroy();
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