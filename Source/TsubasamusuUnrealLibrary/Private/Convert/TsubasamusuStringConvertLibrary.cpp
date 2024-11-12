#include "Convert/TsubasamusuStringConvertLibrary.h"

FString UTsubasamusuStringConvertLibrary::ConvertToBase64(const FString& String)
{
    FTCHARToUTF8 Utf8String(*String);

    return FBase64::Encode((const uint8*)Utf8String.Get(), Utf8String.Length());
}