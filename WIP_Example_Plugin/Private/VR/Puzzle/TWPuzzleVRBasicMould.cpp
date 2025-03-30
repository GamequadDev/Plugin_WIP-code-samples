// Copyright (C) GAMEQUAD - All Rights Reserved


#include "VR/Puzzle/TWPuzzleVRBasicMould.h"

#include "VR/Game/TWGameControllerVRPuzzle.h"
#include "Game/TWGameInstance.h"
#include "VR/Puzzle/TWPuzzleVRBasicShape.h"


// Sets default values
ATWPuzzleVRBasicMould::ATWPuzzleVRBasicMould(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ATWPuzzleVRBasicMould::BeginPlay()
{
	Super::BeginPlay();

	SpawnPuzzlePiecesFromPrefab();
	SetupStatusAndNumber();

	StartScaleObj = GetActorScale3D();
	SetupTimelineSpawnScaleAnim();
	CurveTimelineSpawnMouldScaleAnim.PlayFromStart();
}


// Called every frame
void ATWPuzzleVRBasicMould::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurveTimelineSpawnMouldScaleAnim.TickTimeline(DeltaTime);
	
	const bool bIsGreaterThanZero = MouldPlaceHoldersArray.Num() > 0;
	const bool bIsMouldElement = MouldStatus == EPuzzleMouldStatus::MouldSet;
	if(bIsGreaterThanZero  && bIsMouldElement  && bComplete) //&& PuzzleStatus ==  EPuzzleShapeStatus::PuzzleElement
	{
		TheNearestPuzzleMouldActor = Cast<ATWPuzzleVRBasicMould>(GetTheNearestActorFromArrayTemplate(MouldPlaceHoldersArray));
		if(TheNearestPuzzleMouldActor)
		{
			const bool bIsLastTheNearestAsMouldPlaceHolder = LastTheNearestPuzzleMouldActor->GetMouldStatus() == EPuzzleMouldStatus::MouldPlaceHolder;
			if(LastTheNearestPuzzleMouldActor && bIsLastTheNearestAsMouldPlaceHolder && !LastTheNearestPuzzleMouldActor->bComplete)
			{
				LastTheNearestPuzzleMouldActor->SetMeshMaterialAsPlaceHolderMaterialForChildren();
			}

			const bool bIsTheNearestAsMouldPlaceHolder = TheNearestPuzzleMouldActor->GetMouldStatus() == EPuzzleMouldStatus::MouldPlaceHolder;
			if(bIsTheNearestAsMouldPlaceHolder && !TheNearestPuzzleMouldActor->bComplete)
			{
				TheNearestPuzzleMouldActor->SetMeshMaterialAsPlaceHolderMaterialIlluminatedForChildren();
			}
		LastTheNearestPuzzleMouldActor = TheNearestPuzzleMouldActor;
		}
	
	}
	
}

//----------------------------------------------------------------------//
// Complete Functions 
//----------------------------------------------------------------------//

void ATWPuzzleVRBasicMould::CompletePuzzle()
{
	//MouldStatus = EPuzzleMouldStatus::MouldSet;
	bComplete = true;
	
	//GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, FString::Printf(TEXT("Complete")));
	SetMouldCollisionProfileToAllChildShape();
	
	UTWGameInstance* GameInstance = Cast<UTWGameInstance>(GetGameInstance());
	if(GameInstance)
	{
		ATWGameControllerVRPuzzle* GameController = Cast<ATWGameControllerVRPuzzle>(GameInstance->GetGameController());
		GameController->CompleteStage(this);
	}
	
}

//----------------------------------------------------------------------//
// Set Functions 
//----------------------------------------------------------------------//

void ATWPuzzleVRBasicMould::SetMouldCollisionProfileToAllChildShape()
{
	
	TArray<ATWPuzzleVRBasicShape*> PuzzleShapeActors = GetPuzzleShapesChildFromMould();
	for(ATWPuzzleVRBasicShape* PuzzleShapeActor : PuzzleShapeActors)
	{
		PuzzleShapeActor->BasicStaticMesh->SetCollisionProfileName("IncludeMouldOverlapAll");
	}

	
}


