// Fill out your copyright notice in the Description page of Project Settings.


#include "MPGameInstance.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"

const static FName SESSION_NAME = TEXT("My Session");

UMPGameInstance::UMPGameInstance( const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBP(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBP(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(MainMenuBP.Class != nullptr)) { return; }
	MainMenuWidget = MainMenuBP.Class;
	if (!ensure(InGameMenuBP.Class != nullptr)) { return; }
	InGameMenuWidget = InGameMenuBP.Class;
}

void UMPGameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr)
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface != nullptr)
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMPGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMPGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMPGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMPGameInstance::OnJoinSessionComplete);
		}
	}
}

void UMPGameInstance::Host()
{
	UE_LOG(LogTemp, Warning, TEXT("Hosting"))
	if (SessionInterface == nullptr) { return; }
	auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
	if (ExistingSession == nullptr)
	{
		CreateSession();
	}
	else
	{
		SessionInterface->DestroySession(SESSION_NAME);
	}
}

void UMPGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch);
	if (SessionSearch.IsValid())
	{
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UMPGameInstance::Join(uint32 Index)
{
	if (SessionInterface == nullptr || SessionSearch == nullptr) { return; }

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UMPGameInstance::LoadMainMenu()
{
	if (!ensure(MainMenuWidget != nullptr)) { return; }
	MainMenu = CreateWidget<UMainMenu>(this, MainMenuWidget);
	if (!ensure(MainMenu != nullptr)) { return; }
	MainMenu->Setup();
	MainMenu->SetMenuInterface(this);
}

void UMPGameInstance::LoadInGameMenu()
{
	if (!ensure(InGameMenuWidget != nullptr)) { return; }
	InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuWidget);

	if (!ensure(InGameMenu != nullptr)) { return; }
	InGameMenu->Setup();
	InGameMenu->SetMenuInterface(this);
}

void UMPGameInstance::CreateSession()
{
	if (SessionInterface == nullptr) { return; }
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bUsesPresence = true;
	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

void UMPGameInstance::GoToMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenuLevel", ETravelType::TRAVEL_Absolute);
}

void UMPGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (MainMenu != nullptr)
	{
		MainMenu->Teardown();
	}
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) { return; }
	World->ServerTravel("/Game/Maps/GameMap?listen");
}

void UMPGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}

void UMPGameInstance::OnFindSessionsComplete(bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("Found All Session"))
	if (Success && SessionSearch && MainMenu != nullptr)
	{	
		TArray<FString> ServerNames;
		ServerNames.Add("Test Server 1");
		ServerNames.Add("Test Server 2");
		ServerNames.Add("Test Server 3");
		for (auto SearchResult : SessionSearch->SearchResults)
		{
			ServerNames.Add(SearchResult.GetSessionIdStr());
		}
		MainMenu->SetServerList(ServerNames);
	}
}

void UMPGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) { return; }
	FString ConnectInfo;
	if (!SessionInterface->GetResolvedConnectString(SESSION_NAME, ConnectInfo)) { return; }
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }
	PlayerController->ClientTravel(ConnectInfo, ETravelType::TRAVEL_Absolute);
}
