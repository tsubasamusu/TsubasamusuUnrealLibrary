#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TextureConvertLibrary.generated.h"

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UTextureConvertLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "TSUBASAMUSU|Convert", meta = (Keywords = "convert texture render target to texture 2d"))
	static UTexture2D* ConvertTextureRenderTargetToTexture2D(UTextureRenderTarget2D* TextureRenderTarget2D);
};