bool ATWPuzzleVRBasicMould::SetMouldToPlaceHolder()
{
	if(TheNearestPuzzleMouldActor)
	{
		const bool bCompareClass = this->GetClass() == TheNearestPuzzleMouldActor->GetClass();
		if(bCompareClass)
		{
			const bool bCompareScale = this->GetActorScale() == TheNearestPuzzleMouldActor->GetActorScale();
			if(bCompareScale)
			{
				const bool bCompareBounds = CompareBoundsBetweenThisAndActor(TheNearestPuzzleMouldActor, FTolerance, true);
				if(bCompareBounds)
				{
					ATWPuzzleVRBasicMould* LocalMould = TheNearestPuzzleMouldActor;
					TheNearestPuzzleMouldActor = nullptr;
					
					
					TArray<ATWPuzzleVRBasicShape*> BasicShapeChildArray = LocalMould->GetPuzzleShapesChildFromMould();
					
					for(ATWPuzzleVRBasicShape* BasicShapeActor : BasicShapeChildArray)
					{
						if(BasicShapeActor)
						{
							BasicShapeActor->SetMeshCollisionProfile("IgnoreMouldOverlapAll");
							//GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Black, FString::Printf(TEXT("Num of Puzzle Shape this mould %s"),*BasicShapeActor->GetBasicMaterial()->GetFName().ToString() ));
							
							BasicShapeActor->SetMeshMaterialAsBasicMaterial();
							BasicShapeActor->SetBoolPuzzleElement(true);
							
						}
					}
					
					ATWPuzzleVRBasicMould* ParentMould = Cast<ATWPuzzleVRBasicMould>(LocalMould->GetAttachParentActor());
					if(ParentMould != nullptr)
					{
						//GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Blue, FString::Printf(TEXT("All") ));
						ParentMould->ChangeNumPuzzleElementsCurrent(-1);
					}else
					{
						//GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Blue, FString::Printf(TEXT("No") ));
					}
					
					LocalMould->bComplete = true;
					//TheNearestPuzzleMouldActor->bComplete = true;
					//TheNearestPuzzleMouldActor = nullptr;
					//ReplacePlaceHolder();
					return true;
				}
			}	
		}
	}
	
	return false;
}



//----------------------------------------------------------------------//
// Get Child Actors Functions 
//----------------------------------------------------------------------//

TArray<ATWPuzzleVRBasicMould*> ATWPuzzleVRBasicMould::GetMouldsChildFromMould()
{
	TArray<ATWPuzzleVRBasicMould*> MouldsArray;

	TArray<AActor*> ActorsArray;
	this->GetAllChildActors(ActorsArray);
	for(AActor* Actor : ActorsArray)
	{
		ATWPuzzleVRBasicMould* Mould = Cast<ATWPuzzleVRBasicMould>(Actor);
		if(Mould)
		{
			MouldsArray.Add(Mould);
		}
	}
	
	return MouldsArray;
}

TArray<ATWPuzzleVRBasicShape*> ATWPuzzleVRBasicMould::GetPuzzleShapesChildFromMould()
{
	TArray<ATWPuzzleVRBasicShape*> MouldsArray;

	//TArray<AActor*> ActorsArray;
	TArray<AActor*> AttachActors;
	
	//this->GetAllChildActors(ActorsArray);
	this->GetAttachedActors(AttachActors);
	
	for(AActor* Actor : AttachActors)
	{
		ATWPuzzleVRBasicShape* PuzzleShape = Cast<ATWPuzzleVRBasicShape>(Actor);
		if(PuzzleShape)
		{
			MouldsArray.Add(PuzzleShape);
		}
	}
	
	
	return MouldsArray;
}

//----------------------------------------------------------------------//
// Set Material Functions
//----------------------------------------------------------------------//

void ATWPuzzleVRBasicMould::SetMaterialToAllBasicShapeChild(UMaterial* Material)
{
	
	TArray<ATWPuzzleVRBasicShape*> PuzzleShapeActors = GetPuzzleShapesChildFromMould();
	for(ATWPuzzleVRBasicShape* PuzzleShapeActor : PuzzleShapeActors)
	{
		PuzzleShapeActor->BasicStaticMesh->SetMaterial(0,Material);
	}
	
}
void ATWPuzzleVRBasicMould::SetMeshMaterialAsPlaceHolderMaterialForChildren()
{
	TArray<ATWPuzzleVRBasicShape*> LocalBasicShapesArray = this->GetPuzzleShapesChildFromMould();

	for(ATWPuzzleVRBasicShape* BasicShape : LocalBasicShapesArray)
	{
		if(BasicShape)
		{
			BasicShape->SetMeshMaterialAsPlaceHolderMaterial();
		}
	}
}

