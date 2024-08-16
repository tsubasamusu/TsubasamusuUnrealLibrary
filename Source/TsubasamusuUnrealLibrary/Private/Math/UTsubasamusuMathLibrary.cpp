#include "Math/UTsubasamusuMathLibrary.h"
#include "Debug/LogUtility.h"

double UTsubasamusuMathLibrary::CutDecimal(const double Value, const int DecimalPlace)
{
	if (DecimalPlace < 1)
	{
		ULogUtility::LogError(TEXT("The \"DecimalPlace\" in \"UMathLibrary::CutDecimal()\" must be greater than or equal to 1."));

		return 0.0;
	}

	double PowerOfTen = FMath::Pow(10.0, DecimalPlace);

	return FMath::TruncToDouble(Value * PowerOfTen) / PowerOfTen;
}