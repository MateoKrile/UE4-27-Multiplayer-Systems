// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "ServerRow.generated.h"


UCLASS()
class MULTIPLAYER_COURSE_API UServerRow : public UUserWidget
{
	GENERATED_BODY()
public:

	void Setup(class UMainMenu* InParent, uint32 InIndex);

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ServerName;
	UPROPERTY(meta = (BindWidget))
		class UButton* Select;
	UPROPERTY(BlueprintReadOnly)
		bool isSelected = false;
	UFUNCTION()
		void OnSelected();
private:

	class UMainMenu* Parent;
	uint32 Index;
};
