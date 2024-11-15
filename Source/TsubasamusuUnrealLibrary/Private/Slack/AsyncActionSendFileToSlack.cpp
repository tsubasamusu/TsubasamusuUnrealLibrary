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