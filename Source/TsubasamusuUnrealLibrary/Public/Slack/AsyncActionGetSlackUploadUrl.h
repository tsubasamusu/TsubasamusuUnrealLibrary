#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Slack/Struct/SlackUploadUrlResponse.h"
#include "AsyncActionGetSlackUploadUrl.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGotUrlForUploadFileToSlack, const FSlackUploadUrlResponse&, Response);

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UAsyncActionGetSlackUploadUrl : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnGotUrlForUploadFileToSlack Completed;

	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|Slack", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static UAsyncActionGetSlackUploadUrl* AsyncGetUrlForUploadFileToSlack(UObject* WorldContextObject, const FString& Token, const FString& FileName, const int32 FileSize);

	void Activate() override;

private:
	FString Token;

	FString FileName;

	int32 FileSize;

	void OnCompleted(const FSlackUploadUrlResponse& Response);

	void OnFailed(const FString& TriedThing, const FSlackUploadUrlResponse& Response = FSlackUploadUrlResponse());
};