// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VR/UI/TWUIWidgetVRPuzzle.h"
#include "Engine/DataTable.h"
#include "TWUIWidgetControlPanelVRPuzzle.generated.h"



/**
 * 
 */
UCLASS()
class TWFRAMEWORK_API UTWUIWidgetControlPanelVRPuzzle : public UTWUIWidgetVRPuzzle
{
	GENERATED_BODY()


public:


	//UTWUIWidgetControlPanelVRPuzzle();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="VR Widget")
	void WidgetNextMould(int PuzzleIndex);
	virtual void WidgetNextMould_Implementation(int PuzzleIndex){};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="VR Widget")
	void SetInformationDataTable(UDataTable* Table, int index);
	virtual void SetInformationDataTable_Implementation(UDataTable* Table, int index){InformationPuzzleTable = Table;};

	UFUNCTION()
	void SetNextMapPath(const FName MapPath){NextMapPath = MapPath;};

	
	
protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="VR")
	UDataTable* InformationPuzzleTable;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="VR")
	FName NextMapPath;
	
	
};