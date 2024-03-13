// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLiftLoginUserWidget.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Kismet/GameplayStatics.h"


UGameLiftLoginUserWidget::UGameLiftLoginUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	Http = &FHttpModule::Get();

	//TODO update with your API Gateway endpoint
	ApiGatewayEndpoint = FString::Printf(TEXT("https://x3oc3nzf94.execute-api.us-east-1.amazonaws.com/test/login"));
	LoginURI = FString::Printf(TEXT("/login"));
}

void UGameLiftLoginUserWidget::OnLoginClicked() {
	LoginRequest(user, pass);
}

void UGameLiftLoginUserWidget::LoginRequest(FString usr, FString pwd) {
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField(TEXT("username"), *FString::Printf(TEXT("%s"), *usr));
	JsonObject->SetStringField(TEXT("password"), *FString::Printf(TEXT("%s"), *pwd));

	FString JsonBody;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonBody);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> LoginHttpRequest = Http->CreateRequest();

	LoginHttpRequest->SetVerb("POST");
	LoginHttpRequest->SetURL(ApiGatewayEndpoint + LoginURI);
	LoginHttpRequest->SetHeader("Content-Type", "application/json");
	LoginHttpRequest->SetContentAsString(JsonBody);
	LoginHttpRequest->OnProcessRequestComplete().BindUObject(this, &UGameLiftLoginUserWidget::OnLoginResponse);
	LoginHttpRequest->ProcessRequest();
}

void UGameLiftLoginUserWidget::OnLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
	if (bWasSuccessful) {
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject)) {

			FString Message = JsonObject->GetStringField("msg");

			if(GEngine)  GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Message);

		}
	}

}

void UGameLiftLoginUserWidget::StartSessionRequest(FString idt) {
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> StartSessionHttpRequest = Http->CreateRequest();

	StartSessionHttpRequest->SetVerb("GET");
	StartSessionHttpRequest->SetURL(ApiGatewayEndpoint + StartSessionURI);
	StartSessionHttpRequest->SetHeader("Content-Type", "application/json");
	StartSessionHttpRequest->SetHeader("Authorization", idt);
	StartSessionHttpRequest->OnProcessRequestComplete().BindUObject(this, &UGameLiftLoginUserWidget::OnStartSessionResponse);
	StartSessionHttpRequest->ProcessRequest();
}

void UGameLiftLoginUserWidget::OnStartSessionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
	if (bWasSuccessful) {
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
			FString IpAddress = JsonObject->GetObjectField("PlayerSession")->GetStringField("IpAddress");
			FString Port = JsonObject->GetObjectField("PlayerSession")->GetStringField("Port");

			FString LevelName = IpAddress + ":" + Port;

			UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName), false);
		}

	}
}
