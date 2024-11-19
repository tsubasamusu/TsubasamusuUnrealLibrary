#include "Math/TsubasamusuFloatLibrary.h"
#include "Debug/TsubasamusuLogLibrary.h"

double UTsubasamusuFloatLibrary::CutDecimal(double FloatValue, int32 DecimalPlace)
{
	if (DecimalPlace < 1)
	{
		UTsubasamusuLogLibrary::LogError(TEXT("The argument named \"DecimalPlace\" must be greater than or equal to 1."));

		return 0.0;
	}

	double PowerOfTen = FMath::Pow(10.0, DecimalPlace);

	return FMath::TruncToDouble(FloatValue * PowerOfTen) / PowerOfTen;
}