// Copyright (C) GAMEQUAD - All Rights Reserved


#include "VR/Hand/TWHandVR.h"

#include "Components/WidgetComponent.h"


#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATWHandVR::ATWHandVR(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	
	PrimaryActorTick.bCanEverTick = true;
	
	MotionControllerComp = ObjectInitializer.CreateDefaultSubobject<UMotionControllerComponent>(this, TEXT("MotionController"));
	MotionControllerComp->SetupAttachment(RootComponent);
	
	HandComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Hand"));
	HandComp->SetupAttachment(MotionControllerComp);

	WidgetInteractionComp = ObjectInitializer.CreateDefaultSubobject<UWidgetInteractionComponent>(this, TEXT("WidgetInteractionComp"));
	WidgetInteractionComp->SetupAttachment(MotionControllerComp);

	HandComp->SetRelativeLocationAndRotation(FVector::ZeroVector, FQuat::Identity);
	HandComp->SetRelativeScale3D(FVector(1, 1, 1));
	HandComp->SetVisibility(true);
	HandComp->SetHiddenInGame(false);
	 
	PlaceForGrabDistanceObject = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("PlaceForGrabByDistanceOBject"));
	PlaceForGrabDistanceObject->SetupAttachment(MotionControllerComp);

	PointerNiagaraParticle = ObjectInitializer.CreateDefaultSubobject<UNiagaraComponent>(this, TEXT("PointerParticcle"));
	PointerNiagaraParticle->SetupAttachment(PlaceForGrabDistanceObject);

	PointerMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("PointerMesh"));
	PointerMesh->SetupAttachment(MotionControllerComp);
	
}
//----------------------------------------------------------------------//
// Basic
//----------------------------------------------------------------------//
void ATWHandVR::BeginPlay()
{
	Super::BeginPlay();

	if(PointerNiagaraParticle)
	{
		PointerNiagaraParticle->SetVisibility(true);
		ChangeNiagaraParticleAsset(PointerNiagaraParticle, PointerNiagaraAssetNormal);
	}

}

void ATWHandVR::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(!ObjectActorInHand)
	{
		const TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2) };
		FHitResult HitResult;
		
		SetupTraceObjectByDistance(ObjectTypes,HitResult);
		EndLocationPointerParticle(HitResult.Location);
	}

	if(PointerMesh)
	{
		bool isHoveredWidget = (WidgetInteractionComp->GetHoveredWidgetComponent() != nullptr);
		if(isHoveredWidget)
		{
			FVector ShiftFromSurface = WidgetInteractionComp->GetForwardVector() * SurfaceZShiftPointerMesh; //(WidgetInteractionComp->GetLastHitResult().Distance - SurfaceZShiftPointerMesh);
			PointerMesh->SetWorldLocation(WidgetInteractionComp->GetLastHitResult().Location - ShiftFromSurface); //PointerMesh->SetWorldLocation(WidgetInteractionComp->GetComponentLocation() + ShiftFromSurface);
			
			if(!PointerMesh->IsVisible())
			{
				PointerMesh->SetVisibility(true);
			}

			
		}
		else
		{
			if(PointerMesh->IsVisible())
			{
				PointerMesh->SetVisibility(false);
			}
		
		}
	}
	
}
//----------------------------------------------------------------------//
// Grab Item
//----------------------------------------------------------------------//
void ATWHandVR::GrabPressed()
{

	eHandState = EHandState::Grab;

	
	if(ObjectTraceByDistance != nullptr)
	{
		float distanceToTracedObject = ObjectTraceByDistance->GetDistanceTo(this);
		if(distanceToTracedObject  >= MinDistanceFromObjectAnim)
		{
			HandleInteractivePointObj(false, ObjectTraceByDistance,0);
		}
		
	}

	
	if(ObjectActorInHand)
	{
		return;
	}
	
	TArray<FHitResult> HitResults;
	if(TraceMultiObjectsBySphereInHand( HitResults))
	{
		if(AActor* ActorValid = CheckTraceObjects(HitResults))
		{
			AActor* ValidParentActor = ActorValid->GetAttachParentActor();
			if(ValidParentActor)
			{
				UE_LOG(LogTemp, Log, TEXT("Attach"));
				return;
			}
			
			if(AttachItemToHand(ActorValid))
			{
				SetActorToObjectInHand(*ActorValid);
				
			
			}
		}
		//GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Black, FString::Printf(TEXT("Hit")));
	}

	ChangeNiagaraParticleAsset(PointerNiagaraParticle, PointerNiagaraAssetHold);
}

void ATWHandVR::GrabReleased()
{
	eHandState = EHandState::Idle;
	
	if(ObjectActorInHand)
	{
		FDetachmentTransformRules DetachmentRules = FDetachmentTransformRules::KeepWorldTransform;
		ObjectActorInHand->DetachFromActor(DetachmentRules);
		ObjectActorInHand = nullptr;
		ChildOffObjectActorInHand = nullptr;

		
	}

	ChangeNiagaraParticleAsset(PointerNiagaraParticle, PointerNiagaraAssetNormal);
}

