#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UTsubasamusuMathLibrary.generated.h"

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UTsubasamusuMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|Math", meta = (Keywords = "cut decimal"))
	static double CutDecimal(const double Value, const int DecimalPlace = 1);
};