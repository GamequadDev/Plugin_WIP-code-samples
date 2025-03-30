// Fill out your copyright notice in the Description page of Project Settings.


#include "VR/Procedural/TWSplineSpawnPuzzleShape.h"

ATWSplineSpawnPuzzleShape::ATWSplineSpawnPuzzleShape(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
	SplineComp = ObjectInitializer.CreateDefaultSubobject<USplineComponent>(this, TEXT("SplineComp"));
	SplineComp->SetupAttachment(SceneRoot);
}

void ATWSplineSpawnPuzzleShape::BeginPlay()
{
	Super::BeginPlay();

	
}

void ATWSplineSpawnPuzzleShape::SpawnPuzzleShapes()
{
	//Super::SpawnPuzzleShapes();
	int Index = 0;
	
	FVector SpawnPuzzleShapeLoc;
	FRotator SpawnPuzzleShapeRot = GetActorRotation();
	FActorSpawnParameters SpawnParameters;


	float MaxZBoundZ = 0;
	for(ATWPuzzleVRBasicShape* Actor : PuzzleShapesToSpawnArray)
	{
		if(Actor)
		{
			const float ZHeightBoundObj = Actor->BasicStaticMesh->GetStaticMesh()->GetExtendedBounds().BoxExtent.Z * Actor->DefaultScaleObj.Z;
			if(ZHeightBoundObj > MaxZBoundZ)
			{
				MaxZBoundZ = ZHeightBoundObj;
			}
		}
	}

	float LastSphereRadiusObj = 0;
	CurrentLenght = 0;
	CurrentShift = FVector::Zero();
	SpawnPuzzlesLoc.Empty();
	
	for(ATWPuzzleVRBasicShape* Actor : PuzzleShapesToSpawnArray)
	{
		if(SplineComp)
		{
			if(Index == 0)
			{
				SpawnPuzzleShapeLoc = SplineComp->GetLocationAtDistanceAlongSpline(CurrentLenght ,ESplineCoordinateSpace::World);
				float SphereRadiusObj = Actor->BasicStaticMesh->GetStaticMesh()->GetExtendedBounds().SphereRadius * Actor->DefaultScaleObj.GetMax();
				LastSphereRadiusObj = SphereRadiusObj;
				
				if(Debug)
				{
					DrawDebugSphere(GetWorld(), SpawnPuzzleShapeLoc,SphereRadiusObj, 32, FColor::Red,false, 5.0,0,1);
				}
			}
			else
			{
				float SphereRadiusObj = Actor->BasicStaticMesh->GetStaticMesh()->GetExtendedBounds().SphereRadius * Actor->DefaultScaleObj.GetMax();
				float Difference = SpawnObjectLenghtAlongSpline - SphereRadiusObj < 0 ? FMath::Abs(SpawnObjectLenghtAlongSpline - SphereRadiusObj) + SpawnObjectLenghtAlongSpline: SpawnObjectLenghtAlongSpline;
				
				CurrentLenght = CurrentLenght + Difference + LastSphereRadiusObj;
				LastSphereRadiusObj = SphereRadiusObj;

				
				//CurrentLenght += SpawnObjectLenghtAlongSpline;
				SpawnPuzzleShapeLoc = SplineComp->GetLocationAtDistanceAlongSpline(CurrentLenght ,ESplineCoordinateSpace::World);
				
				FVector LastPointAlongSplineLocation;
				FVector LastPointAlongSplineTangent;
				
				SplineComp->GetLocationAndTangentAtSplinePoint(SplineComp->GetNumberOfSplinePoints() - 1, LastPointAlongSplineLocation, LastPointAlongSplineTangent, ESplineCoordinateSpace::World);

				if(LastPointAlongSplineLocation.Equals(SpawnPuzzleShapeLoc, 2))
				{
					CurrentLenght = 0;
					CurrentShift += Shift + FVector(0,0,-MaxZBoundZ);
					
						
				}
				SpawnPuzzleShapeLoc = SplineComp->GetLocationAtDistanceAlongSpline(CurrentLenght ,ESplineCoordinateSpace::World);
				SpawnPuzzleShapeLoc +=CurrentShift;

				if(Debug)
				{
					DrawDebugSphere(GetWorld(), SpawnPuzzleShapeLoc,SphereRadiusObj, 32, FColor::Red,false, 5.0,0,1);
				}
				
		}
	}
		SpawnPuzzlesLoc.Add(SpawnPuzzleShapeLoc);
		//ATWPuzzleVRBasicShape* SpawnedActor = SpawnPuzzle(SpawnPuzzleShapeLoc,SpawnPuzzleShapeRot,Actor);
		
		//PuzzleShapesSpawned.Add(SpawnedActor);
		//NewStaticArrayPuzzle.Add(*Actor);
		
		Index++;
	}
	FTimerDelegate TimerDel;
	// = PuzzleShapesToSpawnArray.CopyUnfrozen();
	//TArray<ATWPuzzleVRBasicShape*> NewSaticTable = NewStaticArrayPuzzle;
	TimerDel.BindUFunction(this, FName("SpawnPuzzleDelay"), SpawnPuzzlesLoc, SpawnPuzzleShapeRot,PuzzleShapesToSpawnArray);
	GetWorldTimerManager().SetTimer(PuzzleSpawnTimerHandle,TimerDel,SpawnTime,true);
}

//void ATWSplineSpawnPuzzleShape::SpawnPuzzleDelay(TArray<FVector> Locations, FRotator Rotator, TArray<ATWPuzzleVRBasicShape*> ActorsTemplate)
//{
//	ATWProceduralSpawnPuzzleShape::SpawnPuzzleDelay(Locations,Rotator,ActorsTemplate);
//}

