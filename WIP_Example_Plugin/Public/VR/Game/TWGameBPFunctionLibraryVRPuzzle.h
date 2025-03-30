// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VR/Game/TWGameInstanceVRPuzzle.h"
#include "VR/Game/TWGameBlueprintFunctionLibraryVR.h"
#include "TWGameBPFunctionLibraryVRPuzzle.generated.h"

/**
 * 
 */
UCLASS()
class TWFRAMEWORK_API UTWGameBPFunctionLibraryVRPuzzle : public UTWGameBlueprintFunctionLibraryVR
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "TWF|Game")
	static UTWGameInstanceVRPuzzle* GetGameInstanceVRPuzzle(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "TWF|Game")
	static void LoadGameVRPuzzle(const UObject* WorldContextObject, FString InSlot,int32 UserIndex = 0);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "TWF|Game")
	static void SaveGameVRPuzzle(const UObject* WorldContextObject, FString InSlot,int32 UserIndex = 0);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "TWF|Game")
	static void DeleteSaveVRPuzzle(const UObject* WorldContextObject, int32 UserIndex = 0);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "TWF|Game")
	static void ResetVRPuzzle(const UObject* WorldContextObject, int32 UserIndex = 0);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "TWF|Game")
	static ATWPuzzleVRBasicShape* CreatePuzzleShape(const UObject* WorldContextObject,TSubclassOf<ATWPuzzleVRBasicShape> PuzzleBasicShapeForPrefab,UStaticMesh* Mesh, FTransform SpawnTransform);
	
};
