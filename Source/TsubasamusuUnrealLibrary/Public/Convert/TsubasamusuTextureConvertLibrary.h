#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TsubasamusuTextureConvertLibrary.generated.h"

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UTsubasamusuTextureConvertLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "TSUBASAMUSU|Convert", meta = (Keywords = "convert texture render target to texture 2d"))
	static UTexture2D* ConvertTextureRenderTargetToTexture2D(UTextureRenderTarget2D* TextureRenderTarget2D);

	UFUNCTION(BlueprintPure, Category = "TSUBASAMUSU|Convert", meta = (Keywords = "convert texture to byte array"))
	static TArray<uint8> ConvertTextureToByteArray(UTexture2D* Texture2D);

	UFUNCTION(BlueprintPure, Category = "TSUBASAMUSU|Convert", meta = (Keywords = "convert byte array to texture"))
	static UTexture2D* ConvertByteArrayToTexture(const TArray<uint8>& ByteArray, const int32 TextureWidth = 512, const int32 TextureHeight = 512);

	UFUNCTION(BlueprintPure, Category = "TSUBASAMUSU|Convert", meta = (Keywords = "convert texture to png data"))
	static TArray<uint8> ConvertTextureToPngData(UTexture2D* Texture2D);
};