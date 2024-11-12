#include "GoogleCloud/AsyncActionGetGoogleCloudJwt.h"

UAsyncActionGetGoogleCloudJwt* UAsyncActionGetGoogleCloudJwt::AsyncGetGoogleCloudJwt(UObject* WorldContextObject)
{
	UAsyncActionGetGoogleCloudJwt* Action = NewObject<UAsyncActionGetGoogleCloudJwt>();

	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}

FString UAsyncActionGetGoogleCloudJwt::GetGoogleCloudJwtHeader()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

	JsonObject->SetStringField(TEXT("alg"), TEXT("RS256"));
	JsonObject->SetStringField(TEXT("typ"), TEXT("JWT"));

	FString JsonString;

	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);

	if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter)) return JsonString;

	OnFailed(TEXT("create a JSON header"));

	return TEXT("");
}

FString UAsyncActionGetGoogleCloudJwt::GetGoogleCloudJwtPayload(const FString& ServiceAccountMailAddress, const TArray<FString>& Scopes)
{
	IssuedUnixTime = FDateTime::UtcNow().ToUnixTimestamp();

	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

	JsonObject->SetStringField(TEXT("iss"), ServiceAccountMailAddress);
	JsonObject->SetStringField(TEXT("scope"), FString::Join(Scopes, TEXT(" ")));
	JsonObject->SetStringField(TEXT("aud"), TEXT("https://oauth2.googleapis.com/token"));
	JsonObject->SetNumberField(TEXT("exp"), IssuedUnixTime + 3600);
	JsonObject->SetNumberField(TEXT("iat"), IssuedUnixTime);

	FString JsonString;

	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);

	if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter)) return JsonString;

	OnFailed(TEXT("create a JSON payload"));

	return TEXT("");
}

void UAsyncActionGetGoogleCloudJwt::Activate()
{

}

void UAsyncActionGetGoogleCloudJwt::OnCompleted(const FString& Message)
{
	Completed.Broadcast(Message, IssuedUnixTime, true);

	SetReadyToDestroy();
}

void UAsyncActionGetGoogleCloudJwt::OnFailed(const FString& TriedThing, const FString& Message)
{
	UTsubasamusuLogLibrary::LogError(TEXT("Failed to ") + TriedThing + TEXT(" to get a JSON Web Token for Google Cloud."));

	Completed.Broadcast(Message, -1, false);

	SetReadyToDestroy();
}