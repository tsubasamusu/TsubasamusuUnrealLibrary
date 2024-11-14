#include "Convert/TsubasamusuTextureConvertLibrary.h"
#include "Debug/TsubasamusuLogLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "IImageWrapperModule.h"

UTexture2D* UTsubasamusuTextureConvertLibrary::ConvertTextureRenderTargetToTexture(UTextureRenderTarget2D* TextureRenderTarget)
{
    if (!IsValid(TextureRenderTarget))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a TextureRenderTarget2D to a Texture2D failed because the TextureRenderTarget2D is not valid."));

        return nullptr;
    }

    UTexture2D* Texture = UTexture2D::CreateTransient(TextureRenderTarget->SizeX, TextureRenderTarget->SizeY, TextureRenderTarget->GetFormat());

    if (!IsValid(Texture))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a TextureRenderTarget2D to a Texture2D failed because a Texture2D creation failed."));

        return nullptr;
    }

    FTextureRenderTargetResource* TextureRenderTargetResource = TextureRenderTarget->GameThread_GetRenderTargetResource();

    TArray<FColor> ImageColors;

    if (!TextureRenderTargetResource->ReadPixels(ImageColors))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a TextureRenderTarget2D to a Texture2D failed because reading pixels of the TextureRenderTargetResource failed."));

        return nullptr;
    }

    int32 ExpectedDataSize = Texture->GetPlatformData()->Mips[0].BulkData.GetBulkDataSize();

    if (ImageColors.Num() * sizeof(FColor) != ExpectedDataSize)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a TextureRenderTarget2D to a Texture2D failed because the data size mismatch."));

        return nullptr;
    }

    void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);

    FMemory::Memcpy(TextureData, ImageColors.GetData(), ImageColors.Num() * sizeof(FColor));

    Texture->GetPlatformData()->Mips[0].BulkData.Unlock();

    Texture->UpdateResource();

    if (!IsValid(Texture))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a TextureRenderTarget2D to a Texture2D failed."));

        return nullptr;
    }

    return Texture;
}

TArray<uint8> UTsubasamusuTextureConvertLibrary::ConvertTextureToByteArray(UTexture2D* Texture)
{
    if (!IsValid(Texture))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a Texture2D to a byte array failed because the Texture2D is not valid."));

        return TArray<uint8>();
    }

    FTexturePlatformData* TexturePlatformData = Texture->GetPlatformData();

    if (!TexturePlatformData)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a Texture2D to a byte array failed because getting a platform data from the Texture2D failed."));

        return TArray<uint8>();
    }

    if (TexturePlatformData->Mips.Num() == 0)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a Texture2D to a byte array failed because no mipmaps available in the Texture2D."));

        return TArray<uint8>();
    }

    FTexture2DMipMap& MipMap = TexturePlatformData->Mips[0];

    int32 BulkDataSize = MipMap.BulkData.GetBulkDataSize();

    if (BulkDataSize == 0)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a Texture2D to a byte array failed because the bulk data size is 0."));

        return TArray<uint8>();
    }

    void* Data = MipMap.BulkData.Lock(LOCK_READ_ONLY);

    if (!Data)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a Texture2D to a byte array failed because locking the texture data failed."));

        MipMap.BulkData.Unlock();

        return TArray<uint8>();
    }

    TArray<uint8> TextureData;

    TextureData.Append(static_cast<uint8*>(Data), BulkDataSize);

    MipMap.BulkData.Unlock();

    if (TextureData.Num() == 0)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a Texture2D to a byte array failed because the texture data size is 0."));

        return TArray<uint8>();
    }

    return TextureData;
}

UTexture2D* UTsubasamusuTextureConvertLibrary::ConvertByteArrayToTexture(const TArray<uint8>& ByteArray, int32 TextureWidth, int32 TextureHeight, EPixelFormat PixelFormat)
{
    UTexture2D* Texture2D = UTexture2D::CreateTransient(TextureWidth, TextureHeight, PixelFormat);

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

TArray<uint8> UTsubasamusuTextureConvertLibrary::ConvertTextureToPngData(UTexture2D* Texture)
{
    TArray<uint8> PngData;

    if (!IsValid(Texture))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("The \"Texture2D\" is not valid."));

        return PngData;
    }

    if (!Texture->GetPlatformData() || Texture->GetPlatformData()->Mips.Num() == 0)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Invalid PlatformData or no mipmaps available."));

        return PngData;
    }

    FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];

    void* Data = Mip.BulkData.Lock(LOCK_READ_ONLY);

    if (!Data)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Failed to lock the texture data."));

        return PngData;
    }

    int32 TextureWidth = Texture->GetSizeX();
    int32 TextureHeight = Texture->GetSizeY();

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