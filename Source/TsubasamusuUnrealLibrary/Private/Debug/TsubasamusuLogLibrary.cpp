#include "Debug/TsubasamusuLogLibrary.h"

void UTsubasamusuLogLibrary::Log(const FString& Message, bool bShowInViewort, float DisplayTime)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *Message);

	if (!bShowInViewort) return;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DisplayTime, FColor::Green, Message);
}

void UTsubasamusuLogLibrary::LogWarning(const FString& Message, bool bShowInViewort, float DisplayTime)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);

	if (!bShowInViewort) return;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DisplayTime, FColor::Yellow, Message);
}

void UTsubasamusuLogLibrary::LogError(const FString& Message, bool bShowInViewort, float DisplayTime)
{
	UE_LOG(LogTemp, Error, TEXT("%s"), *Message);

	if (!bShowInViewort) return;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DisplayTime, FColor::Red, Message);
}