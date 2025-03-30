// Fill out your copyright notice in the Description page of Project Settings.


#include "VR/Hand/TWHandVRPhysics.h"


ATWHandVRPhysics::ATWHandVRPhysics(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PhysicsHandComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("PhysicsHand"));
	PhysicsHandComp->SetupAttachment(HandComp);
	
	PhysicsConstraintComp = ObjectInitializer.CreateDefaultSubobject<UPhysicsConstraintComponent>(this, TEXT("PhysicsConstraint"));
	PhysicsConstraintComp->SetupAttachment(HandComp);

	GrabbingConstraintComp = ObjectInitializer.CreateDefaultSubobject<UPhysicsConstraintComponent>(this, TEXT("GrabbingConstraint"));
	GrabbingConstraintComp->SetupAttachment(HandComp);
}

void ATWHandVRPhysics::GrabPressedPhysics()
{
	const FVector Start = PhysicsHandComp->GetSocketLocation("cubeSocket"), End= PhysicsHandComp->GetSocketLocation("cubeSocket");
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	FHitResult HitArray;
	const TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody) };

	EDrawDebugTrace::Type DrawDebugTypeSphere;
	if(bDebug)
	{
		DrawDebugTypeSphere = EDrawDebugTrace::ForDuration;
	} else {DrawDebugTypeSphere = EDrawDebugTrace::None;}
	
	const bool Hit = UKismetSystemLibrary::SphereTraceSingleForObjects( GetWorld(), Start, End, SphereTraceRadius, ObjectTypes, false,ActorsToIgnore, DrawDebugTypeSphere,HitArray, true, FLinearColor::Gray, FLinearColor::Blue, 3.0f);

	if(Hit)
	{
		if(HitArray.GetComponent()->IsSimulatingPhysics())
		{ 
			GrabbingConstraintComp->SetConstrainedComponents(PhysicsHandComp,"None",HitArray.GetComponent(), "None");
			bObjectInHand = true;
			
				SetFingerPos(bRighHand,EFingersName::Thumb,FingersSocket.ThumbFinger, PhysicsHandComp, ObjectTakenHand);
				SetFingerPos(bRighHand,EFingersName::Index,FingersSocket.IndexFinger, PhysicsHandComp, ObjectTakenHand);
				SetFingerPos(bRighHand,EFingersName::Middle,FingersSocket.MiddleFinger, PhysicsHandComp, ObjectTakenHand);
				SetFingerPos(bRighHand,EFingersName::Pinky,FingersSocket.PinkyFinger, PhysicsHandComp, ObjectTakenHand);
				SetFingerPos(bRighHand,EFingersName::Ring,FingersSocket.RingFinger, PhysicsHandComp, ObjectTakenHand);
		}
	}
}

void ATWHandVRPhysics::GrabReleasedPhysics()
{
		
	GrabbingConstraintComp->BreakConstraint();
	bObjectInHand = false;
}


void ATWHandVRPhysics::SetFingerPos(bool isRight,EFingersName FingerEum,FName FingerSocket, USkeletalMeshComponent* PhysicsHand, ATWObjectTaken* Object)
{
	FVector Start = PhysicsHand->GetSocketTransform(FingerSocket).GetLocation();
	FVector End = PhysicsHand->GetSocketTransform(FingerSocket).GetLocation() + (PhysicsHand->GetSocketTransform(FingerSocket).GetRotation().GetForwardVector() * 20);
	
	TArray<AActor*> ActorsToIgnore; ActorsToIgnore.Add(GetOwner());
	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody) };
	const TArray<AActor*> ActorIgnore;
	FHitResult HitResult;

	EDrawDebugTrace::Type DrawDebugType;
	if(bDebug)
	{
		DrawDebugType = EDrawDebugTrace::ForDuration;
	} else {DrawDebugType = EDrawDebugTrace::None;}
	
	bool Hit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, ObjectTypes, false, ActorIgnore, DrawDebugType,HitResult,true,  FLinearColor::Red,  FLinearColor::Green,5.0f);
	if(Hit)
	{
		DrawDebugSphere(GetWorld(), HitResult.Location, 1.0f, 6, FColor::Green, true,5.0f);
		Object = Cast<ATWObjectTaken>(HitResult.GetActor());
		if(Object)
		{
			UStaticMeshComponent* FingerMesh = nullptr;
				switch (FingerEum) {
				case 0:
					FingerMesh =isRight ? Object->RightFingerMeshesPos.FingerThumbPosMesh:Object->LeftFingerMeshesPos.FingerThumbPosMesh ;
					ObjectTakenHand = Object;
					break;
				case 1:
					FingerMesh =isRight ? Object->RightFingerMeshesPos.FingerIndexPosMesh:Object->LeftFingerMeshesPos.FingerIndexPosMesh ;
					ObjectTakenHand = Object;
					break;
				case 2:
					FingerMesh =isRight ? Object->RightFingerMeshesPos.FingerMiddlePosMesh:Object->LeftFingerMeshesPos.FingerMiddlePosMesh ;
					ObjectTakenHand = Object;
					break;
				case 3:
					FingerMesh =isRight ? Object->RightFingerMeshesPos.FingerPinkyPosMesh:Object->LeftFingerMeshesPos.FingerPinkyPosMesh ;
					ObjectTakenHand = Object;
					break;
				case 4:
					FingerMesh =isRight ? Object->RightFingerMeshesPos.FingerRingPosMesh:Object->LeftFingerMeshesPos.FingerRingPosMesh ;
					ObjectTakenHand = Object;
					break;
				}
			if(FingerMesh)
			{
				FingerMesh->SetWorldLocation(HitResult.Location,false, new FHitResult, ETeleportType::TeleportPhysics);
			}
		}
	}
}

void ATWHandVRPhysics::BeginPlay()
{
	Super::BeginPlay();
	
	PhysicsConstraintComp->SetConstrainedComponents(PhysicsHandComp, "None",HandComp , "None");
	PhysicsHandComp->SetSimulatePhysics(true);
	
}

void ATWHandVRPhysics::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(!PhysicsHandComp->IsAnyRigidBodyAwake())
	{
		PhysicsHandComp->WakeRigidBody();
	}

}
