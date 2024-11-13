#include "Convert/TsubasamusuStringConvertLibrary.h"

FString UTsubasamusuStringConvertLibrary::ConvertToBase64(const FString& String)
{
    FTCHARToUTF8 Utf8String(*String);

    return FBase64::Encode((const uint8*)Utf8String.Get(), Utf8String.Length());
}

TArray<uint8> UTsubasamusuStringConvertLibrary::ConvertToUtf8(const FString& String)
{
    FTCHARToUTF8 Utf8(*String);

    TArray<uint8> StringData;

    StringData.Append((uint8*)Utf8.Get(), Utf8.Length());

    return StringData;
}

TArray<char> UTsubasamusuStringConvertLibrary::ConvertToCharArray(const FString& String)
{
    FTCHARToUTF8 UTF8String(*String);

    TArray<char> CharArray;

    CharArray.Reserve(UTF8String.Length());

    CharArray.Append(UTF8String.Get(), UTF8String.Length());

    return CharArray;
}