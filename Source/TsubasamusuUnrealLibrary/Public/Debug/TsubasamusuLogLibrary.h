#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TsubasamusuLogLibrary.generated.h"

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UTsubasamusuLogLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|Debug")
	static void Log(const FString& Message, bool bShowInViewport = true, float DisplayTime = 2.0f);

	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|Debug")
	static void LogWarning(const FString& Message, bool bShowInViewport = true, float DisplayTime = 2.0f);

	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|Debug")
	static void LogError(const FString& Message, bool bShowInViewport = true, float DisplayTime = 2.0f);
};