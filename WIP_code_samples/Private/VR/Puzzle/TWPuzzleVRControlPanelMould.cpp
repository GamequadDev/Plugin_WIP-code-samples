// Fill out your copyright notice in the Description page of Project Settings.


#include "VR/Puzzle/TWPuzzleVRControlPanelMould.h"
#include "VR/Game/TWGameInstanceVRPuzzle.h"


ATWPuzzleVRControlPanelMould::ATWPuzzleVRControlPanelMould(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

	RootScene = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("RootSceneComponent"));
	SetRootComponent(RootScene);
	
	ControlPanelWidgetComp = ObjectInitializer.CreateDefaultSubobject<UTWWidgetCompVRPuzzleControlPanel>(this, TEXT("WidgetCompVRPuzzleControlPanel"));
	ControlPanelWidgetComp->SetupAttachment(RootScene);
	
}

void ATWPuzzleVRControlPanelMould::SetPuzzlesInformationTable_Handle(UDataTable* Table, int index)
{
	PuzzlesInformationTable = Table;
	CurrentSpawnedIndexPuzzleMould = index;
}

void ATWPuzzleVRControlPanelMould::BeginPlay()
{

	Super::BeginPlay();
	
	if(ControlPanelWidgetComp)
	{
		ControlPanelWidget = Cast<UTWUIWidgetControlPanelVRPuzzle>(ControlPanelWidgetComp->GetWidget());
	}

	if(ControlPanelWidget)
	{
		WidgetNextMouldDelegate.AddDynamic(ControlPanelWidget, &UTWUIWidgetControlPanelVRPuzzle::UTWUIWidgetControlPanelVRPuzzle::WidgetNextMould);
		ControlPanelWidget->SetInformationDataTable(PuzzlesInformationTable, CurrentSpawnedIndexPuzzleMould);
	}
	
}

void ATWPuzzleVRControlPanelMould::SetNextMapPath_Handle(const FName MapPath)
{
	if(ControlPanelWidget)
	{
		ControlPanelWidget->SetNextMapPath(MapPath);
	}
}





