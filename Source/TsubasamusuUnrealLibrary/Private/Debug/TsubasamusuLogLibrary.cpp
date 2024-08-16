#include "Debug/TsubasamusuLogLibrary.h"

void UTsubasamusuLogLibrary::Log(const FString& Message)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
}

void UTsubasamusuLogLibrary::LogError(const FString& Message)
{
	UE_LOG(LogTemp, Error, TEXT("%s"), *Message);
}

void UTsubasamusuLogLibrary::LogWarning(const FString& Message)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
}