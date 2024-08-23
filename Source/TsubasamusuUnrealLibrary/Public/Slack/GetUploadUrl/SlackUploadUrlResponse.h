#pragma once

#include "CoreMinimal.h"
#include "SlackUploadUrlResponse.generated.h"

USTRUCT(BlueprintType)
struct FSlackUploadUrlResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool ok;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString upload_url;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString file_id;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString error;
};