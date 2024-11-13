#include "Debug/TsubasamusuLogLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogTsubasamusu, Log, All);
DEFINE_LOG_CATEGORY_STATIC(LogTsubasamusuWarning, Warning, All);
DEFINE_LOG_CATEGORY_STATIC(LogTsubasamusuError, Error, All);

void UTsubasamusuLogLibrary::Log(const FString& Message, bool bShowInViewort, float DisplayTime)
{
	UE_LOG(LogTsubasamusu, Log, TEXT("%s"), *Message);

	if (!bShowInViewort) return;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DisplayTime, FColor::Green, Message);
}

void UTsubasamusuLogLibrary::LogWarning(const FString& Message, bool bShowInViewort, float DisplayTime)
{
	UE_LOG(LogTsubasamusuWarning, Warning, TEXT("%s"), *Message);

	if (!bShowInViewort) return;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DisplayTime, FColor::Yellow, Message);
}

void UTsubasamusuLogLibrary::LogError(const FString& Message, bool bShowInViewort, float DisplayTime)
{
	UE_LOG(LogTsubasamusuError, Error, TEXT("%s"), *Message);

	if (!bShowInViewort) return;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DisplayTime, FColor::Red, Message);
}