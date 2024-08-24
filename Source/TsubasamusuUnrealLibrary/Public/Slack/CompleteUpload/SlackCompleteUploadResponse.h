#pragma once

#include "CoreMinimal.h"
#include "SlackCompleteUploadResponse.generated.h"

USTRUCT(BlueprintType)
struct FSlackCompleteUploadResponse
{
	GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool ok;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString error;
};