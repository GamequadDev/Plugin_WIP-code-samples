// Copyright (C) GAMEQUAD - All Rights Reserved


#pragma once

#include "CoreMinimal.h"
#include "VR/Puzzle/TWPuzzleVRBasicShape.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "TWProceduralSpawnPuzzleShape.generated.h"

UCLASS()
class TWFRAMEWORK_API ATWProceduralSpawnPuzzleShape : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATWProceduralSpawnPuzzleShape(const class FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TWF Puzzle VR")
	USceneComponent* SceneRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TWF Puzzle VR")
	TArray<ATWPuzzleVRBasicShape*> PuzzleShapesToSpawnArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TWF Puzzle VR")
	TArray<ATWPuzzleVRBasicShape*> PuzzleShapesSpawned;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="TWF Puzzle VR")
	bool Debug = false;

	TArray<FVector> SpawnPuzzlesLoc;
	FTimerHandle PuzzleSpawnTimerHandle;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="TWF Puzzle VR")
	float SpawnTime = 0.2;

	virtual ATWPuzzleVRBasicShape* SpawnPuzzle(FVector Location,FRotator Rotator,ATWPuzzleVRBasicShape* ActorTemplate);

	UFUNCTION()
	virtual void SpawnPuzzleDelay(TArray<FVector> Locations, FRotator Rotator, TArray<ATWPuzzleVRBasicShape*> ActorsTemplate);

	int indexSpawnDelay = 0;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void AddPuzzleShapeToSpawn(ATWPuzzleVRBasicShape* PuzzleShape) {PuzzleShapesToSpawnArray.Add(PuzzleShape);};

	UFUNCTION()
	virtual void ClearPuzzleShapeToSpawn() {PuzzleShapesToSpawnArray.Empty();};
	
	UFUNCTION()
	virtual int GetNumPuzzleShapesToSpawn() {return PuzzleShapesToSpawnArray.Num();};

	UFUNCTION()
	virtual void SpawnPuzzleShapes(); 
};
