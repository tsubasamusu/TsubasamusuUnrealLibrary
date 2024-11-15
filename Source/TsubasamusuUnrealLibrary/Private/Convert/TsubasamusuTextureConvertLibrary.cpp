#include "Convert/TsubasamusuTextureConvertLibrary.h"
#include "Debug/TsubasamusuLogLibrary.h"
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
    if (ByteArray.Num() == 0)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a byte array to a Texture2D failed because the byte array size is 0."));

        return nullptr;
    }

    UTexture2D* Texture = UTexture2D::CreateTransient(TextureWidth, TextureHeight, PixelFormat);

    if (!IsValid(Texture))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a byte array to a Texture2D failed because a Texture2D creation failed."));

        return nullptr;
    }

    FTexturePlatformData* TexturePlatformData = Texture->GetPlatformData();

    if (!TexturePlatformData)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a byte array to a Texture2D failed because getting a platform data from the Texture2D failed."));

        return nullptr;
    }

    if (TexturePlatformData->Mips.Num() == 0)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a byte array to a Texture2D failed because no mipmaps available in the Texture2D."));

        return nullptr;
    }

    FTexture2DMipMap& MipMap = TexturePlatformData->Mips[0];

    int32 BulkDataSize = MipMap.BulkData.GetBulkDataSize();

    if (BulkDataSize == 0)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a byte array to a Texture2D failed because the bulk data size is 0."));

        return nullptr;
    }

    if (ByteArray.Num() != BulkDataSize)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a byte array to a Texture2D failed because The byte array size does not match the expected texture data size."));

        return nullptr;
    }

    void* Data = MipMap.BulkData.Lock(LOCK_READ_WRITE);

    if (!Data)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a byte array to a Texture2D failed because locking the texture data failed."));

        MipMap.BulkData.Unlock();

        return nullptr;
    }

    FMemory::Memcpy(Data, ByteArray.GetData(), ByteArray.Num());

    MipMap.BulkData.Unlock();

    Texture->UpdateResource();

    if (!IsValid(Texture))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a byte array to a Texture2D failed because the converted Texture2D is not valid."));

        return nullptr;
    }

    return Texture;
}

TArray<uint8> UTsubasamusuTextureConvertLibrary::ConvertTextureToImageData(UTexture2D* Texture, EImageDataFormat ImageDataFormat)
{
    if (!IsValid(Texture))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a Texture2D to a image data failed because the Texture2D is not valid."));

        return TArray<uint8>();
    }

    FTexturePlatformData* TexturePlatformData = Texture->GetPlatformData();

    if (!TexturePlatformData)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a Texture2D to a image data failed because getting a platform data from the Texture2D failed."));

        return TArray<uint8>();
    }

    if (TexturePlatformData->Mips.Num() == 0)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a Texture2D to a image data failed because no mipmaps available in the Texture2D."));

        return TArray<uint8>();
    }

    FTexture2DMipMap& MipMap = TexturePlatformData->Mips[0];

    int32 BulkDataSize = MipMap.BulkData.GetBulkDataSize();

    if (BulkDataSize == 0)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a Texture2D to a image data failed because the bulk data size is 0."));

        return TArray<uint8>();
    }

    int32 TextureWidth = Texture->GetSizeX();
    int32 TextureHeight = Texture->GetSizeY();

    int32 ExpectedDataSize = TextureWidth * TextureHeight * 4;

    if (BulkDataSize != ExpectedDataSize)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a Texture2D to a image data failed because the bulk data size does not match the expected image data size."));

        return TArray<uint8>();
    }

    void* Data = MipMap.BulkData.Lock(LOCK_READ_ONLY);

    if (!Data)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a Texture2D to a image data failed because locking the bulk data failed."));

        MipMap.BulkData.Unlock();

        return TArray<uint8>();
    }

    TArray<uint8> RawData;

    RawData.Append(static_cast<uint8*>(Data), ExpectedDataSize);

    MipMap.BulkData.Unlock();

    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));

    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ConvertImageDataFormatToImageFormat(ImageDataFormat));

    if (!ImageWrapper.IsValid())
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a Texture2D to a image data failed because an ImageWrapper creation failed."));

        return TArray<uint8>();
    }

    if (!ImageWrapper->SetRaw(RawData.GetData(), RawData.Num(), TextureWidth, TextureHeight, ERGBFormat::BGRA, 8))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a Texture2D to a image data failed because setting raw to the ImageWrapper failed."));

        return TArray<uint8>();
    }

    TArray<uint8> ImageData;

    ImageData = ImageWrapper->GetCompressed();

    if (ImageData.Num() == 0)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Converting a Texture2D to a image data failed because the converted image data size is 0."));

        return TArray<uint8>();
    }

    return ImageData;
}

EImageFormat UTsubasamusuTextureConvertLibrary::ConvertImageDataFormatToImageFormat(EImageDataFormat ImageDataFormat)
{
    return static_cast<EImageFormat>(static_cast<int32>(ImageDataFormat));
}