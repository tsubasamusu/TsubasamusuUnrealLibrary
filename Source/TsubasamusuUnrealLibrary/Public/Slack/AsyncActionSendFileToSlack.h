#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncActionSendFileToSlack.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompletedSendFileToSlack, bool, bSucceeded);

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UAsyncActionSendFileToSlack : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnCompletedSendFileToSlack Completed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static UAsyncActionSendFileToSlack* AsyncSendFileToSlack(UObject* WorldContextObject, const FString& Token, const FString& FileName, const FString& ChannelId, const FString& Message, const TArray<uint8>& FileData);

	void Activate() override;

private:
	FString Token;

	FString FileName;

	FString ChannelId;

	FString Message;

	TArray<uint8> FileData;

	void OnSucceeded();

	void OnFailed(const FString& TriedThing);
};