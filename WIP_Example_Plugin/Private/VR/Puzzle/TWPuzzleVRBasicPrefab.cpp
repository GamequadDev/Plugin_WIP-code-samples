// Fill out your copyright notice in the Description page of Project Settings.


#include "VR/Puzzle/TWPuzzleVRBasicPrefab.h"

// Sets default values
ATWPuzzleVRBasicPrefab::ATWPuzzleVRBasicPrefab()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATWPuzzleVRBasicPrefab::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATWPuzzleVRBasicPrefab::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<UStaticMeshComponent*> ATWPuzzleVRBasicPrefab::GetMeshComps()
{
	if(MeshComponents.Num() >1)
	{
		return MeshComponents;
	}

	TArray<USceneComponent*> ChildrenSceneComp;
	RootComponent->GetChildrenComponents(false,ChildrenSceneComp);

	for(USceneComponent* LocalComp : ChildrenSceneComp)
	{
		if(UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(LocalComp))
		{
			MeshComponents.Add(MeshComp);
		}
	}
	return MeshComponents;
}

