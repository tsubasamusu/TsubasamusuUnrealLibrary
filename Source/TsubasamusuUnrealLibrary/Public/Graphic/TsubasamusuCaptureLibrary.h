#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "TsubasamusuCaptureLibrary.generated.h"

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UTsubasamusuCaptureLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "TSUBASAMUSU|Graphic", meta = (WorldContext = "WorldContextObject"))
	static UTexture2D* GetCameraViewTexture(UObject* WorldContextObject, UCameraComponent* CameraComponent, const FPostProcessSettings& PostProcessSettings, int32 TextureWidth = 256, int32 TextureHeight = 256, ETextureRenderTargetFormat TextureRenderTargetFormat = RTF_RGBA8_SRGB, ESceneCaptureSource SceneCaptureSource = SCS_FinalToneCurveHDR);
};