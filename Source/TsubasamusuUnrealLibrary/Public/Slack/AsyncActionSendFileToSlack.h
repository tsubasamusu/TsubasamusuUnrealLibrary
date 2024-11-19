#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncActionSendFileToSlack.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSucceededToSendFileToSlack);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFailedToSendFileToSlack);

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UAsyncActionSendFileToSlack : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnSucceededToSendFileToSlack Succeeded;

	UPROPERTY(BlueprintAssignable)
	FOnFailedToSendFileToSlack Failed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static UAsyncActionSendFileToSlack* AsyncSendFileToSlack(UObject* WorldContextObject, const FString& Token, const FString& FileName, const FString& ChannelId, const FString& Message, const TArray<uint8>& FileData);

	void Activate() override;

private:
	FString Token;

	FString FileName;

	FString ChannelId;

	FString Message;

	TArray<uint8> FileData;

	void UploadFileToSlack(const FString& UploadUrl, const FString& FileId);

	void CompleteUploadFileToSlack(const FString& FileId);

	void OnSucceeded();

	void OnFailed(const FString& TriedThing);
};