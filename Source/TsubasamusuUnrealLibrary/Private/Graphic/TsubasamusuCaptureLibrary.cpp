#include "Graphic/TsubasamusuCaptureLibrary.h"
#include "Convert/TsubasamusuTextureConvertLibrary.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/SceneCapture2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Debug/TsubasamusuLogLibrary.h"

UTexture2D* UTsubasamusuCaptureLibrary::GetCameraViewTexture(UObject* WorldContextObject, UCameraComponent* TargetCameraComponent, int32 TextureWidth, int32 TextureHeight, FPostProcessSettings PostProcessSettings, ETextureRenderTargetFormat TextureRenderTargetFormat, ESceneCaptureSource SceneCaptureSource)
{
    if (!IsValid(WorldContextObject))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Getting a camera view texture failed because the world context object is not valid."));

        return nullptr;
    }

    if (!IsValid(TargetCameraComponent))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Getting a camera view texture failed because the camera component is not valid."));

        return nullptr;
    }

    UTextureRenderTarget2D* TextureRenderTarget2D = UKismetRenderingLibrary::CreateRenderTarget2D(WorldContextObject, TextureWidth, TextureHeight, TextureRenderTargetFormat);

    if (!IsValid(TextureRenderTarget2D))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Getting a camera view texture failed because a TextureRenderTarget2D creation failed."));

        return nullptr;
    }

    ASceneCapture2D* SceneCaptureActor = WorldContextObject->GetWorld()->SpawnActor<ASceneCapture2D>();

    if (!IsValid(SceneCaptureActor))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Getting a camera view texture failed because a SceneCapture2D actor failed to spawn."));

        return nullptr;
    }

    SceneCaptureActor->SetActorLocation(TargetCameraComponent->GetComponentLocation());
    SceneCaptureActor->SetActorRotation(TargetCameraComponent->GetComponentRotation());

    USceneCaptureComponent2D* SceneCaptureComponent2D = SceneCaptureActor->GetCaptureComponent2D();

    SceneCaptureComponent2D->CaptureSource = SceneCaptureSource;
    SceneCaptureComponent2D->bCaptureEveryFrame = false;
    SceneCaptureComponent2D->bCaptureOnMovement = false;
    SceneCaptureComponent2D->bAlwaysPersistRenderingState = true;
    SceneCaptureComponent2D->TextureTarget = TextureRenderTarget2D;
    SceneCaptureComponent2D->FOVAngle = TargetCameraComponent->FieldOfView;
    SceneCaptureComponent2D->PostProcessSettings = PostProcessSettings;
    SceneCaptureComponent2D->CaptureScene();

    UTexture2D* Texture = UTsubasamusuTextureConvertLibrary::ConvertTextureRenderTargetToTexture2D(SceneCaptureComponent2D->TextureTarget);

    if (!SceneCaptureActor->Destroy())
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Getting a camera view texture failed because the SceneCapture2D actor failed to destroy."));

        return nullptr;
    }

    if (!IsValid(Texture))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Getting a camera view texture failed because the TextureRenderTarget2D conversion failed."));

        return nullptr;
    }

    return Texture;
}