void ATWHandVR::TriggerGrabPressed()
{
	
	if(ObjectActorInHand)
	{
		return;
	}

	if(ObjectTraceByDistance != nullptr)
	{
		float distanceToTracedObject = ObjectTraceByDistance->GetDistanceTo(this);
		if(distanceToTracedObject  >= MinDistanceFromObjectAnim)
		{
			HandleInteractivePointObj(false, ObjectTraceByDistance,0);
		}
	}

	eHandState = EHandState::Grab;
	
	bTriggerGrab = true;
	TArray<FHitResult> HitResults;
	if(TraceMultiObjectsBySphereInHand( HitResults))
	{
		if(AActor* ActorValid = CheckTraceObjects(HitResults))
		{

			AActor* ValidParentActor = ActorValid->GetAttachParentActor();
			if(ValidParentActor)
			{
				UE_LOG(LogTemp, Log, TEXT("Attach"));
				return;
			}
			
			if(AttachItemToHand(ActorValid))
			{
				SetActorToObjectInHand(*ActorValid);
			}
		}
		//GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Black, FString::Printf(TEXT("Hit")));
	}
	ChangeNiagaraParticleAsset(PointerNiagaraParticle, PointerNiagaraAssetHold);
}

void ATWHandVR::TriggerGrabReleased()
{
	eHandState = EHandState::Idle;
	
	bTriggerGrab = false;
	if(ObjectActorInHand)
	{
		FDetachmentTransformRules DetachmentRules = FDetachmentTransformRules::KeepWorldTransform;
		ObjectActorInHand->DetachFromActor(DetachmentRules);
		ObjectActorInHand = nullptr;
		ChildOffObjectActorInHand = nullptr;
	}

	ChangeNiagaraParticleAsset(PointerNiagaraParticle, PointerNiagaraAssetNormal);
}

void ATWHandVR::GrabByDistance()
{
	if(!ObjectActorInHand)
	{
		const TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2) };
		FHitResult HitResult;
		
		ForwardHandTraceByDistance(ObjectTypes,HitResult);
		EndLocationPointerParticle(HitResult.Location);
		
		AActor* Actor = GetTraceObjectByDistance();
		const FVector Location = GetPlaceForGrabDistanceObject()->GetComponentLocation();
		
		if(Actor != nullptr)
		{
			float DistanceBetween =  UKismetMathLibrary::Vector_Distance(HitResult.Location, Actor->GetActorLocation());
			const FVector NewLoc = Location + (GetPlaceForGrabDistanceObject()->GetComponentLocation().ForwardVector * DistanceBetween);
			
			Actor->SetActorLocation(NewLoc);
			//GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Black, FString::Printf(TEXT("Hit")));	
		}
		
	}
	
	
}


//----------------------------------------------------------------------//
// Function
//----------------------------------------------------------------------//
bool ATWHandVR::AttachItemToHand(AActor* Actor)
{
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::KeepWorldTransform;
	AttachmentRules.LocationRule = EAttachmentRule::KeepWorld;
	AttachmentRules.RotationRule = EAttachmentRule::KeepWorld;
	AttachmentRules.ScaleRule = EAttachmentRule::KeepWorld;
	
	Actor->AttachToActor(this, AttachmentRules);
	return true;
}
AActor* ATWHandVR::CheckTraceObjects(TArray<FHitResult>& HitArray)
{
	TArray<AActor*> Actors;
	TArray<FHitResult> ValidHitResults;
	
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
	
	AActor* TheNearestActor = GetTheNearestObjectFromThis(Actors);
	return TheNearestActor;
	
}
bool ATWHandVR::ConditionForCheckTraceObject(FHitResult& HitResult)
{
	AActor* ActorCast = Cast<AActor>(HitResult.GetActor());
	bool bCheck = IsValid(ActorCast); 
	return bCheck;
}

AActor* ATWHandVR::GetTheNearestObjectFromThis(TArray<AActor*> Array)
{
	if(Array.Num() > 0)
	{
		TMap<int, float> TMapFloatDistance;
		int indexArray = 0;

	
		for (AActor* Actor : Array)
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
	return nullptr;
}

bool ATWHandVR::TraceMultiObjectsBySphereInHand(TArray<FHitResult>& HitResults)
{
	const FVector Start = HandComp->GetSocketLocation("cubeSocket"), End= HandComp->GetSocketLocation("cubeSocket");
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	const TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic) };
	
	EDrawDebugTrace::Type DrawDebugTypeSphere;
	bDebug ? DrawDebugTypeSphere = EDrawDebugTrace::ForDuration : DrawDebugTypeSphere = EDrawDebugTrace::None;

	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects( GetWorld(), Start, End, SphereTraceRadius, ObjectTypes, false,ActorsToIgnore, DrawDebugTypeSphere,HitResults, true, FLinearColor::Gray, FLinearColor::Blue, 3.0f);
	return bHit;
}

