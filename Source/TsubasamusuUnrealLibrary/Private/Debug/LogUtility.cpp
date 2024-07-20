#include "Debug/LogUtility.h"

void ULogUtility::Log(const FString& Message)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
}

void ULogUtility::LogError(const FString& Message)
{
	UE_LOG(LogTemp, Error, TEXT("%s"), *Message);
}

void ULogUtility::LogWarning(const FString& Message)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
}