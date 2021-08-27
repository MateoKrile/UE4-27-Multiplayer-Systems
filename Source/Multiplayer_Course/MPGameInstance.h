// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "MenuSystem/MenuInterface.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "MPGameInstance.generated.h"

UCLASS()
class MULTIPLAYER_COURSE_API UMPGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()


public:
	UMPGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init();

	UFUNCTION(Exec)
	virtual void Host() override;
	UFUNCTION(Exec)
	virtual void Join(uint32 Index) override;
	UFUNCTION(BlueprintCallable)
	virtual void LoadMainMenu() override;
	UFUNCTION(BlueprintCallable)
	virtual void LoadInGameMenu() override;

	virtual void GoToMainMenu() override;

	virtual void RefreshServerList() override;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

private:
	TSubclassOf<class UUserWidget> MainMenuWidget;
	TSubclassOf<class UUserWidget> InGameMenuWidget;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	IOnlineSessionPtr SessionInterface;
	class UInGameMenu* InGameMenu;
	class UMainMenu* MainMenu;

	void CreateSession();
};
