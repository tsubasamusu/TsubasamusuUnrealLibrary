#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "SlackUploadUrlResponse.h"
#include "GetSlackUploadUrlAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncGetUrlForUploadFileToSlackDelegate, const FSlackUploadUrlResponse&, Response);

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UGetSlackUploadUrlAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FAsyncGetUrlForUploadFileToSlackDelegate Completed;

	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|Slack", meta = (BlueprintInternalUseOnly = "true", Keywords = "async get url for upload file to slack"))
	static UGetSlackUploadUrlAsyncAction* AsyncGetUrlForUploadFileToSlack(const FString& Token, const FString& FileName, const int32 FileSize);

	void Activate() override;

private:
	FString Token;

	FString FileName;

	int32 FileSize;

	void OnCompleted(const FSlackUploadUrlResponse& Response);

	void OnFailed(const FString& TriedThing, const FSlackUploadUrlResponse& Response = FSlackUploadUrlResponse());
};