#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncActionGetGoogleCloudJwt.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGotGoogleCloudJwt, const FString&, Message, int64, IssuedUnixTime, bool, bSuccess);

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UAsyncActionGetGoogleCloudJwt : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnGotGoogleCloudJwt Completed;

	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|GoogleCloud", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AutoCreateRefTerm = "Scopes"))
	static UAsyncActionGetGoogleCloudJwt* AsyncGetGoogleCloudJwt(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "TSUBASAMUSU|GoogleCloud")
	FString GetGoogleCloudJwtHeader();

	UFUNCTION(BlueprintPure, Category = "TSUBASAMUSU|GoogleCloud")
	FString GetGoogleCloudJwtPayload(const FString& ServiceAccountMailAddress, const TArray<FString>& Scopes);

	void Activate() override;

private:
	int64 IssuedUnixTime;

	void OnCompleted(const FString& Message);

	void OnFailed(const FString& TriedThing, const FString& Message = FString());
};