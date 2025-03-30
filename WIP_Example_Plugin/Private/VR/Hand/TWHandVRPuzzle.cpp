// Copyright (C) GAMEQUAD - All Rights Reserved


#include "VR/Hand/TWHandVRPuzzle.h"

#include "VR/Puzzle/TWPuzzleVRControlPanelMould.h"
#include "VR/Puzzle/TWPuzzleVR.h"
#include "VR/Puzzle/TWPuzzleVRBasicMould.h"
#include "VR/Puzzle/TWPuzzleVRBasicShape.h"


ATWHandVRPuzzle::ATWHandVRPuzzle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void ATWHandVRPuzzle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ATWHandVRPuzzle::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATWHandVRPuzzle::GrabPressed()
{
	Super::GrabPressed();
	GrabByDistance();
	
}

void ATWHandVRPuzzle::GrabReleased()
{
	ATWPuzzleVRBasicShape* ChildPuzzle = Cast<ATWPuzzleVRBasicShape>(ChildOffObjectActorInHand);
	ATWPuzzleVRBasicShape* PuzzleActor = Cast<ATWPuzzleVRBasicShape>(ObjectActorInHand);
	ATWPuzzleVRBasicMould* PuzzleMould = Cast<ATWPuzzleVRBasicMould>(ObjectActorInHand);
	
	Super::GrabReleased();
	
	if(PuzzleActor)
	{
		if(PuzzleActor->SetPuzzleToPlaceHolder())
		{
			PuzzleActor->Destroy();
		}
	}
	
	if(PuzzleMould)
	{
		if(PuzzleMould->SetMouldToPlaceHolder())
		{
			PuzzleMould->Destroy();
		}
	}


	ATWPuzzleVRBasicShape* PuzzleActorTop = Cast<ATWPuzzleVRBasicShape>(LastActorGrabByDistance);
	if(PuzzleActorTop)
	{
		PuzzleActorTop->SmoothTeleport(false,FVector::Zero());
	}
	
}

void ATWHandVRPuzzle::TriggerGrabPressed()
{
	Super::TriggerGrabPressed();
	GrabByDistance();
}

void ATWHandVRPuzzle::TriggerGrabReleased()
{
	ATWPuzzleVRBasicShape* ChildPuzzle = Cast<ATWPuzzleVRBasicShape>(ChildOffObjectActorInHand);
	ATWPuzzleVRBasicShape* PuzzleActor = Cast<ATWPuzzleVRBasicShape>(ObjectActorInHand);
	ATWPuzzleVRBasicMould* PuzzleMould = Cast<ATWPuzzleVRBasicMould>(ObjectActorInHand);

	
	Super::TriggerGrabReleased();
	
	if(PuzzleActor)
	{
		if(PuzzleActor->SetPuzzleToPlaceHolder())
		{
			PuzzleActor->Destroy();
		}
	}
	
	if(PuzzleMould)
	{
		if(PuzzleMould->SetMouldToPlaceHolder())
		{
			PuzzleMould->Destroy();
		}
	}
	
}

void ATWHandVRPuzzle::GrabByDistance()
{
	if(!ObjectActorInHand)
	{
		//return;
		const TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2) };
		FHitResult HitResult;
		
		ForwardHandTraceByDistance(ObjectTypes,HitResult);
		EndLocationPointerParticle(HitResult.Location);
		
		AActor* Actor = GetTraceObjectByDistance();
		
		ATWPuzzleVRBasicShape* PuzzleActor = Cast<ATWPuzzleVRBasicShape>(Actor);
		if(PuzzleActor)
		{
			if(PuzzleActor->GetAttachParentActor())
			{
			Actor =	GetActorTopAttachParent(PuzzleActor);
				
				ATWPuzzleVRControlPanelMould* ControlPanelMould = Cast<ATWPuzzleVRControlPanelMould>(Actor);
				if(ControlPanelMould != nullptr)
				{
					return;
				}
			}
		}
		
		const FVector Location = GetPlaceForGrabDistanceObject()->GetComponentLocation();
		const FVector RelativeCompLoc = GetPlaceForGrabDistanceObject()->GetRelativeLocation();

		LastActorGrabByDistance = Actor;
		if(Actor != nullptr)
		{
			float DistanceBetweenMeshCollisionAndActor = UKismetMathLibrary::Vector_Distance(HitResult.Location, Actor->GetActorLocation());
			FTransform GlobalTrans; GlobalTrans.SetIdentity();
			FVector LocalLoc = RelativeCompLoc + (GetPlaceForGrabDistanceObject()->GetForwardVector() * DistanceBetweenMeshCollisionAndActor);
			const FVector GlobalLoc =  UKismetMathLibrary::TransformLocation(this->GetActorTransform(), LocalLoc);
			const FVector NewLoc = GetPlaceForGrabDistanceObject()->GetComponentLocation() + (GetPlaceForGrabDistanceObject()->GetForwardVector() * DistanceBetweenMeshCollisionAndActor);
			
			float DistanceBetweenActorAndHand =  UKismetMathLibrary::Vector_Distance(Actor->GetActorLocation(), this->GetActorLocation());
			float DistanceBetweenNewLocAndHand =  UKismetMathLibrary::Vector_Distance(NewLoc, this->GetActorLocation());
			
			const bool bCompareDistance = DistanceBetweenNewLocAndHand < DistanceBetweenActorAndHand;
			if(bCompareDistance)
			{
				ATWPuzzleVRBasicShape* PuzzleActorTop = Cast<ATWPuzzleVRBasicShape>(Actor);
				if(PuzzleActorTop)
				{
					PuzzleActorTop->SmoothTeleport(true,NewLoc);
				}
				else
				{
					Actor->SetActorLocation(NewLoc);
				}
				
				//GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Black, FString::Printf(TEXT("Hit")));	
			}
		
		}
		
	}
	
}

