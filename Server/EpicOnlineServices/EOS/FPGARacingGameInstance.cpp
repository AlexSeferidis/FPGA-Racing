// Fill out your copyright notice in the Description page of Project Settings.


#include "FPGARacingGameInstance.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"

void UFPGARacingGameInstance::LogIn()
{
	if (identityPtr)
	{
		FOnlineAccountCredentials onlineAccountCredentials;
		onlineAccountCredentials.Type = "accountportal";
		onlineAccountCredentials.Id = "";
		onlineAccountCredentials.Token = "";

		identityPtr->OnLoginCompleteDelegates->AddUObject(this, &UFPGARacingGameInstance::LoginCompleted);
		identityPtr->Login(0 , onlineAccountCredentials);

	}
}

void UFPGARacingGameInstance::CreateSession()
{
	if (onlineSubsystem)
	{
		if (sessionPtr = onlineSubsystem->GetSessionInterface())
		{
			FOnlineSessionSettings sessionSettings;
			sessionSettings.bAllowInvites = true;
			sessionSettings.bIsDedicated = false;
			sessionSettings.bIsLANMatch = false;
			sessionSettings.bShouldAdvertise = true;
			sessionSettings.bUseLobbiesIfAvailable = true;
			sessionSettings.bUsesPresence = true;
			sessionSettings.bAllowJoinInProgress = true;
			sessionSettings.bAllowJoinViaPresence = true;
			sessionSettings.NumPublicConnections = true;

			sessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UFPGARacingGameInstance::CreateSessionCompleted);
			sessionSettings.Set(FName("LobbyName"), FString("PreMatchLobby"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
			sessionPtr->CreateSession(0, FName("PreMatchLobby"), sessionSettings);
		}
	}
}

void UFPGARacingGameInstance::Init()
{
	Super::Init();

	onlineSubsystem = IOnlineSubsystem::Get();
	identityPtr = onlineSubsystem->GetIdentityInterface();

}

void UFPGARacingGameInstance::LoginCompleted(int numOfPlayers, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Logged in Successful"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Logged in Failed: %s"), *Error);

	}

}

void UFPGARacingGameInstance::CreateSessionCompleted(FName Name, bool bWasSuccessful)
{

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning ,TEXT("session created successfully"))
	}
	else
	{
		UE_LOG(LogTemp, Warning ,TEXT("session failed"))
	}

	if (!GameLevel.IsValid())
	{
		GameLevel.LoadSynchronous();

	}
	if (GameLevel.IsValid())
	{
		const FName LevelName = FName(*FPackageName::ObjectPathToPackageName(GameLevel.ToString()));
		GetWorld()->ServerTravel(LevelName.ToString() + "?Listen");

	}

	

}
