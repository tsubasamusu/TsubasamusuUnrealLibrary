#include "Convert/TsubasamusuTextureConvertLibrary.h"
#include "Debug/TsubasamusuLogLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "IImageWrapperModule.h"

UTexture2D* UTsubasamusuTextureConvertLibrary::ConvertTextureRenderTargetToTexture2D(UTextureRenderTarget2D* TextureRenderTarget2D)
{
    if (!IsValid(TextureRenderTarget2D))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("The \"TextureRenderTarget2D\" is not valid."));

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

    int32 ExpectedDataSize = Texture2D->GetPlatformData()->Mips[0].BulkData.GetBulkDataSize();

    if (ImageColors.Num() * sizeof(FColor) != ExpectedDataSize)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("The data size mismatch."));

        return nullptr;
    }

    void* TextureData = Texture2D->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);

    FMemory::Memcpy(TextureData, ImageColors.GetData(), ImageColors.Num() * sizeof(FColor));

    Texture2D->GetPlatformData()->Mips[0].BulkData.Unlock();

    Texture2D->UpdateResource();

    return Texture2D;
}

TArray<uint8> UTsubasamusuTextureConvertLibrary::ConvertTextureToByteArray(UTexture2D* Texture2D)
{
    TArray<uint8> TextureData;

    if (!IsValid(Texture2D))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("The \"Texture2D\" is not valid."));

        return TextureData;
    }

    if (!Texture2D->GetPlatformData() || Texture2D->GetPlatformData()->Mips.Num() == 0)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Invalid PlatformData or no mipmaps available."));

        return TextureData;
    }

    FTexture2DMipMap& Texture2DMipMap = Texture2D->GetPlatformData()->Mips[0];

    void* Data = Texture2DMipMap.BulkData.Lock(LOCK_READ_ONLY);

    if (!Data)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Failed to lock the texture data."));

        return TextureData;
    }

    int32 BulkDataSize = Texture2DMipMap.BulkData.GetBulkDataSize();

    if (BulkDataSize > 0)
    {
        TextureData.Append(static_cast<uint8*>(Data), BulkDataSize);
    }
    else
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Bulk data size is zero."));
    }

    Texture2DMipMap.BulkData.Unlock();

    return TextureData;
}

UTexture2D* UTsubasamusuTextureConvertLibrary::ConvertByteArrayToTexture(const TArray<uint8>& ByteArray, int32 TextureWidth, int32 TextureHeight)
{
    UTexture2D* Texture2D = UTexture2D::CreateTransient(TextureWidth, TextureHeight, EPixelFormat::PF_B8G8R8A8);

    if (!IsValid(Texture2D))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Failed to create a Texture2D."));

        return nullptr;
    }

    int32 BulkDataSize = Texture2D->GetPlatformData()->Mips[0].BulkData.GetBulkDataSize();

    if (ByteArray.Num() != BulkDataSize)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("The \"ByteArray\" size does not match the expected texture data size."));

        return nullptr;
    }

    void* TextureData = nullptr;

    TextureData = Texture2D->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);

    FMemory::Memcpy(TextureData, ByteArray.GetData(), ByteArray.Num());

    Texture2D->GetPlatformData()->Mips[0].BulkData.Unlock();

    Texture2D->UpdateResource();

    return Texture2D;
}

TArray<uint8> UTsubasamusuTextureConvertLibrary::ConvertTextureToPngData(UTexture2D* Texture2D)
{
    TArray<uint8> PngData;

    if (!IsValid(Texture2D))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("The \"Texture2D\" is not valid."));

        return PngData;
    }

    if (!Texture2D->GetPlatformData() || Texture2D->GetPlatformData()->Mips.Num() == 0)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Invalid PlatformData or no mipmaps available."));

        return PngData;
    }

    FTexture2DMipMap& Mip = Texture2D->GetPlatformData()->Mips[0];

    void* Data = Mip.BulkData.Lock(LOCK_READ_ONLY);

    if (!Data)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Failed to lock the texture data."));

        return PngData;
    }

    int32 TextureWidth = Texture2D->GetSizeX();
    int32 TextureHeight = Texture2D->GetSizeY();

    int32 ExpectedDataSize = TextureWidth * TextureHeight * 4;

    TArray<uint8> RawData;

    if (Mip.BulkData.GetBulkDataSize() == ExpectedDataSize)
    {
        RawData.Append(static_cast<uint8*>(Data), ExpectedDataSize);
    }
    else
    {
        UTsubasamusuLogLibrary::LogError(TEXT("The texture size does not match the expected texture data size."));
        
        Mip.BulkData.Unlock();
        
        return PngData;
    }

    Mip.BulkData.Unlock();

    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
    
    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

    if (ImageWrapper.IsValid() && ImageWrapper->SetRaw(RawData.GetData(), RawData.Num(), TextureWidth, TextureHeight, ERGBFormat::BGRA, 8))
    {
        PngData = ImageWrapper->GetCompressed();
    }
    else
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Failed to convert the texture to PNG data."));
    }

    return PngData;
}