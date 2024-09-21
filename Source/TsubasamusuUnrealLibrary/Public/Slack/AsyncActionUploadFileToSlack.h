#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncActionUploadFileToSlack.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUploadedFileToSlack, bool, bSuccess, const FString&, Response);

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UAsyncActionUploadFileToSlack : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnUploadedFileToSlack Completed;

	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|Slack", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static UAsyncActionUploadFileToSlack* AsyncUploadFileToSlack(UObject* WorldContextObject, const FString& Token, const FString& UploadURL, const FString& FileName, const TArray<uint8>& FileData);

	void Activate() override;

private:
	FString Token;

	FString UploadURL;

	FString FileName;

	TArray<uint8> FileData;

	void OnCompleted(bool bSuccess, const FString& Response);

	void OnFailed(const FString& TriedThing, const FString& Response = FString());
};