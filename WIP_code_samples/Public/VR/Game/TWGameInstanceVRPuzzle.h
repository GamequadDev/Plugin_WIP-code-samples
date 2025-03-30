// Copyright (C) GAMEQUAD - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "VR/Game/TWGameControllerVRPuzzle.h"
#include "Game/TWGameInstance.h"
#include "VR/Game/TWGameInstanceVR.h"
#include "TWGameInstanceVRPuzzle.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EPuzzleLevelStatus
{
	None UMETA(DisplayName = "None"),
	Progress UMETA(DisplayName = "Progress"),
	Complete UMETA(DisplayName = "Complete"),
};

UCLASS()
class TWFRAMEWORK_API UTWGameInstanceVRPuzzle : public UTWGameInstanceVR
{
	GENERATED_BODY()


public:
	
	UFUNCTION()
	ATWGameControllerVRPuzzle* GetGameControllerVRPuzzle();
	
	UFUNCTION(Exec, Category="Commands")
	void TWFPuzzleVR_PutTogetherPuzzle();

	UFUNCTION(Exec, Category="Commands")
	void TWFPuzzleVR_PutRandomPuzzle();

	UFUNCTION(Exec, Category="Commands")
	void TWFPuzzleVR_HandsGrabByDistance();
	
	UFUNCTION(Exec, Category="Commands")
	void TWFPuzzleVR_ControlPanelSwitchUI(int index);
	
	virtual void Init() override;
	
	virtual void SaveGame(const FString& InSlotName, const int32 UserIndex = 0) override;
	virtual void SaveGameVRPuzzle(const int32 UserIndex = 0, const FName& Level = "", int NewMouldIndex = 0, int NewChildMouldIndex = 0, bool bCompleteLevel = false);

	UFUNCTION(BlueprintPure, Category="TWFPuzzleVR")
	virtual EPuzzleLevelStatus CheckLevelStatus(const FString& InSlotName, const int32 UserIndex = 0);
	
	virtual bool LoadGame(const FString& InSlotName, const int32 UserIndex = 0) override;
	
	bool LoadGameData(const FString& InSlotName,const int32 UserIndex = 0);
	
	FName SearchLevelPathFromTable(FName LevelName, UTWLevelsData* LevelsData);


	UFUNCTION()
	void ChangeSaveProperty(FName Level, int NewMouldIndex, int NewChildMouldIndex  = -1);

	UFUNCTION()
	int ReturnMouldIndex() const {return MouldIndex;};

	UFUNCTION()
	FName ReturnCurrentLevelName() const {return CurrentLevelName;};

	UFUNCTION()
	void ResetSavedProperty();

protected:

	UPROPERTY()
	ATWGameControllerVRPuzzle* GameControllerVRPuzzle;
	
	UPROPERTY()
	FName CurrentLevelName;
	
	UPROPERTY()
	int MouldIndex = 0;

	UPROPERTY()
	int ChildMouldIndex = -1;
	

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="TWFPuzzleVR")
	UTWLevelsData* AllGameLevels;
	
};