void ATWPuzzleVRBasicMould::SetMeshMaterialAsPlaceHolderMaterialIlluminatedForChildren()
{

	TArray<ATWPuzzleVRBasicShape*> LocalBasicShapesArray = this->GetPuzzleShapesChildFromMould();

	for(ATWPuzzleVRBasicShape* BasicShape : LocalBasicShapesArray)
	{
		if(BasicShape)
		{
			BasicShape->SetMeshMaterialAsPlaceHolderMaterialIlluminated();
		}
	}
}
//----------------------------------------------------------------------//
// Check Functions
//----------------------------------------------------------------------//

bool ATWPuzzleVRBasicMould::CheckHasMouldAsChild()
{
	bool AnotherActor = false;
	TArray<AActor*> Actors;
	GetAttachedActors(Actors);
	for(AActor* Actor : Actors)
	{
		const ATWPuzzleVRBasicMould* AttachPuzzleVRBasicMould = Cast<ATWPuzzleVRBasicMould>(Actor);
		const ATWPuzzleVRBasicShape* AttachPuzzleVRBasicShape =  Cast<ATWPuzzleVRBasicShape>(Actor);
		if(AttachPuzzleVRBasicMould)
		{
			HasMouldChild = true;
		}
		else if(HasMouldChild == true || AnotherActor)
		{
			UE_LOG(LogTemp, Error, TEXT("Error: Mould have a another mould and another actor"));
		}
		else if(!AttachPuzzleVRBasicShape)
		{
			AnotherActor = true;
		}
	}
	return HasMouldChild;
}
//----------------------------------------------------------------------//
// Change Functions
//----------------------------------------------------------------------//
void ATWPuzzleVRBasicMould::ChangeNumPuzzleElementsCurrent(int IntValue)
{
	NumPuzzleElementsCurrent += IntValue;
	if(NumPuzzleElementsCurrent == 0)
	{
		const bool bIsMouldMain = MouldStatus == EPuzzleMouldStatus::MouldMain;
		if(!bIsMouldMain)
		{
			MouldStatus = EPuzzleMouldStatus::MouldSet;
		}
		
		CompletePuzzle();
	}
}

//----------------------------------------------------------------------//
// Dynamic Functions
//----------------------------------------------------------------------//

void ATWPuzzleVRBasicMould::SpawnPuzzlePiecesFromPrefab()
{
	if(PuzzlePrefab)
	{
		FVector PrefabLoc = GetActorLocation();
		FRotator PrefabRotator = GetActorRotation();
		FActorSpawnParameters PrefabSpawnParameters;
		ATWPuzzleVRBasicPrefab* PrefabActor = GetWorld()->SpawnActor<ATWPuzzleVRBasicPrefab>(PuzzlePrefab,PrefabLoc,PrefabRotator, PrefabSpawnParameters);
		TArray<UStaticMeshComponent*> MeshComp = PrefabActor->GetMeshComps();
		//PuzzlePrefab->GetMesh
		for(UStaticMeshComponent* Mesh : MeshComp)
		{
			//PuzzlePrefab->GetMeshComps()
			FVector ActorLoc = Mesh->GetComponentLocation();
			FRotator ActorRotator = Mesh->GetComponentRotation();
			FActorSpawnParameters SpawnParameters;
			ATWPuzzleVRBasicShape* Actor = GetWorld()->SpawnActor<ATWPuzzleVRBasicShape>(PuzzleBasicShapeForPrefab,ActorLoc,ActorRotator, SpawnParameters);
			Actor->AttachToActor(this,  FAttachmentTransformRules::KeepWorldTransform);
			
			Actor->SetMeshMaterial(Mesh->GetMaterial(0));
			
			if(Mesh->GetMaterials().Num() > 1)
			{
				Actor->SetBasicMaterial(Mesh->GetMaterial(0));
				TArray<UMaterialInterface*> Materials = Mesh->GetMaterials();
				Actor->SetBasicMaterials(Materials);
			}
			else
			{
				Actor->SetBasicMaterial(Mesh->GetMaterial(0));
			}
			
			Actor->BasicStaticMesh->SetStaticMesh(Mesh->GetStaticMesh());
			
			
			Actor->SetActorScale3D(Mesh->GetComponentScale());
			Actor->DefaultScaleObj = Actor->GetActorScale3D();

			
			Actor->SetupStatusAndMaterial();
			
		}
		PrefabLoc.Z = PrefabLoc.Z + 1000;
		if(PrefabActor)
		{
			PrefabActor->SetActorLocation(PrefabLoc);
			PrefabActor->Destroy();
		}
		
	}
}

