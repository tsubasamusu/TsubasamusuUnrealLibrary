#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TsubasamusuUrlLibrary.generated.h"

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UTsubasamusuUrlLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "TSUBASAMUSU|HTTP")
	static FString AddQueryParameters(const FString& SourceUrl, const TMap<FString, FString>& QueryParameters);
};