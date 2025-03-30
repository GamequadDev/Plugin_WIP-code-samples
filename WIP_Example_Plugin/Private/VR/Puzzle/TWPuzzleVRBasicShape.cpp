// Copyright (C) GAMEQUAD - All Rights Reserved

#include "VR/Puzzle/TWPuzzleVRBasicShape.h"

#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATWPuzzleVRBasicShape::ATWPuzzleVRBasicShape(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BasicStaticMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMesh"));

	SetRootComponent(BasicStaticMesh);
	BasicStaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ATWPuzzleVRBasicShape::MeshOverlapBegin);
	BasicStaticMesh->OnComponentEndOverlap.AddDynamic(this, &ATWPuzzleVRBasicShape::MeshOverlapEnd);

	
	TimelineCompOverlayMaterial = ObjectInitializer.CreateDefaultSubobject<UTimelineComponent>(this, TEXT("TimelineOverlayMaterial"));
	
	//Bind the Callbackfuntion for the float return value
	HandleTimelineOverMaterial.BindUFunction(this, FName{ TEXT("TimelineOverlayMaterial") });
	
}

// Called when the game starts or when spawned
void ATWPuzzleVRBasicShape::BeginPlay()
{
	Super::BeginPlay();
	SetupStatusAndMaterial();
	
	if(PuzzleStatus == EPuzzleShapeStatus::PuzzleElement)
	{
		SetupTimelineScaleObjAnim();
		SetupTimelineSpawnScaleAnim();
	}
	
	TimelineCompOverlayMaterial->AddInterpFloat(CurveFloatOverlayMaterial, HandleTimelineOverMaterial, FName{ TEXT("Value")}); // Tymczasowe bedzie lepsza wersja
	
}

// Called when the game starts or when spawned
void ATWPuzzleVRBasicShape::PostSpawnSetup()
{
	ScaleObjAnimReturn = GetActorScale3D();
	
	CurveTimelineSpawnScaleAnim.PlayFromStart();
}

