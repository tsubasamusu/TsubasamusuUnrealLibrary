#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "SlackCompleteUploadResponse.h"
#include "AsyncActionCompleteUploadToSlack.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompletedUploadFileToSlack, const FSlackCompleteUploadResponse&, Response);

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UAsyncActionCompleteUploadToSlack : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnCompletedUploadFileToSlack Completed;

	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|Slack", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static UAsyncActionCompleteUploadToSlack* AsyncCompleteUploadFileToSlack(UObject* WorldContextObject, const FString& Token, const FString& FileID, const FString& FileName, const FString& ChannelID, const FString& Message);

	void Activate() override;

private:
	FString Token;

	FString FileID;

	FString FileName;

	FString ChannelID;

	FString Message;

	void OnCompleted(const FSlackCompleteUploadResponse& Response);

	void OnFailed(const FString& TriedThing, const FSlackCompleteUploadResponse& Response = FSlackCompleteUploadResponse());
};