#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncActionGetFileDataByUri.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSucceededToGetFileDataByUri, const TArray<uint8>&, FileData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFailedToGetFileDataByUri);

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UAsyncActionGetFileDataByUri : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnSucceededToGetFileDataByUri Succeeded;

	UPROPERTY(BlueprintAssignable)
	FOnFailedToGetFileDataByUri Failed;

	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|HTTP", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static UAsyncActionGetFileDataByUri* AsyncGetFileDataByUri(UObject* WorldContextObject, const FString& Uri);

	void Activate() override;

private:
	FString Uri;

	void OnSucceeded(const TArray<uint8>& FileData);

	void OnFailed(const FString& TriedThing);
};