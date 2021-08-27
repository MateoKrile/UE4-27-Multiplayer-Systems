// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "InGameMenu.generated.h"

UCLASS()
class MULTIPLAYER_COURSE_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()

	bool Initialize();
	UFUNCTION()
	void QuitPressed();
	UFUNCTION()
	void CancelPressed();

	UPROPERTY(meta = (BindWidget))
		class UButton* Quit;
	UPROPERTY(meta = (BindWidget))
		class UButton* Cancel;

};
