#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncActionGetGoogleAccessToken.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGotGoogleAccessToken, const FString&, AccessToken, int64, ExpirationUnixTime, bool, bSucceeded);

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UAsyncActionGetGoogleAccessToken : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnGotGoogleAccessToken Completed;

	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|Google", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AutoCreateRefTerm = "Scopes"))
	static UAsyncActionGetGoogleAccessToken* AsyncGetGoogleAccessToken(UObject* WorldContextObject, const FString& PrivateKey, const FString& ServiceAccountMailAddress, const TArray<FString>& Scopes);

	void Activate() override;

private:
	FString PrivateKey;

	FString ServiceAccountMailAddress;

	TArray<FString> Scopes;

	int64 ExpirationUnixTime;

	FString GetGoogleAccessTokenHeader();

	FString GetGoogleAccessTokenPayload();

	FString GetGoogleCloudJwt();

	FString GetGoogleCloudJsonContent();

	void OnSucceeded(const FString& AccessToken);

	void OnFailed(const FString& TriedThing);
};