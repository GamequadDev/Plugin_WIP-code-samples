// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VR/Procedural/TWProceduralSpawnPuzzleShape.h"
#include "TWSplineSpawnPuzzleShape.generated.h"

/**
 * 
 */
UCLASS()
class TWFRAMEWORK_API ATWSplineSpawnPuzzleShape : public ATWProceduralSpawnPuzzleShape
{
	GENERATED_BODY()

public:
	ATWSplineSpawnPuzzleShape(const class FObjectInitializer& ObjectInitializer);

	
	
protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TWF Puzzle VR")
	USplineComponent* SplineComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TWF Puzzle VR")
	float SpawnObjectLenghtAlongSpline = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TWF Puzzle VR")
	FVector Shift = {0,0,-40};
	
	float CurrentLenght = 0;
	FVector CurrentShift = FVector::Zero();
	//void SpawnPuzzleShape();

	//UFUNCTION()
	//virtual void SpawnPuzzleDelay(TArray<FVector> Locations, FRotator Rotator, TArray<ATWPuzzleVRBasicShape*> ActorsTemplate);
	
	public:
	
	virtual void SpawnPuzzleShapes() override;
	
};