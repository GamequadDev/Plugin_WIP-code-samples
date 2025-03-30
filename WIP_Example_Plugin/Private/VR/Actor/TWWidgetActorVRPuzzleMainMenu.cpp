// Fill out your copyright notice in the Description page of Project Settings.


#include "VR/Actor/TWWidgetActorVRPuzzleMainMenu.h"


ATWWidgetActorVRPuzzleMainMenu::ATWWidgetActorVRPuzzleMainMenu(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	LeftWidgetComp = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("LeftWidgetComp"));
	LeftWidgetComp->SetupAttachment(SceneRootComp);

	RightWidgetComp = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("RightWidgetComp"));
	RightWidgetComp->SetupAttachment(SceneRootComp);
	
}

void ATWWidgetActorVRPuzzleMainMenu::BeginPlay()
{
	Super::BeginPlay();
	
}


void ATWWidgetActorVRPuzzleMainMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
