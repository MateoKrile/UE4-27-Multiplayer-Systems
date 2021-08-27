// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

UCLASS()
class MULTIPLAYER_COURSE_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FString> ServerNames);

	void SetSelectedIndex(uint32 Index);

protected:
	virtual bool Initialize();

private:
	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* MenuSwitcher;
	//Join Menu
	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;
	UPROPERTY(meta = (BindWidget))
		class UButton* CancelJoinMenu;
	UPROPERTY(meta = (BindWidget))
		class UButton* ConfirmJoinMenu;

	//Main Menu
	UPROPERTY(meta = (BindWidget))
		class UWidget* MainMenu;
	UPROPERTY(meta = (BindWidget))
		class UButton* Join;
	UPROPERTY(meta = (BindWidget))
		class UButton* Host;
	UPROPERTY(meta = (BindWidget))
		class UButton* Quit;

	//Server List on the Join Menu
	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* ServerList;
	TSubclassOf<UUserWidget> ServerRowWidget;

	//Button Press Functions
	UFUNCTION()
		void JoinGame();
	UFUNCTION()
		void HostGame();
	UFUNCTION()
		void OpenJoinMenu();
	UFUNCTION()
		void OpenMainMenu();
	UFUNCTION()
		void QuitPressed();

	void UpdateServers();

	TOptional<uint32> SelectedIndex;
};
