#include "Debug/TsubasamusuLogLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UTsubasamusuLogLibrary::Log(const FString Message, const bool bShowInViewort, const float DisplayTime)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *Message);

	UKismetSystemLibrary::PrintString(nullptr, Message, true, false, FLinearColor::Green, DisplayTime);
}

void UTsubasamusuLogLibrary::LogWarning(const FString Message, const bool bShowInViewort, const float DisplayTime)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);

	UKismetSystemLibrary::PrintString(nullptr, Message, true, false, FLinearColor::Yellow, DisplayTime);
}

void UTsubasamusuLogLibrary::LogError(const FString Message, const bool bShowInViewort, const float DisplayTime)
{
	UE_LOG(LogTemp, Error, TEXT("%s"), *Message);

	UKismetSystemLibrary::PrintString(nullptr, Message, true, false, FLinearColor::Red, DisplayTime);
}