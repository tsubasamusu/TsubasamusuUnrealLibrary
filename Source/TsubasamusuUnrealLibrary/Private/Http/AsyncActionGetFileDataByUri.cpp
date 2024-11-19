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