#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TsubasamusuTextureConvertLibrary.generated.h"

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UTsubasamusuTextureConvertLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "TSUBASAMUSU|Convert|Texture")
	static UTexture2D* ConvertTextureRenderTargetToTexture(UTextureRenderTarget2D* TextureRenderTarget);

	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|Convert|Texture")
	static TArray<uint8> ConvertTextureToByteArray(UTexture2D* Texture);

	UFUNCTION(BlueprintPure, Category = "TSUBASAMUSU|Convert|Texture")
	static UTexture2D* ConvertByteArrayToTexture(const TArray<uint8>& ByteArray, int32 TextureWidth = 256, int32 TextureHeight = 256, EPixelFormat PixelFormat = PF_B8G8R8A8);

	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|Convert|Texture")
	static TArray<uint8> ConvertTextureToPngData(UTexture2D* Texture);
};