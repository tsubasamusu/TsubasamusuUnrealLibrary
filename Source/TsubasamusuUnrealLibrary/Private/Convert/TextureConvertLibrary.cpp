#include "Convert/TextureConvertLibrary.h"
#include "Debug/TsubasamusuLogLibrary.h"
#include "Engine/TextureRenderTarget2D.h"

UTexture2D* UTextureConvertLibrary::ConvertTextureRenderTargetToTexture2D(UTextureRenderTarget2D* TextureRenderTarget2D)
{
    if (!IsValid(TextureRenderTarget2D))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("The \"TextureRenderTarget2D\" is not valid."));

        return nullptr;
    }

    if (TextureRenderTarget2D->GetFormat() != EPixelFormat::PF_B8G8R8A8)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("The format of the \"TextureRenderTarget2D\" should be \"EPixelFormat::PF_B8G8R8A8\"."));

        return nullptr;
    }

    UTexture2D* Texture2D = UTexture2D::CreateTransient(TextureRenderTarget2D->SizeX, TextureRenderTarget2D->SizeY, TextureRenderTarget2D->GetFormat());

    if (!IsValid(Texture2D))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Failed to create a Texture2D."));

        return nullptr;
    }

    FTextureRenderTargetResource* TextureRenderTargetResource = TextureRenderTarget2D->GameThread_GetRenderTargetResource();

    TArray<FColor> ImageColors;

    TextureRenderTargetResource->ReadPixels(ImageColors);

    int32 ExpectedDataSize = Texture2D->PlatformData->Mips[0].BulkData.GetBulkDataSize();

    if (ImageColors.Num() * sizeof(FColor) != ExpectedDataSize)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("The data size mismatch."));

        return nullptr;
    }

    void* TextureData = Texture2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);

    FMemory::Memcpy(TextureData, ImageColors.GetData(), ImageColors.Num() * sizeof(FColor));

    Texture2D->PlatformData->Mips[0].BulkData.Unlock();

    Texture2D->UpdateResource();

    return Texture2D;
}