// Fill out your copyright notice in the Description page of Project Settings.


#include "VR/Procedural/TWProceduralSpawnPuzzleShape.h"
#include "TimerManager.h"

// Sets default values
ATWProceduralSpawnPuzzleShape::ATWProceduralSpawnPuzzleShape(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneRoot = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("SceneRoot"));
	SceneRoot->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ATWProceduralSpawnPuzzleShape::BeginPlay()
{
	
	
}

// Called every frame
void ATWProceduralSpawnPuzzleShape::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ATWProceduralSpawnPuzzleShape::SpawnPuzzleShapes()
{
	
}

void ATWProceduralSpawnPuzzleShape::SpawnPuzzleDelay(TArray<FVector> Locations, FRotator Rotator, TArray<ATWPuzzleVRBasicShape*> ActorsTemplate)
{
	if(indexSpawnDelay == ActorsTemplate.Num())
	{
		GetWorldTimerManager().ClearTimer(PuzzleSpawnTimerHandle);
		indexSpawnDelay = 0;
	}
	else
	{
		SpawnPuzzle(Locations[indexSpawnDelay],Rotator,ActorsTemplate[indexSpawnDelay]);
		indexSpawnDelay++;
	}
}


ATWPuzzleVRBasicShape* ATWProceduralSpawnPuzzleShape::SpawnPuzzle(FVector Location,FRotator Rotator,ATWPuzzleVRBasicShape* ActorTemplate)
{
	ATWPuzzleVRBasicShape* SpawnedActor = GetWorld()->SpawnActorDeferred<ATWPuzzleVRBasicShape>(ActorTemplate->GetClass(), FTransform::Identity,nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	SpawnedActor->SetBasicMaterial(ActorTemplate->GetBasicMaterial());
	if(SpawnedActor->BasicStaticMesh->GetStaticMesh() != ActorTemplate->BasicStaticMesh->GetStaticMesh())
	{
		SpawnedActor->BasicStaticMesh->SetStaticMesh(ActorTemplate->BasicStaticMesh->GetStaticMesh());
	}
	FTransform TransformSpawn;
		
	TransformSpawn.SetLocation(Location);
	TransformSpawn.SetRotation(Rotator.Quaternion());
	TransformSpawn.SetScale3D({1,1,1});
		
		
	SpawnedActor->FinishSpawning(TransformSpawn);

	//SpawnedActor->SetActorScale3D(ActorTemplate->GetActorScale3D());
	SpawnedActor->SetActorScale3D(ActorTemplate->DefaultScaleObj);


	
	SpawnedActor->PostSpawnSetup();
	PuzzleShapesSpawned.Add(SpawnedActor);
	
	return SpawnedActor; 
}
