// Copyright (C) GAMEQUAD - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "VR/Puzzle/TWPuzzleVRBasicMould.h"
#include "VR/Puzzle/TWPuzzleVRBasicShape.h"
#include "VR/Puzzle/TWPuzzleVRControlPanelMould.h"
#include "VR/Procedural/TWProceduralSpawnPuzzleShape.h"
#include "VR/Game/TWGameControllerVR.h"
#include "TWGameControllerVRPuzzle.generated.h"

USTRUCT(BlueprintType)
struct FPuzzleInformation : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TW VR")
	FName MouldName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TW VR")
	FText MainText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TW VR")
	TArray<FText> FunFacts;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TW VR")
	FText Text;
	
};
UCLASS()
class TWFRAMEWORK_API ATWGameControllerVRPuzzle : public ATWGameControllerVR
{
	GENERATED_BODY()


protected:

	virtual void BeginPlay() override;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TWF Puzzle VR")
	TArray<TSubclassOf<ATWPuzzleVRBasicMould>> MainMouldToSpawnArray;
	
	UPROPERTY()
	TArray<ATWPuzzleVRBasicShape*> PuzzleShapesToSpawnArray;

	UPROPERTY()
	TArray<ATWPuzzleVRBasicMould*> PuzzleMouldToSpawnArray;
	
	UPROPERTY()
	TArray<ATWProceduralSpawnPuzzleShape*> ProceduralSpawnPuzzleShapesArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="TWF Puzzle VR")
	TArray<ATWPuzzleVRBasicMould*> SpawnedMainMouldArray;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="TWF Puzzle VR")
	//ATWPuzzleVRBasicMould* CurrentMould = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="TWF Puzzle VR")
	TArray<ATWPuzzleVRBasicMould*> PuzzleMouldChildArray;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TWF Puzzle VR")
	float DistanceFromPawnVRToMould = 50;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TWF Puzzle VR")
	FVector ShiftFromPawnVRToMould = {50,0,-10};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TWF Puzzle VR")
	float ShiftFromPawnToMouldForward = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="TWF Puzzle VR")
	FVector MouldSpawnLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="TWF Puzzle VR")
	FRotator MouldSpawnRotator;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="TWF Puzzle VR")
	//int CurrentSpawnIndexPuzzleMould = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="TWF Puzzle VR")
	int CurrentSpawnedIndexPuzzleMould = 0;

	
	UPROPERTY()
    TArray<ATWPuzzleVRControlPanelMould*> ControlPanels;
	
	UFUNCTION()
	TArray<ATWProceduralSpawnPuzzleShape*> GetProceduralSpawnObjectsFromLevel();
	
	UFUNCTION()
	TArray<ATWPuzzleVRBasicShape*> GetPuzzleShapesFromMould(ATWPuzzleVRBasicMould* Mould);

	UFUNCTION()
	TArray<ATWPuzzleVRBasicMould*> GetPuzzleMouldOffMould(ATWPuzzleVRBasicMould* Mould);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TWF Puzzle VR")
	UDataTable* PuzzleMouldsData;

	/** 
	* Get All ControlPanels from current Level
	*
	* @return                       ControlPanels
	*/
	
	UFUNCTION()
	TArray<ATWPuzzleVRControlPanelMould*> GetControlPanelsFromLevel();
	
	//-----------------------
	//	Puzzle Functions
	//----------------------
	
	void SetPuzzleStatusForMainMould(ATWPuzzleVRBasicMould* MainMould);
	void AddPuzzleShapeToSpawnArray(TArray<ATWPuzzleVRBasicShape*> PuzzleShapes,TArray<ATWProceduralSpawnPuzzleShape*> ProceduralSpawnArray);
	void SpawnPuzzleShapes(TArray<ATWProceduralSpawnPuzzleShape*> ProceduralSpawnArray );
	void ClearPuzzleShapeToSpawnArray(TArray<ATWProceduralSpawnPuzzleShape*> ProceduralSpawnArray);
	void SetupChildMould();
	
	void SpawnMainMould(TSubclassOf<ATWPuzzleVRBasicMould> MouldToSpawn,FVector Location, FRotator Rotator,FActorSpawnParameters& ActorSpawnParameters);
	
	public:
	
	UFUNCTION()
	void CompleteStage(ATWPuzzleVRBasicMould* MainMould );

	UFUNCTION()
	void CompleteAllPuzzles();
	
	UFUNCTION()
	void PutTogetherPuzzle();

	UFUNCTION()
	void PutRandomPuzzle();
	
	UFUNCTION()
	void SwitchUMGControlPanel(int index);
	
	UFUNCTION()
	int GetCurrentSpawnedIndexPuzzleMould() const {return CurrentSpawnedIndexPuzzleMould;}
	
	protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TWF Puzzle VR")
	FName NextMapPath;
};
