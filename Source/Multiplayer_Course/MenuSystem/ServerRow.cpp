// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"
#include "Components/Button.h"
#include "MainMenu.h"

void UServerRow::Setup(UMainMenu* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
	Select->OnClicked.AddDynamic(this, &UServerRow::OnSelected);
}

void UServerRow::OnSelected()
{
	if (!ensure(Parent != nullptr)) { return; }
	Parent->SetSelectedIndex(Index);
}
