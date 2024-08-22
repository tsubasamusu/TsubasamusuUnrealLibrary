#include "Http/TsubasamusuUrlLibrary.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Debug/TsubasamusuLogLibrary.h"

FString UTsubasamusuUrlLibrary::AddQueryParameters(const FString& SourceURL, const TMap<FString, FString>& QueryParameters)
{
    if (SourceURL.IsEmpty())
    {
        UTsubasamusuLogLibrary::LogError(TEXT("The \"SourceURL\" is empty."));

        return FString();
    }

    FString NewURL = SourceURL;

    if (!NewURL.Contains(TEXT("?"))) NewURL.Append(TEXT("?"));

    bool bIsFirstParameter = NewURL.EndsWith(TEXT("?"));

    for (const TTuple<FString, FString>& QueryParameter : QueryParameters)
    {
        FString EncodedKey = FGenericPlatformHttp::UrlEncode(QueryParameter.Key);
        FString EncodedValue = FGenericPlatformHttp::UrlEncode(QueryParameter.Value);

        if (!bIsFirstParameter && !NewURL.EndsWith(TEXT("&"))) NewURL.Append(TEXT("&"));

        NewURL.Append(EncodedKey + TEXT("=") + EncodedValue);

        bIsFirstParameter = false;
    }

    return NewURL;
}