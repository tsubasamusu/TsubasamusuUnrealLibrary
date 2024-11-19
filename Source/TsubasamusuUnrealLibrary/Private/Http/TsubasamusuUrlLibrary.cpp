#include "Http/TsubasamusuUrlLibrary.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Debug/TsubasamusuLogLibrary.h"

FString UTsubasamusuUrlLibrary::AddQueryParameters(const FString& SourceUrl, const TMap<FString, FString>& QueryParameters)
{
    if (SourceUrl.IsEmpty())
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Cannot add query parameters to an empty URL."));

        return TEXT("");
    }

    if (QueryParameters.Num() == 0)
    {
        UTsubasamusuLogLibrary::LogWarning(TEXT("The size of query parameters for \"") + SourceUrl + TEXT("\" is 0."));

        return SourceUrl;
    }

    FString NewUrl = SourceUrl;

    if (!NewUrl.Contains(TEXT("?"))) NewUrl.Append(TEXT("?"));

    bool bIsFirstParameter = NewUrl.EndsWith(TEXT("?"));

    for (const TTuple<FString, FString>& QueryParameter : QueryParameters)
    {
        if (QueryParameter.Key.IsEmpty())
        {
            UTsubasamusuLogLibrary::LogWarning(TEXT("Key of the query parameters is empty."));

            continue;
        }

        if (QueryParameter.Value.IsEmpty())
        {
            UTsubasamusuLogLibrary::LogWarning(TEXT("Value of the query parameters is empty."));

            continue;
        }

        FString EncodedKey = FGenericPlatformHttp::UrlEncode(QueryParameter.Key);
        FString EncodedValue = FGenericPlatformHttp::UrlEncode(QueryParameter.Value);

        if (!bIsFirstParameter && !NewUrl.EndsWith(TEXT("&"))) NewUrl.Append(TEXT("&"));

        NewUrl.Append(EncodedKey + TEXT("=") + EncodedValue);

        bIsFirstParameter = false;
    }

    return NewUrl;
}