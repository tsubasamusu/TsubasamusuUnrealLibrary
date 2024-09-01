#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncActionGetGoogleCloudJwt.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGotGoogleCloudJwt, const FString&, Message,const int64&, CurrentUnixTime);

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UAsyncActionGetGoogleCloudJwt : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnGotGoogleCloudJwt Completed;

	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|GoogleCloud", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AutoCreateRefTerm = "Scopes"))
	static UAsyncActionGetGoogleCloudJwt* AsyncGetGoogleCloudJwt(UObject* WorldContextObject, const FString& PrivateKey, const FString& ServiceAccountEmailAddress, const TArray<FString>& Scopes);

	void Activate() override;

private:
	FString PrivateKey;

	FString ServiceAccountEmailAddress;

	TArray<FString> Scopes;

	void OnCompleted(const FString& Message);

	void OnFailed(const FString& TriedThing, const FString& Message = FString());
};