// Called every frame
void ATWPuzzleVRBasicShape::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	const bool bIsGreaterThanZero = PuzzlePlaceHoldersArray.Num() > 0;
	const bool bIsPuzzleElement = PuzzleStatus ==  EPuzzleShapeStatus::PuzzleElement;
	if(bIsGreaterThanZero && !bPlaceHolder && bIsPuzzleElement)
	{
		TheNearestPuzzleBasicShapeActor = ComparePuzzleShapeStaticMeshAndLenghtOffThis(PuzzlePlaceHoldersArray);
		if(!TheNearestPuzzleBasicShapeActor)
		{
			TheNearestPuzzleBasicShapeActor = Cast<ATWPuzzleVRBasicShape>(GetTheNearestActorFromArrayTemplate(PuzzlePlaceHoldersArray));
		}
		
		if(TheNearestPuzzleBasicShapeActor)
		{
			if(LastTheNearestPuzzleBasicShapeActor && !LastTheNearestPuzzleBasicShapeActor->bPuzzleElementSet)
			{
				LastTheNearestPuzzleBasicShapeActor->SetOneMaterialMeshMaterials(PlaceHolderMaterial);
			}

			if(!TheNearestPuzzleBasicShapeActor->bPuzzleElementSet)
			{

				TheNearestPuzzleBasicShapeActor->SetOneMaterialMeshMaterials(PlaceHolderMaterialIlluminated);
				bool IsFit = IsPlaceHolderFit();
				if(IsFit)
				{
					TheNearestPuzzleBasicShapeActor->SetOneMaterialMeshMaterials(PlaceHolderMaterialFit);
				}
				
			}
			LastTheNearestPuzzleBasicShapeActor = TheNearestPuzzleBasicShapeActor;
		}
	}
	
	CurveTimelineScaleObjAnim.TickTimeline(DeltaTime);
	CurveTimelineSpawnScaleAnim.TickTimeline(DeltaTime);

	if(!SmoothLocationTarget.IsZero())
	{
		const FVector Loc = GetActorLocation();
		const FVector SetLoc = FMath::VInterpConstantTo(Loc,SmoothLocationTarget,DeltaTime,SpeedSmoothMove);
		this->SetActorLocation(SetLoc);

		if(Loc.Equals(SmoothLocationTarget,0.1f))
		{
			SmoothLocationTarget = FVector::Zero();
		}
	}
	
	
}
//----------------------------------------------------------------------//
// Overlap Functions 
//----------------------------------------------------------------------//
void ATWPuzzleVRBasicShape::MeshOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	PuzzleBasicShapeActorOverlap = Cast<ATWPuzzleVRBasicShape>(OtherActor);
	if(PuzzleBasicShapeActorOverlap)
	{
		const bool bIsPuzzlePlaceHolder = PuzzleBasicShapeActorOverlap->PuzzleStatus == EPuzzleShapeStatus::PuzzlePlaceHolder;
		if(bIsPuzzlePlaceHolder)
		{
			PuzzlePlaceHoldersArray.Add(PuzzleBasicShapeActorOverlap);
			//GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Black, FString::Printf(TEXT("Begin")));
		}

		const bool bIsInMouldPlaceHolder = PuzzleBasicShapeActorOverlap->PuzzleStatus == EPuzzleShapeStatus::InMouldPlaceHolder;
		if(bIsInMouldPlaceHolder)
		{
			ATWPuzzleVRBasicMould* MouldParent = Cast<ATWPuzzleVRBasicMould>(PuzzleBasicShapeActorOverlap->GetAttachParentActor());
			if(MouldParent)
			{
				const bool bIsNotContain = !ActorParentMould->IsMouldPlaceHoldersArrayContainsThisItem(MouldParent);
				if(bIsNotContain)
				{
					ActorParentMould->AddMouldToMouldPlaceHoldersArray(MouldParent);
				}
			}
		
			//GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Black, FString::Printf(TEXT("Begin")));
		}
		
	}
	
	
}
void ATWPuzzleVRBasicShape::MeshOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PuzzleBasicShapeActorOverlap = Cast<ATWPuzzleVRBasicShape>(OtherActor);
	if(PuzzleBasicShapeActorOverlap)
	{
		const bool bIsPuzzlePlaceHolder = PuzzleBasicShapeActorOverlap->PuzzleStatus == EPuzzleShapeStatus::PuzzlePlaceHolder;
		if(bIsPuzzlePlaceHolder)
		{
			if(!PuzzleBasicShapeActorOverlap->bPuzzleElementSet && !PuzzleBasicShapeActorOverlap->bPuzzleElementSet)
			{
				PuzzleBasicShapeActorOverlap->SetOneMaterialMeshMaterials(PlaceHolderMaterial);
			}
			PuzzlePlaceHoldersArray.Remove(PuzzleBasicShapeActorOverlap);
		}

		const bool bIsInMouldPlaceHolder = PuzzleBasicShapeActorOverlap->PuzzleStatus == EPuzzleShapeStatus::InMouldPlaceHolder;
		if(bIsInMouldPlaceHolder)
		{
			
			// Set Material to all puzzle
			
			ATWPuzzleVRBasicMould* MouldParent = Cast<ATWPuzzleVRBasicMould>(PuzzleBasicShapeActorOverlap->GetAttachParentActor());
			if(MouldParent)
			{
				const bool bIsContain = ActorParentMould->IsMouldPlaceHoldersArrayContainsThisItem(MouldParent);
				if(bIsContain)
				{
					if(!MouldParent->bComplete)
					{
						MouldParent->SetMeshMaterialAsPlaceHolderMaterialForChildren();
					}
					
					ActorParentMould->RemoveMouldFromMouldPlaceHoldersArray(MouldParent);
				}
			}
		}
	}
	
}

//----------------------------------------------------------------------//
// Get Functions 
//----------------------------------------------------------------------//

ATWPuzzleVRBasicShape* ATWPuzzleVRBasicShape::GetTheNearestObjectFromPlaceHoldersArray(TArray<ATWPuzzleVRBasicShape*> Array)
{
	TMap<int, float> TMapFloatDistance;
	int indexArray = 0;
	
	for (ATWPuzzleVRBasicShape* Actor : Array)
	{
		TMapFloatDistance.Add(indexArray,Actor->GetDistanceTo(this));
		indexArray +=1;
	}
	
	TArray<float> FloatArray;
	int indexValue;
	float MinValue;
	
	TMapFloatDistance.GenerateValueArray(FloatArray);
	UKismetMathLibrary::MinOfFloatArray(FloatArray, indexValue, MinValue);
	
	return Array[indexValue];
}

