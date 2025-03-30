// Copyright (C) GAMEQUAD - All Rights Reserved


#include "VR/Pawn/TWVRPawnPuzzle.h"

//#include "TWPuzzleVRBasicMould.h"

ATWVRPawnPuzzle::ATWVRPawnPuzzle(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void ATWVRPawnPuzzle::BeginPlay()
{
	Super::BeginPlay();
}

//----------------------------------------------------------------------//
// Movement 
//----------------------------------------------------------------------//
void ATWVRPawnPuzzle::MoveForward(float value)
{
	Super::MoveForward(value);
}

void ATWVRPawnPuzzle::MoveRight(float value)
{
	Super::MoveRight(value);
}

void ATWVRPawnPuzzle::CharacterRot(float value)
{
	if(abs(value) < 0.1f)
	{
		CanRot = true;
	}
	
	if(RightControllerActor->ObjectActorInHand == nullptr)
	{
		if(abs(value) > 0.9f && CanRot)
		{
			CanRot = false;
			FRotator AddRot;
			FVector DevicePos;
			//UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(DeviceRot, DevicePos);
			float RotYaw = (round(value) * AngleRot);
			AddRot.Yaw = RotYaw;
			AddActorLocalRotation(AddRot);
			//UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition(RotYaw,  EOrientPositionSelector::Orientation);
		}
	}
	else
	{
		if(abs(value) > 0)
		{
			if(RightControllerActor->ObjectActorInHand)
			{
				AActor* Actor = RightControllerActor->ObjectActorInHand; 
				//FRotator NewRotator = Actor->GetActorRotation();
				FRotator NewLocalRotator;
				NewLocalRotator.Yaw = FSpeedObjectRotation * value;
				//Origin.Distance()
				//NewRotator.Yaw = Actor->GetActorRotation().Yaw + value * FSpeedRotation;
				RightControllerActor->ObjectActorInHand->AddActorWorldRotation(NewLocalRotator);
				//RightControllerActor->ObjectActorInHand->SetActorRotation(NewRotator);
			}
			
		}
	}
	
	
}

void ATWVRPawnPuzzle::RightControllerAxisY(float value)
{
	/*
	if(abs(value) > 0)
	{
		if(RightControllerActor->ObjectActorInHand)
		{
			//FRotator NewRotator = Actor->GetActorRotation();
			FRotator NewLocalRollRotator;
			NewLocalRollRotator.Roll = FSpeedObjectRotation * value;
			//Origin.Distance()
			//NewRotator.Yaw = Actor->GetActorRotation().Yaw + value * FSpeedRotation;
			RightControllerActor->ObjectActorInHand->AddActorLocalRotation(NewLocalRollRotator);
			
			//RightControllerActor->ObjectActorInHand->SetActorRotation(NewRotator);
		}
	}*/
}

//----------------------------------------------------------------------//
// Grab item 
//----------------------------------------------------------------------//
void ATWVRPawnPuzzle::GrabLeftPressed()
{
	Super::GrabLeftPressed();
}

void ATWVRPawnPuzzle::GrabLeftReleased()
{
	Super::GrabLeftReleased();
}

void ATWVRPawnPuzzle::GrabRightPressed()
{
	Super::GrabRightPressed();
}

void ATWVRPawnPuzzle::GrabRightReleased()
{
	Super::GrabRightReleased();
}

void ATWVRPawnPuzzle::TriggerLeftGrabPressed()
{
	Super::TriggerLeftGrabPressed();
}

void ATWVRPawnPuzzle::TriggerLeftGrabReleased()
{
	Super::TriggerLeftGrabReleased();
}

void ATWVRPawnPuzzle::TriggerRightGrabPressed()
{
	Super::TriggerRightGrabPressed();
}

void ATWVRPawnPuzzle::TriggerRightGrabReleased()
{
	Super::TriggerRightGrabReleased();
}

void ATWVRPawnPuzzle::DistanceGrab()
{
	/*
	AActor* Actor = RightControllerActor->GetTraceObjectByDistance();
	float Distance = 0.0f;
	AActor* ActorMould = nullptr;
	if(Actor)
	{
		ActorMould = Cast<ATWPuzzleVRBasicMould>(Actor->GetParentActor());
	}
	
	if(Actor && !ActorMould)
	{
			const FVector Location = RightControllerActor->GetPlaceForGrabDistanceObject()->GetComponentLocation();
			Actor->SetActorLocation(Location);
			GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Black, FString::Printf(TEXT("Hit")));
	}*/

}

void ATWVRPawnPuzzle::GrabByDistanceLeftHand()
{
	LeftControllerActor->GrabByDistance();
}

void ATWVRPawnPuzzle::GrabByDistanceRightHand()
{
	RightControllerActor->GrabByDistance();
	/*
	AActor* Actor = RightControllerActor->GetTraceObjectByDistance();
	float Distance = 0.0f;
	AActor* ActorMould = nullptr;
	if(Actor)
	{
		ActorMould = Cast<ATWPuzzleVRBasicMould>(Actor->GetAttachParentActor());
	}
	
	if(Actor && !ActorMould)
	{
		const FVector Location = RightControllerActor->GetPlaceForGrabDistanceObject()->GetComponentLocation();
		Actor->SetActorLocation(Location);
		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Black, FString::Printf(TEXT("Hit")));
	}*/
}

void ATWVRPawnPuzzle::GameMenu()
{
	UTWGameInstanceVRPuzzle * GameInstance = Cast<UTWGameInstanceVRPuzzle>(GetGameInstance());
	if(GameInstance)
	{
		GameInstance->TWFPuzzleVR_PutTogetherPuzzle();
	}
	
}



//----------------------------------------------------------------------//
// Input 
//----------------------------------------------------------------------//

