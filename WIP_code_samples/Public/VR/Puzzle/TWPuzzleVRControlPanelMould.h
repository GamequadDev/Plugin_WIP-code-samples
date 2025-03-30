// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VR/UI/ControlPanel/TWUIWidgetControlPanelVRPuzzle.h"
#include "VR/UI/Components/WidgetComponents/TWWidgetCompVRPuzzleControlPanel.h"
#include "VR/Puzzle/TWPuzzleVRBasicMould.h"
#include "TWPuzzleVRControlPanelMould.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateWidgetNextMouldSignature, int , Index);

/**
 * 
 */
UCLASS()
class TWFRAMEWORK_API ATWPuzzleVRControlPanelMould : public ATWPuzzleVRBasicMould
{
	GENERATED_BODY()


public:
	
	ATWPuzzleVRControlPanelMould(const class FObjectInitializer& ObjectInitializer);


	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintAssignable)
	FDelegateWidgetNextMouldSignature WidgetNextMouldDelegate;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="VR")
	void NextMould(int PuzzleIndex);
	virtual void NextMould_Implementation(int PuzzleIndex){if(WidgetNextMouldDelegate.IsBound()){WidgetNextMouldDelegate.Broadcast(PuzzleIndex); CurrentSpawnedIndexPuzzleMould = PuzzleIndex;}};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="VR")
	void SwitchUMG(int UMGIndex);
	virtual void SwitchUMG_Implementation(int UMGIndex){};
	

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="VR")
	void CompleteAllPuzzle();
	virtual void CompleteAllPuzzle_Implementation(){};

	UFUNCTION()
	void SetPuzzlesInformationTable_Handle(UDataTable* Table, int index);
	
	UFUNCTION()
	void SetNextMapPath_Handle(const FName MapPath);


	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF|Widget", meta=(AllowPrivateAccess = true))
	UTWWidgetCompVRPuzzleControlPanel* ControlPanelWidgetComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF|Widget", meta=(AllowPrivateAccess = true))
	USceneComponent* RootScene = nullptr; 

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="VR")
	UDataTable* PuzzlesInformationTable;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="VR")
	int CurrentSpawnedIndexPuzzleMould = 0;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="VR")
	UTWUIWidgetControlPanelVRPuzzle* ControlPanelWidget;
	
};
