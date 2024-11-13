#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TsubasamusuLogLibrary.generated.h"

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UTsubasamusuLogLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|Debug", meta = (AdvancedDisplay = "1"))
	static void Log(const FString& Message = FString(TEXT("Log")), bool bShowInViewport = true, float DisplayTime = 5.0f, FLinearColor TextColor = FLinearColor::Green);

	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|Debug", meta = (AdvancedDisplay = "1"))
	static void LogWarning(const FString& Message = FString(TEXT("Warning")), bool bShowInViewport = true, float DisplayTime = 5.0f, FLinearColor TextColor = FLinearColor::Yellow);

	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|Debug", meta = (AdvancedDisplay = "1"))
	static void LogError(const FString& Message = FString(TEXT("Error")), bool bShowInViewport = true, float DisplayTime = 5.0f, FLinearColor TextColor = FLinearColor::Red);
};