#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TsubasamusuSlackFileLibrary.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCompletedSendFileToSlack, bool, bSuccess, const FString&, Response);

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UTsubasamusuSlackFileLibrary : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnCompletedSendFileToSlack Completed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AutoCreateRefTerm = "FileData"))
	static UTsubasamusuSlackFileLibrary* AsyncSendFileToSlack(UObject* WorldContextObject, const FString& Token, const FString& FileName, const FString& ChannelID, const FString& Message, const TArray<uint8>& FileData);

	void Activate() override;

private:
	FString Token;

	FString FileName;

	FString ChannelID;

	FString Message;

	TArray<uint8> FileData;

	void AsyncUploadFileToSlack(const FString& UploadURL, const FString& FileID);

	void AsyncCompleteUploadFileToSlack(const FString& FileID);

	void OnFailed(const FString& TriedThing, const FString& Response = FString());

	void OnSucceeded(const FString& Response = FString());

	void OnCompleted(bool bSuccess, const FString& Response);
};