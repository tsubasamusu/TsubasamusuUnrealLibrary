#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncActionGetGoogleAccessToken.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGotGoogleAccessToken, const FString&, Message, int64, IssuedUnixTime, bool, bSuccess);

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UAsyncActionGetGoogleAccessToken : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnGotGoogleAccessToken Completed;

	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|Google", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AutoCreateRefTerm = "Scopes"))
	static UAsyncActionGetGoogleAccessToken* AsyncGetGoogleAccessToken(UObject* WorldContextObject);

	void Activate() override;

private:
	int64 IssuedUnixTime;

	FString GetGoogleAccessTokenHeader();

	FString GetGoogleAccessTokenPayload(const FString& ServiceAccountMailAddress, const TArray<FString>& Scopes);

	void OnCompleted(const FString& Message);

	void OnFailed(const FString& TriedThing, const FString& Message = FString());
};