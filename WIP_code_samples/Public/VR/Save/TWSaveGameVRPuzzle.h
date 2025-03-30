// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VR/Save/TWSaveGameVR.h"
#include "TWSaveGameVRPuzzle.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType) struct FSaveGameLevelPuzzleData
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Version = "0.5";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LevelName;
	
	//UPROPERTY()
	//TMap<int,int> MouldIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MouldIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ChildMouldIndex = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFinish;
	

};

USTRUCT(BlueprintType) struct FMeshesForPuzzleDataEditor  : public FTableRowBase 
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Scale = FVector(0.1,0.1,0.1);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Category = "None";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* MeshImage;
	
};


UCLASS()
class TWFRAMEWORK_API UTWSaveGameVRPuzzle : public UTWSaveGameVR
{
	GENERATED_BODY()


public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UTWSaveGame)
	TMap<FName, FSaveGameLevelPuzzleData> ProgressAllLevels;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UTWSaveGame)
	FSaveGameLevelPuzzleData LastLevelData;
	
};
