#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TsubasamusuStringConvertLibrary.generated.h"

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UTsubasamusuStringConvertLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "TSUBASAMUSU|Convert|String")
	static FString ConvertToBase64(const FString& String);

	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|Convert|String")
	static TArray<uint8> ConvertToUtf8(const FString& String);
	
	static TArray<char> ConvertToCharArray(const FString& String);
};