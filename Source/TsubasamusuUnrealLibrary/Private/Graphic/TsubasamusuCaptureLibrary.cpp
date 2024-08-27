#include "Graphic/TsubasamusuCaptureLibrary.h"
#include "Convert/TsubasamusuTextureConvertLibrary.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/SceneCapture2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Debug/TsubasamusuLogLibrary.h"

UTexture2D* UTsubasamusuCaptureLibrary::GetCameraViewTexture(UObject* WorldContextObject, UCameraComponent* CameraComponent, int32 TextureWidth, int32 TextureHeight, bool bUseCameraFieldOfView, FPostProcessSettings PostProcessSettings)
{
    if (!IsValid(WorldContextObject))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("The \"WorldContextObject\" is not valid."));

        return nullptr;
    }

    if (!IsValid(CameraComponent))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("The \"CameraComponent\" is not valid."));

        return nullptr;
    }

    UTextureRenderTarget2D* TextureRenderTarget2D = UKismetRenderingLibrary::CreateRenderTarget2D(WorldContextObject, TextureWidth, TextureHeight, ETextureRenderTargetFormat::RTF_RGBA8);

    if (!IsValid(TextureRenderTarget2D))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Failed to create a TextureRenderTarget2D."));

        return nullptr;
    }

    ASceneCapture2D* SceneCaptureActor = WorldContextObject->GetWorld()->SpawnActor<ASceneCapture2D>();

    if (!IsValid(SceneCaptureActor))
    {
        UTsubasamusuLogLibrary::LogError(TEXT("Failed to spawn a SceneCapture2D Actor."));

        return nullptr;
    }

    SceneCaptureActor->SetActorLocation(CameraComponent->GetComponentLocation());
    SceneCaptureActor->SetActorRotation(CameraComponent->GetComponentRotation());

    USceneCaptureComponent2D* SceneCaptureComponent2D = SceneCaptureActor->GetCaptureComponent2D();

    SceneCaptureComponent2D->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
    SceneCaptureComponent2D->bCaptureEveryFrame = false;
    SceneCaptureComponent2D->bCaptureOnMovement = false;
    SceneCaptureComponent2D->bAlwaysPersistRenderingState = true;
    SceneCaptureComponent2D->TextureTarget = TextureRenderTarget2D;
    if (bUseCameraFieldOfView) SceneCaptureComponent2D->FOVAngle = CameraComponent->FieldOfView;

    SceneCaptureComponent2D->PostProcessSettings = PostProcessSettings;

    SceneCaptureComponent2D->CaptureScene();

    UTexture2D* Texture2D = UTsubasamusuTextureConvertLibrary::ConvertTextureRenderTargetToTexture2D(SceneCaptureComponent2D->TextureTarget);

    SceneCaptureActor->Destroy();

    return Texture2D;
}