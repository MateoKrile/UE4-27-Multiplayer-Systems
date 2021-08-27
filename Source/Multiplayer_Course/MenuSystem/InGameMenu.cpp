// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"

#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }
	if (!ensure(Quit != nullptr)) { return false; }
	Quit->OnPressed.AddDynamic(this, &UInGameMenu::QuitPressed);
	if (!ensure(Cancel != nullptr)) { return false; }
	Cancel->OnPressed.AddDynamic(this, &UInGameMenu::CancelPressed);

	return true;
}

void UInGameMenu::QuitPressed()
{
	if (MenuInterface != nullptr)
	{
		Teardown();
		MenuInterface->GoToMainMenu();
	}
}

void UInGameMenu::CancelPressed()
{
	Teardown();
}