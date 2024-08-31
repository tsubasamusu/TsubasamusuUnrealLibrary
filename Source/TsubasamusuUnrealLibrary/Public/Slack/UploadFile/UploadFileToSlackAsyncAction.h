#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "UploadFileToSlackAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncUploadFileToSlackDelegate, const bool, bSuccess, const FString&, Response);

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UUploadFileToSlackAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FAsyncUploadFileToSlackDelegate Completed;

	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|Slack", meta = (BlueprintInternalUseOnly = "true"))
	static UUploadFileToSlackAsyncAction* AsyncUploadFileToSlack(const FString& Token, const FString& UploadURL, const FString& FileName, const TArray<uint8>& FileData);

	void Activate() override;

private:
	FString Token;

	FString UploadURL;

	FString FileName;

	TArray<uint8> FileData;

	void OnCompleted(const bool& bSuccess, const FString& Response);

	void OnFailed(const FString& TriedThing, const FString& Response = FString());
};