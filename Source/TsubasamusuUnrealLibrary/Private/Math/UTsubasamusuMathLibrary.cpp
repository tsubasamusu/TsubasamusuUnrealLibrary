#include "Math/UTsubasamusuMathLibrary.h"
#include "Debug/LogUtility.h"

float UTsubasamusuMathLibrary::CutDecimal(const float Value, const int DecimalPlace)
{
	if (DecimalPlace < 1)
	{
		ULogUtility::LogError(TEXT("The \"DecimalPlace\" in \"UMathLibrary::CutDecimal()\" must be greater than or equal to 1."));

		return 0;
	}

	return 0;
}