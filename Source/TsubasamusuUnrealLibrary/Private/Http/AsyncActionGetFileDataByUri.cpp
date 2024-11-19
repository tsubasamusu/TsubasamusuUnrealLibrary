#include "Http/AsyncActionGetFileDataByUri.h"

UAsyncActionGetFileDataByUri* UAsyncActionGetFileDataByUri::AsyncGetFileDataByUri(UObject* WorldContextObject, const FString& Uri)
{
	UAsyncActionGetFileDataByUri* Action = NewObject<UAsyncActionGetFileDataByUri>();

	Action->Uri = Uri;

	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}

void UAsyncActionGetFileDataByUri::Activate()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->SetURL(Uri);

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

			const TArray<uint8>& FileData = HttpResponsePtr->GetContent();

			if (FileData.Num() == 0)
			{
				OnFailed(TEXT("get file data larger than 1 byte"));

				return;
			}

			OnSucceeded(FileData);
		});

	if (!HttpRequest->ProcessRequest()) OnFailed(TEXT("process a HTTP request"));
}

void UAsyncActionGetFileDataByUri::OnSucceeded(const TArray<uint8>& FileData)
{
	Succeeded.Broadcast(FileData);

	SetReadyToDestroy();
}

void UAsyncActionGetFileDataByUri::OnFailed(const FString& TriedThing)
{
	UTsubasamusuLogLibrary::LogError(TEXT("Failed to ") + TriedThing + TEXT(" to get file data from \"") + Uri + TEXT("\"."));

	Failed.Broadcast();

	SetReadyToDestroy();
}