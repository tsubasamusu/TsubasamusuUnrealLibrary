#include "Math/TsubasamusuFloatLibrary.h"
#include "Debug/TsubasamusuLogLibrary.h"

double UTsubasamusuFloatLibrary::CutDecimal(const double Value, const int DecimalPlace)
{
	if (DecimalPlace < 1)
	{
		UTsubasamusuLogLibrary::LogError(TEXT("The \"DecimalPlace\" in \"UMathLibrary::CutDecimal()\" must be greater than or equal to 1."));

		return 0.0;
	}

	double PowerOfTen = FMath::Pow(10.0, DecimalPlace);

	return FMath::TruncToDouble(Value * PowerOfTen) / PowerOfTen;
}