bool ATWHandVR::ForwardHandTraceByDistance(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, FHitResult& HitArray)
{
	const FVector Start = PlaceForGrabDistanceObject->GetComponentLocation();//HandComp->GetSocketLocation("cubeSocket");
	const FVector End = Start + PlaceForGrabDistanceObject->GetForwardVector() * 1000;
	return DefaultSphereTraceSingle(Start, End,ObjectTypes,HitArray,SphereRadiusObjectTraceByDistance);
}

bool ATWHandVR::DefaultSphereTraceSingle(const FVector& Start, const FVector& End, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, FHitResult& HitArray, float Radius, const bool bIgnoreOwner)
{
	TArray<AActor*> ActorsToIgnore;
	if(bIgnoreOwner)
	{
		ActorsToIgnore.Add(GetOwner());
	}
	
	EDrawDebugTrace::Type DrawDebugTypeSphere;
	bDebug ? DrawDebugTypeSphere = EDrawDebugTrace::ForDuration : DrawDebugTypeSphere = EDrawDebugTrace::None;
	
	return UKismetSystemLibrary::SphereTraceSingleForObjects( GetWorld(), Start, End, Radius, ObjectTypes, false,ActorsToIgnore, DrawDebugTypeSphere,HitArray, true, FLinearColor::Gray, FLinearColor::Blue,3.0f);

}


bool ATWHandVR::DefaultLineTrace(const FVector& Start,const FVector& End, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, FHitResult& HitResult)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	EDrawDebugTrace::Type DrawDebugTypeSphere;
	bDebug ? DrawDebugTypeSphere = EDrawDebugTrace::ForDuration : DrawDebugTypeSphere = EDrawDebugTrace::None;
	
	return UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, ObjectTypes, false,ActorsToIgnore, DrawDebugTypeSphere,HitResult, true, FLinearColor::Gray, FLinearColor::Blue, 3.0f);
}

void ATWHandVR::SetupTraceObjectByDistance(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, FHitResult& HitResult)
{
	bIsObjectTraceByDistance = ForwardHandTraceByDistance(ObjectTypes, HitResult);
	if(bIsObjectTraceByDistance)
	{
		ObjectTraceByDistance = HitResult.GetActor();
		DistanceFromObject = HitResult.GetActor()->GetDistanceTo(this);

		if(eHandState != EHandState::Grab && DistanceFromObject > MinDistanceFromObjectAnim)
		{
			if((LastObjectTraceByDistance == nullptr || !bDistanceLastCheck))
			{
				HandleInteractivePointObj(true, ObjectTraceByDistance,DistanceFromObject);
			}
			
			if(LastObjectTraceByDistance != ObjectTraceByDistance)
			{
				HandleInteractivePointObj(false, LastObjectTraceByDistance,0);
				HandleInteractivePointObj(true, ObjectTraceByDistance,DistanceFromObject);
			}
			
		}
		
		if(DistanceFromObject <= MinDistanceFromObjectAnim && bDistanceLastCheck)
		{
			HandleInteractivePointObj(false, LastObjectTraceByDistance,0);
		}
	
		
		if(eHandState != EHandState::Grab)
		{
			eHandState = EHandState::Point;
			
		}
		LastObjectTraceByDistance = ObjectTraceByDistance;
		bDistanceLastCheck = DistanceFromObject > MinDistanceFromObjectAnim;
	}
	else
	{
		ObjectTraceByDistance = nullptr;
		DistanceFromObject = 0;

		if(eHandState != EHandState::Grab)
		{
			eHandState = EHandState::Idle;
		}

		if(LastObjectTraceByDistance != nullptr)
		{
			HandleInteractivePointObj(false, LastObjectTraceByDistance, 0);
		}

		
		bDistanceLastCheck = false;
		LastObjectTraceByDistance = nullptr;
	}
}

void ATWHandVR::EndLocationPointerParticle(FVector End)
{
	if(PointerNiagaraParticle)
	{
		if(bIsObjectTraceByDistance)
		{
			const FVector LocalPos = UKismetMathLibrary::InverseTransformLocation(PointerNiagaraParticle->GetComponentTransform(), End);
			PointerNiagaraParticle->SetVectorParameter("BeamEnd", LocalPos);
		}
		else
		{
			FVector Pos = PointerNiagaraParticle->GetRelativeLocation();
			Pos.X +=DefaultEndLocationPointerParticle;
			PointerNiagaraParticle->SetVectorParameter("BeamEnd",Pos );
		}
		//UKismetMathLibrary::TransformLocation()
		//DrawDebugCapsule(GetWorld(), LocalPos, 5,8, FQuat::Identity,  FColor::Red, true, 3);
	}
}

void ATWHandVR::SetActorToObjectInHand(AActor& Actor)
{
	ObjectActorInHand = &Actor;
}

bool ATWHandVR::ChangeNiagaraParticleAsset(UNiagaraComponent* Comp, UNiagaraSystem* Asset)
{
	if(Comp)
	{
		if(Asset)
		{
			Comp->SetAsset(Asset);
			return true;
		}
	}
	
	return false;
}

void ATWHandVR::HandleInteractivePointObj(bool bStart,AActor* Actor,float Distance)
{

}
