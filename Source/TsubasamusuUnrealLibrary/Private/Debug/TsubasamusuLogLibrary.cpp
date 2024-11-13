#include "Debug/TsubasamusuLogLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogTsubasamusu, Log, All);
DEFINE_LOG_CATEGORY_STATIC(LogTsubasamusuWarning, Warning, All);
DEFINE_LOG_CATEGORY_STATIC(LogTsubasamusuError, Error, All);

void UTsubasamusuLogLibrary::Log(const FString& Message, bool bShowInViewort, float DisplayTime, FLinearColor TextColor)
{
	UE_LOG(LogTsubasamusu, Log, TEXT("%s"), *Message);

	if (!bShowInViewort) return;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DisplayTime, TextColor.ToFColor(true), Message);
}

void UTsubasamusuLogLibrary::LogWarning(const FString& Message, bool bShowInViewort, float DisplayTime, FLinearColor TextColor)
{
	UE_LOG(LogTsubasamusuWarning, Warning, TEXT("%s"), *Message);

	if (!bShowInViewort) return;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DisplayTime, TextColor.ToFColor(true), Message);
}

void UTsubasamusuLogLibrary::LogError(const FString& Message, bool bShowInViewort, float DisplayTime, FLinearColor TextColor)
{
	UE_LOG(LogTsubasamusuError, Error, TEXT("%s"), *Message);

	if (!bShowInViewort) return;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DisplayTime, TextColor.ToFColor(true), Message);
}