#include "Graphic/TsubasamusuCaptureLibrary.h"
#include "Camera/CameraComponent.h"
#include "Debug/TsubasamusuLogLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/SceneCapture2D.h"
#include "Convert/TsubasamusuTextureConvertLibrary.h"

UTexture2D* UTsubasamusuCaptureLibrary::GetCameraViewTexture(UObject* WorldContextObject, UCameraComponent* CameraComponent, int32 TextureWidth /*= 256*/, int32 TextureHeight /*= 256*/, bool bUseCameraFieldOfView /*= true*/)
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

	ASceneCapture2D* SceneCaptureActor = WorldContextObject->GetWorld()->SpawnActor<ASceneCapture2D>();

	SceneCaptureActor->SetActorLocation(CameraComponent->GetComponentLocation());
	SceneCaptureActor->SetActorRotation(CameraComponent->GetComponentRotation());

	USceneCaptureComponent2D* SceneCaptureComponent2D = SceneCaptureActor->GetCaptureComponent2D();

	SceneCaptureComponent2D->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
	SceneCaptureComponent2D->bCaptureEveryFrame = false;
	SceneCaptureComponent2D->bCaptureOnMovement = false;
	SceneCaptureComponent2D->bAlwaysPersistRenderingState = true;
	SceneCaptureComponent2D->TextureTarget = TextureRenderTarget2D;
	if (bUseCameraFieldOfView) SceneCaptureComponent2D->FOVAngle = CameraComponent->FieldOfView;

	SceneCaptureComponent2D->CaptureScene();

	return UTsubasamusuTextureConvertLibrary::ConvertTextureRenderTargetToTexture2D(TextureRenderTarget2D);
}