bool ATWPuzzleVRBasicShape::CheckValidPlaceHolder()
{
	if(TheNearestPuzzleBasicShapeActor)
	{

		bool bCompareClass = this->GetClass() == TheNearestPuzzleBasicShapeActor->GetClass();
		if(bCompareClass)
		{
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------------//
// Set Functions 
//----------------------------------------------------------------------//

void ATWPuzzleVRBasicShape::SetMeshMaterial(UMaterialInterface* Material)
{
	if(BasicStaticMesh)
	{
		BasicStaticMesh->SetMaterial(0,Material);
	}
		
}

void ATWPuzzleVRBasicShape::SetMeshMaterials(TArray<UMaterialInterface*> Materials)
{
	if(BasicStaticMesh)
	{
		int MaterialIndex = 0;
		for(UMaterialInterface* Material : Materials)
		{
			BasicStaticMesh->SetMaterial(MaterialIndex,Material);
			MaterialIndex++; 
		}
	}
		
}

void ATWPuzzleVRBasicShape::SetOneMaterialMeshMaterials(UMaterialInterface* Material)
{
	if(BasicStaticMesh)
	{
		for(int MaterialIndex = 0; MaterialIndex < GetNumberMaterialsStaticMesh();MaterialIndex++)
		{
			BasicStaticMesh->SetMaterial(MaterialIndex,Material);
		}
	}
}



bool ATWPuzzleVRBasicShape::SetPuzzleToPlaceHolder()
{
	const bool bIsValid = CheckValidPlaceHolder();
	if(bIsValid)
	{
		const bool bCompareScale = this->GetActorScale3D() == TheNearestPuzzleBasicShapeActor->GetActorScale3D();
		if(bCompareScale)
		{
			const bool bCompareMaterial = this->BasicMaterial == TheNearestPuzzleBasicShapeActor->GetBasicMaterial();
			if(bCompareMaterial)
			{
				const bool bCompareBounds = CompareBoundsBetweenThisAndActor(TheNearestPuzzleBasicShapeActor, FTolerance, false);
				if(bCompareBounds)
				{

					const bool bCompareStaticMesh =  this->BasicStaticMesh->GetStaticMesh()->GetFName() == TheNearestPuzzleBasicShapeActor->BasicStaticMesh->GetStaticMesh()->GetFName();
					if(bCompareStaticMesh)
					{
						if(PuzzleToPlaceHolderSound)
						{
							UGameplayStatics::PlaySoundAtLocation(GetWorld(),PuzzleToPlaceHolderSound,GetActorLocation());
						}
						
						ReplacePlaceHolder();
						return true;
					}
				}
			}
		}
	}
	
	return false;
}

bool ATWPuzzleVRBasicShape::IsPlaceHolderFit()
{
	const bool bIsValid = CheckValidPlaceHolder();
	if(bIsValid)
	{
		const bool bCompareScale = this->GetActorScale3D() == TheNearestPuzzleBasicShapeActor->GetActorScale3D();
		if(bCompareScale)
		{
			const bool bCompareMaterial = this->BasicMaterial == TheNearestPuzzleBasicShapeActor->GetBasicMaterial();
			if(bCompareMaterial)
			{
				const bool bCompareBounds = CompareBoundsBetweenThisAndActor(TheNearestPuzzleBasicShapeActor, FTolerance, false);
				if(bCompareBounds)
				{
					const bool bCompareStaticMesh =  this->BasicStaticMesh->GetStaticMesh()->GetFName() == TheNearestPuzzleBasicShapeActor->BasicStaticMesh->GetStaticMesh()->GetFName();
					if(bCompareStaticMesh)
					{
						return true;
					}
				}
			}
		}
	}
	
	return false;
}

void ATWPuzzleVRBasicShape::ReplacePlaceHolder()
{
	if(TheNearestPuzzleBasicShapeActor)
	{
		TheNearestPuzzleBasicShapeActor->bPuzzleElementSet = true;
		
		if(TheNearestPuzzleBasicShapeActor->bMultiMaterials)
		{
			TheNearestPuzzleBasicShapeActor->SetMeshMaterials(BasicMaterials);
			
			TheNearestPuzzleBasicShapeActor->SetOverlapAnimMaterial();
		}
		else
		{
			TheNearestPuzzleBasicShapeActor->SetMeshMaterial(BasicMaterial);
			
			TheNearestPuzzleBasicShapeActor->SetOverlapAnimMaterial();
		}
		
		TheNearestPuzzleBasicShapeActor->BasicStaticMesh->SetCollisionProfileName("IgnorePuzzleAllOverlap");
		TheNearestPuzzleBasicShapeActor->SetPuzzleShapeStatus(EPuzzleShapeStatus::PuzzleSet);
		ATWPuzzleVRBasicMould* MouldActor = Cast<ATWPuzzleVRBasicMould>(TheNearestPuzzleBasicShapeActor->GetAttachParentActor());
		if(MouldActor)
		{
			MouldActor->ChangeNumPuzzleElementsCurrent(-1);
		}
		TheNearestPuzzleBasicShapeActor = nullptr;
		
	}
}
//----------------------------------------------------------------------//
// Compare Functions 
//----------------------------------------------------------------------//

ATWPuzzleVRBasicShape* ATWPuzzleVRBasicShape::ComparePuzzleShapesClassAndLenghtOffThis(TArray<ATWPuzzleVRBasicShape*> PuzzleShapeArray)
{
	ATWPuzzleVRBasicShape* PuzzleShape = nullptr;

	for(ATWPuzzleVRBasicShape* Actor : PuzzleShapeArray)
	{
		const bool bCompareClass = this->GetClass() == Actor->GetClass();
		if(bCompareClass)
		{
			const bool bCompareDistance = Actor->GetDistanceTo(this) < FTolerance;
			if(bCompareDistance)
			{
				PuzzleShape = Actor;
			}
		}
	}
	
	return PuzzleShape;
}

ATWPuzzleVRBasicShape* ATWPuzzleVRBasicShape::ComparePuzzleShapeStaticMeshAndLenghtOffThis(TArray<ATWPuzzleVRBasicShape*> PuzzleShapeArray)
{
	ATWPuzzleVRBasicShape* PuzzleShape = nullptr;

	for(ATWPuzzleVRBasicShape* Actor : PuzzleShapeArray)
	{
		const bool bCompareMeshes = this->BasicStaticMesh->GetStaticMesh() == Actor->BasicStaticMesh->GetStaticMesh();
		if(bCompareMeshes)
		{
			const bool bCompareDistance =Actor->GetDistanceTo(this) < FTolerance;
			if(bCompareDistance)
			{
				PuzzleShape = Actor;
			}
		}
	}
	return PuzzleShape;
}


//----------------------------------------------------------------------//
// Other 
//----------------------------------------------------------------------//

#if WITH_EDITOR
void ATWPuzzleVRBasicShape::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{

	const bool bCompareName = PropertyChangedEvent.GetPropertyName() == "BasicMaterial";
	if(bCompareName)
	{
		if(BasicStaticMesh->GetMaterials().Num() == 1)
		{
			BasicStaticMesh->SetMaterial(0,BasicMaterial);
		}
	}
}
#endif

void ATWPuzzleVRBasicShape::SetTheNearestPuzzleBasicShapeActor()
{
	const bool bIsGreaterThanZero = PuzzlePlaceHoldersArray.Num() > 0;
	const bool bIsPuzzleElement = PuzzleStatus ==  EPuzzleShapeStatus::PuzzleElement;
	if(bIsGreaterThanZero && !bPlaceHolder && bIsPuzzleElement)
	{
		//TheNearestPuzzleBasicShapeActor = GetTheNearestObjectFromPlaceHoldersArray(PuzzlePlaceHoldersArray);
		//TheNearestPuzzleBasicShapeActor = ComparePuzzleShapesClassAndLenghtOffThis(PuzzlePlaceHoldersArray);
		TheNearestPuzzleBasicShapeActor = ComparePuzzleShapeStaticMeshAndLenghtOffThis(PuzzlePlaceHoldersArray);
		
		if(!TheNearestPuzzleBasicShapeActor)
		{
			TheNearestPuzzleBasicShapeActor = Cast<ATWPuzzleVRBasicShape>(GetTheNearestActorFromArrayTemplate(PuzzlePlaceHoldersArray));
		}
		
		if(TheNearestPuzzleBasicShapeActor)
		{
			if(LastTheNearestPuzzleBasicShapeActor && !LastTheNearestPuzzleBasicShapeActor->bPuzzleElementSet)
			{
				LastTheNearestPuzzleBasicShapeActor->SetMeshMaterial(PlaceHolderMaterial);
			}

			if(!TheNearestPuzzleBasicShapeActor->bPuzzleElementSet)
			{
				TheNearestPuzzleBasicShapeActor->SetMeshMaterial(PlaceHolderMaterialIlluminated);
			}
			LastTheNearestPuzzleBasicShapeActor = TheNearestPuzzleBasicShapeActor;
		}
	}
}

void ATWPuzzleVRBasicShape::SetupStatusAndMaterial()
{

	if(BasicStaticMesh->GetMaterials().Num() > 1)
	{
		bMultiMaterials = true;
		
		if(BasicMaterials.Num()  == 0)
		{
			BasicMaterials = BasicStaticMesh->GetMaterials();

			TArray<UMaterialInterface*> Materials = BasicStaticMesh->GetMaterials();

			for(UMaterialInterface* Material : Materials)
			{
				FName Name = Material->GetFName();
				FString StringText = Name.ToString();
				UE_LOG(LogTemp, Warning, TEXT("Material Name: %s"), *StringText);
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("Mulit Material"));
	}

	if(BasicMaterials.Num() > 0)
	{
		FString number = FString::FromInt(BasicMaterials.Num());
		UE_LOG(LogTemp, Warning, TEXT("Mulit Material %s"), *number);
	}
	
	
	bool bIsValid = IsValid(GetAttachParentActor());
	if(bIsValid && PuzzleStatus != EPuzzleShapeStatus::PuzzleSet)
	{
		if(PuzzleStatus != EPuzzleShapeStatus::PuzzleEditor)
		{
			PuzzleStatus = EPuzzleShapeStatus::PuzzlePlaceHolder;
		}
		
		ATWPuzzleVRBasicMould* Mould =  Cast<ATWPuzzleVRBasicMould>(GetAttachParentActor());
		if(Mould)
		{
			ActorParentMould = Mould;
		}
	}

	const bool bIsGreaterThanOne = GetActorTopParentIndexInherit(this) > 1 || GetActorTopAttachParentIndexInherit(this) > 1;
	if(bIsGreaterThanOne)
	{
		BasicStaticMesh->SetCollisionProfileName("IncludeMouldOverlapAll");
		PuzzleStatus = EPuzzleShapeStatus::InMouldPlaceHolder;
	}
	
	switch (PuzzleStatus)
	{
		
		case EPuzzleShapeStatus::PuzzlePlaceHolder:


			if(bMultiMaterials)
			{
				SetOneMaterialMeshMaterials(PlaceHolderMaterial);
			}
			else
			{
				SetMeshMaterial(PlaceHolderMaterial);
			}
		
			break;
			
		case EPuzzleShapeStatus::PuzzleElement:
			
				if(bMultiMaterials)
				{
				
					SetMeshMaterials(BasicMaterials);
					
				}
				else
				{
					SetMeshMaterial(BasicMaterial);
				}
		
			break;
		
	default:
		break;
	}
	
}


//----------------------------------------------------------------------//
// ScaleAnimObj
//----------------------------------------------------------------------//


void ATWPuzzleVRBasicShape::SetupTimelineScaleObjAnim()
{
	if(CurveFloatScaleObjAnim)
	{
		FOnTimelineFloat  TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineScaleObjAnim"));
		CurveTimelineScaleObjAnim.AddInterpFloat(CurveFloatScaleObjAnim, TimelineProgress);
		CurveTimelineScaleObjAnim.SetLooping(true);
	}
	
}

void ATWPuzzleVRBasicShape::HandlePointInteractive(bool bStart, FString Name, float Distance)
{
	if(!bStart)
	{
		HandsInteractive.Remove(Name);
		if(HandsInteractive.Num() > 0)
		{
			return;
		}
		
		EnableTimelineScaleObjAnim(bStart);
		return;
	}
	
	if(HandsInteractive.Contains(Name))
	{
		
	}
	else
	{
		HandsInteractive.Add(Name);
		if(HandsInteractive.Num() == 1)
		{
			EnableTimelineScaleObjAnim(bStart);
		}
	}
	
}

void ATWPuzzleVRBasicShape::EnableTimelineScaleObjAnim(bool bEnable)
{
	
	if(PuzzleStatus == EPuzzleShapeStatus::PuzzleElement && !(CurveTimelineSpawnScaleAnim.IsPlaying()))
	{
		if(bEnable)
		{
			if(CurveTimelineScaleObjAnim.IsPlaying())
			{
				CurveTimelineScaleObjAnim.Stop();
				//return;
			}
			
			CurveTimelineScaleObjAnim.PlayFromStart();
			return;
		}
		CurveTimelineScaleObjAnim.Stop();
		SetActorScale3D(ScaleObjAnimReturn);
	}
}

void ATWPuzzleVRBasicShape::TimelineScaleObjAnim(float Value)
{
	if(bLoadSave)
	{
		return;
	}
	
	const FVector MaxScale = FVector(ScaleObjAnimReturn.X * MaxScaleObjAnim,ScaleObjAnimReturn.Y * MaxScaleObjAnim, ScaleObjAnimReturn.Z * MaxScaleObjAnim);
	const FVector ScaleAdd = FMath::Lerp(ScaleObjAnimReturn, MaxScale, Value);
	SetActorScale3D(ScaleAdd);
}
//----------------------------------------------------------------------//
// SpawnScaleAnim
//----------------------------------------------------------------------//

void ATWPuzzleVRBasicShape::SetupTimelineSpawnScaleAnim()
{
	if(CurveFloatSpawnScaleAnim)
	{
		FOnTimelineFloat  TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineSpawnScaleAnim"));
		CurveTimelineSpawnScaleAnim.AddInterpFloat(CurveFloatSpawnScaleAnim, TimelineProgress);
		CurveTimelineSpawnScaleAnim.SetLooping(false);
		CurveTimelineSpawnScaleAnim.SetTimelineLength(1);
	}
	
}

void ATWPuzzleVRBasicShape::TimelineSpawnScaleAnim(float Value)
{
	if(bLoadSave)
	{
		return;
	}
	
	const FVector Scale = FVector(ScaleObjAnimReturn.X * Value,ScaleObjAnimReturn.Y * Value, ScaleObjAnimReturn.Z * Value);
	SetActorScale3D(Scale);
	if(Scale.Equals(ScaleObjAnimReturn))
	{
		CurveTimelineSpawnScaleAnim.Stop();
	}
	
}

void ATWPuzzleVRBasicShape::StopAnimScale()
{
	if(CurveTimelineSpawnScaleAnim.IsPlaying())
	{
		CurveTimelineSpawnScaleAnim.Stop();
	}
	EnableTimelineScaleObjAnim(false);
	SetActorScale3D(ScaleObjAnimReturn);
}


//----------------------------------------------------------------------//
// Overlay Material
//----------------------------------------------------------------------//


void ATWPuzzleVRBasicShape::SetOverlapAnimMaterial()
{
	if(BasicStaticMesh)
	{
		DynOverlayMaterialTemp = UMaterialInstanceDynamic::Create(OverlapMaterial, this);
		//set paramater with Set***ParamaterValue
		//DynMaterial->SetScalarParameterValue("Time", myFloatValue);

	
		if(DynOverlayMaterialTemp)
		{
			BasicStaticMesh->SetOverlayMaterial(DynOverlayMaterialTemp);
		}
	
		//BasicStaticMesh->Overla
		TimelineCompOverlayMaterial->PlayFromStart();
		
		//BasicStaticMesh->GetOverlayMaterial()
	}
}

void ATWPuzzleVRBasicShape::TimelineOverlayMaterial(float Value)
{

	if(DynOverlayMaterialTemp)
	{
		DynOverlayMaterialTemp->SetScalarParameterValue("Opacity", Value);	
	}
	
}
void ATWPuzzleVRBasicShape::TimelineOverlayMaterialEnd()
{
	if(DynOverlayMaterialTemp)
	{
		if(BasicStaticMesh)
		{
			//DynOverlayMaterialTemp = UMaterialInstanceDynamic::Create(Materials, this);
			//set paramater with Set***ParamaterValue
			//DynMaterial->SetScalarParameterValue("Time", myFloatValue);
			//BasicStaticMesh->OverlayMaterial = nullptr;
		//	BasicStaticMesh->SetOverlayMaterial(nullptr);
		//	DynOverlayMaterialTemp = nullptr;
			//BasicStaticMesh->GetOverlayMaterial()
		}
	}
}

void ATWPuzzleVRBasicShape::SmoothTeleport(bool bEnable, FVector Target)
{
	if(bEnable)
	{
		SmoothLocationTarget = Target;
		return;
	}
	SmoothLocationTarget = FVector::Zero();
}