void ATWPuzzleVRBasicMould::SetupStatusAndNumber()
{
	TArray<AActor*> OutActors;
	GetAttachedActors(OutActors);
	
	NumPuzzleElements = 0;
	
	for(AActor* LocalActor : OutActors)
	{
		ATWPuzzleVRBasicShape* LocalPuzzleActor = Cast<ATWPuzzleVRBasicShape>(LocalActor);
		if(LocalPuzzleActor)
		{
			if(LocalPuzzleActor->GetPuzzleShapeStatus() == EPuzzleShapeStatus::PuzzlePlaceHolder)
			{
				NumPuzzleElements++;
				LocalPuzzleActor->DefaultScaleObj = LocalPuzzleActor->GetActorScale3D();
				
				continue;
			}
		}
		
		ATWPuzzleVRBasicMould* LocalMould = Cast<ATWPuzzleVRBasicMould>(LocalActor);
		if(LocalMould)
		{
			if(LocalMould->GetMouldStatus() == EPuzzleMouldStatus::MouldPlaceHolder)
			{
				NumPuzzleElements++;
				
			}
		}
		
	}
	
	//NumPuzzleElements = OutActors.Num();
	
	NumPuzzleElementsCurrent = NumPuzzleElements;
	
	AActor* LocAttachParentActor = GetAttachParentActor();
	
	if(LocAttachParentActor != nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("MouldPlaceHolder"));
		MouldStatus = EPuzzleMouldStatus::MouldPlaceHolder;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MouldElement"));
		MouldStatus = EPuzzleMouldStatus::MouldElement;
	}

}


void ATWPuzzleVRBasicMould::SetTheNearestMouldActor()
{
	const bool bIsGreaterThanZero = MouldPlaceHoldersArray.Num() > 0;
	const bool bIsMouldElement = MouldStatus == EPuzzleMouldStatus::MouldSet;
	if(bIsGreaterThanZero  && bIsMouldElement  && bComplete) //&& PuzzleStatus ==  EPuzzleShapeStatus::PuzzleElement
		{
		TheNearestPuzzleMouldActor = Cast<ATWPuzzleVRBasicMould>(GetTheNearestActorFromArrayTemplate(MouldPlaceHoldersArray));
		if(TheNearestPuzzleMouldActor)
		{
			const bool bIsLastTheNearestAsMouldPlaceHolder = LastTheNearestPuzzleMouldActor->GetMouldStatus() == EPuzzleMouldStatus::MouldPlaceHolder;
			if(LastTheNearestPuzzleMouldActor && bIsLastTheNearestAsMouldPlaceHolder && !LastTheNearestPuzzleMouldActor->bComplete)
			{
				LastTheNearestPuzzleMouldActor->SetMeshMaterialAsPlaceHolderMaterialForChildren();
			}

			const bool bIsTheNearestAsMouldPlaceHolder = TheNearestPuzzleMouldActor->GetMouldStatus() == EPuzzleMouldStatus::MouldPlaceHolder;
			if(bIsTheNearestAsMouldPlaceHolder && !TheNearestPuzzleMouldActor->bComplete)
			{
				TheNearestPuzzleMouldActor->SetMeshMaterialAsPlaceHolderMaterialIlluminatedForChildren();
			}
			LastTheNearestPuzzleMouldActor = TheNearestPuzzleMouldActor;
		}
	
		}
}

//************************************//
//
//	Play anim spawn
//
//************************************//

void ATWPuzzleVRBasicMould::SetupTimelineSpawnScaleAnim()
{
	if(CurveFloatSpawnScaleAnim)
	{
		FOnTimelineFloat  TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineSpawnScaleAnim"));
		CurveTimelineSpawnMouldScaleAnim.AddInterpFloat(CurveFloatSpawnScaleAnim, TimelineProgress);
		CurveTimelineSpawnMouldScaleAnim.SetLooping(false);
		CurveTimelineSpawnMouldScaleAnim.SetTimelineLength(1);
	}
}

void ATWPuzzleVRBasicMould::TimelineSpawnScaleAnim(float Value)
{
	//if(bLoadSave)
	//{
		//return;
	//}
	
	const FVector Scale = FVector(StartScaleObj.X * Value,StartScaleObj.Y * Value, StartScaleObj.Z * Value);
	SetActorScale3D(Scale);
	if(Scale.Equals(StartScaleObj))
	{
		CurveTimelineSpawnMouldScaleAnim.Stop();
	}
}

void ATWPuzzleVRBasicMould::StopAnimScale()
{
	if(CurveTimelineSpawnMouldScaleAnim.IsPlaying())
	{
		CurveTimelineSpawnMouldScaleAnim.Stop();
	}
	SetActorScale3D(StartScaleObj);
}
