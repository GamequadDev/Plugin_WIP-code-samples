// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VR/Puzzle/TWPuzzleVR.h"
//#include "GameFramework/Actor.h"
#include "TWPuzzleVRBasicPrefab.generated.h"

UCLASS()
class TWFRAMEWORK_API ATWPuzzleVRBasicPrefab : public ATWPuzzleVR
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATWPuzzleVRBasicPrefab();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<UStaticMeshComponent*> MeshComponents;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual TArray<UStaticMeshComponent*> GetMeshComps();
	
};