bool ATWHandVRPuzzle::AttachItemToHand(AActor* Actor)

{
	return Super::AttachItemToHand(Actor);

}
//----------------------------------------------------------------------//
// Function
//----------------------------------------------------------------------//
bool ATWHandVRPuzzle::TraceMultiObjectsBySphereInHand(TArray<FHitResult>& HitResults)
{
	const FVector Start = HandComp->GetSocketLocation("cubeSocket"), End= HandComp->GetSocketLocation("cubeSocket");
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
	const EObjectTypeQuery PuzzleType{ UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2) };
	const EObjectTypeQuery MouldType { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3) };
	
	ObjectTypes.Add(PuzzleType);
	ObjectTypes.Add(MouldType);
	
	EDrawDebugTrace::Type DrawDebugTypeSphere;

	bDebug ? DrawDebugTypeSphere = EDrawDebugTrace::ForDuration : DrawDebugTypeSphere = EDrawDebugTrace::None;

	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects( GetWorld(), Start, End, SphereTraceRadius, ObjectTypes, false,ActorsToIgnore, DrawDebugTypeSphere,HitResults, true, FLinearColor::Gray, FLinearColor::Blue, 3.0f);
	return bHit;
}

void ATWHandVRPuzzle::SetActorToObjectInHand(AActor& Actor)
{
	ATWPuzzleVRBasicShape* PuzzleActorTop = Cast<ATWPuzzleVRBasicShape>(LastActorGrabByDistance);
	if(PuzzleActorTop)
	{
		PuzzleActorTop->SmoothTeleport(false,FVector::Zero());
	}
	
	ChildOffObjectActorInHand = &Actor;
	//ObjectActorInHand  = GetActorTopAttachParent(&Actor);
	ObjectActorInHand  = ATWPuzzleVR::GetPuzzleTopAttachParent(&Actor);
	
	//UE_LOG(LogTemp, Warning, TEXT("The Actor's name is %s"), *ObjectActorInHand->GetName());
	//UE_LOG(LogTemp, Warning, TEXT("The Actor's TopAttachParent name is %s"), *GetActorTopAttachParent(&Actor)->GetName());
}

AActor* ATWHandVRPuzzle::CheckTraceObjects(TArray<FHitResult>& HitArray)
{
	TArray<AActor*> Actors;
	TArray<FHitResult> ValidHitResults;
	
	TArray<AActor*> NoneMouldHitResultsArray;
	TArray<AActor*> MouldHitResultsArray;

	TArray<AActor*> BasicShapeOffMouldArray;

	for(FHitResult& Hit :HitArray)
	{
		if(ConditionForCheckTraceObject(Hit))
		{
			ValidHitResults.Add(Hit);
			AActor* LocalActorCast = Cast<AActor>(Hit.GetActor());
			if(LocalActorCast)
			{
				Actors.Add(LocalActorCast);
			}
		}
	}
	
	for(FHitResult& Hit :ValidHitResults)
	{
		
		!Hit.GetActor()->GetAttachParentActor() ? NoneMouldHitResultsArray.Add(Hit.GetActor()) : MouldHitResultsArray.Add(GetActorTopAttachParent(Hit.GetActor()));
		if(Hit.GetActor()->GetAttachParentActor())
		{
			BasicShapeOffMouldArray.Add(Hit.GetActor());
		}
	}

	if(NoneMouldHitResultsArray.Num() > 0)
	{
		AActor* TheNearestActor = GetTheNearestObjectFromThis(NoneMouldHitResultsArray);
		ChildOffObjectActorInHand = TheNearestActor;
		return TheNearestActor;
	}
	else
	{

		int Index = 0;
		int IndexMinValue = 0;
		int MinValue = 0;
		
		for(AActor* Actor : BasicShapeOffMouldArray)
		{
			if(Index == 0)
			{
				MinValue =  GetActorTopAttachParentIndexInherit(Actor);
				IndexMinValue = Index;
			}

			int LocalValue = GetActorTopAttachParentIndexInherit(Actor);

			if(LocalValue < MinValue)
			{
				MinValue =  GetActorTopAttachParentIndexInherit(Actor);
				IndexMinValue = Index;
			}
			
			Index++;
		}

		AActor* TopAttachParentActor = GetActorTopAttachParent(BasicShapeOffMouldArray[IndexMinValue]);
		ChildOffObjectActorInHand = BasicShapeOffMouldArray[IndexMinValue];
		
		
		if(TopAttachParentActor->IsA(ATWPuzzleVR::StaticClass()))
		{
			return GetActorTopAttachParent(BasicShapeOffMouldArray[IndexMinValue]);
		}
		return nullptr;
		//TopAttachParentActor
		
		//AActor* TheNearestActor = GetTheNearestObjectFromThis(MouldHitResultsArray);
		
		return GetActorTopAttachParent(BasicShapeOffMouldArray[IndexMinValue]);
	}
	
}

void ATWHandVRPuzzle::HandleInteractivePointObj(bool bStart, AActor* Actor, float Distance)
{
	//Super::HandleInteractivePointObj(bStart, Actor);
	
	ATWPuzzleVRBasicShape* PuzzleActor = Cast<ATWPuzzleVRBasicShape>(Actor);
	if(PuzzleActor)
	{
		PuzzleActor->HandlePointInteractive(bStart,this->GetName(),Distance);
	}
	
}



