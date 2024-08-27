#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TsubasamusuCaptureLibrary.generated.h"

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UTsubasamusuCaptureLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "TSUBASAMUSU|Graphic", meta = (Keywords = "get camera view texture", WorldContext = "WorldContextObject"))
	static UTexture2D* GetCameraViewTexture(UObject* WorldContextObject, UCameraComponent* CameraComponent, int32 TextureWidth = 256, int32 TextureHeight = 256, FPostProcessSettings PostProcessSettings = FPostProcessSettings());
};