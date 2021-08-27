// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "ServerRow.h"
#include "Components/Button.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBP(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBP.Class != nullptr)) { return; }
	ServerRowWidget = ServerRowBP.Class;
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	if (!ensure(Host != nullptr)) { return false; }
	Host->OnPressed.AddDynamic(this, &UMainMenu::HostGame);
	if (!ensure(Join != nullptr)) { return false; }
	Join->OnPressed.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	if (!ensure(Quit != nullptr)) { return false; }
	Quit->OnPressed.AddDynamic(this, &UMainMenu::QuitPressed);
	if (!ensure(ConfirmJoinMenu != nullptr)) { return false; }
	ConfirmJoinMenu->OnPressed.AddDynamic(this, &UMainMenu::JoinGame);
	if (!ensure(CancelJoinMenu != nullptr)) { return false; }
	CancelJoinMenu->OnPressed.AddDynamic(this, &UMainMenu::OpenMainMenu);

	return true;
}

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
	uint32 i = 0;
	for (auto ServerName : ServerNames)
	{
		if (!ensure(ServerRowWidget != nullptr)) { return; }
		UServerRow* ServerRow = CreateWidget<UServerRow>(this, ServerRowWidget);
		if (!ensure(ServerRow != nullptr)) { return; }
		ServerRow->Setup(this, i);
		ServerRow->ServerName->SetText(FText::FromString(ServerName));
		ServerList->AddChild(ServerRow);
		i++;
	}
}

void UMainMenu::SetSelectedIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateServers();
}


void UMainMenu::UpdateServers()
{
	for (int i = 0; i < ServerList->GetChildrenCount(); i++)
	{
		UServerRow* Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row)
		{
			Row->isSelected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}

void UMainMenu::JoinGame()
{
	if (SelectedIndex.IsSet() && MenuInterface != nullptr)
	{
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Server Index Not Set"));
	}
}

void UMainMenu::HostGame()
{
	if (MenuInterface == nullptr) { return; }
	UE_LOG(LogTemp, Warning, TEXT("Hosting"))
	MenuInterface->Host();
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) { return; }
	if (!ensure(JoinMenu != nullptr)) { return; }
	MenuSwitcher->SetActiveWidget(JoinMenu);
	if (MenuInterface != nullptr)
	{
		MenuInterface->RefreshServerList();
	}
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) { return; }
	if (!ensure(MainMenu != nullptr)) { return; }
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitPressed()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) { return; }
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }
	PlayerController->ConsoleCommand("quit");
}