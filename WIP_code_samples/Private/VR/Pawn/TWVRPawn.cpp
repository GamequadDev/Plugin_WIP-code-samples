// Copyright (C) GAMEQUAD - All Rights Reserved

#include "VR/Pawn/TWVRPawn.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "IHeadMountedDisplay.h"
#include "Components/WidgetComponent.h"


ATWVRPawn::ATWVRPawn(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//PrimaryActorTick.bCanEverTick = false;
	
	DefaultSceneRoot = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("DefaultSceneRoot"));
	
	VROrigin = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("VROrigin"));
	VROrigin->SetupAttachment(DefaultSceneRoot);
	
	CameraComp = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Camera"));
	CameraComp->SetupAttachment(VROrigin);

	FloatingPawnMovementComp = ObjectInitializer.CreateDefaultSubobject<UFloatingPawnMovement>(this, TEXT("FloatingPawnMovementComponent"));

	WidgetComp = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("WidgetCmp"));
	WidgetComp->SetupAttachment(DefaultSceneRoot);
	WidgetComp->SetVisibility(false);
}

void ATWVRPawn::BeginPlay()
{
	Super::BeginPlay();
	if (LeftActorControllerToSpawn)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		LeftControllerActor = GetWorld()->SpawnActor<ATWHandVR>(LeftActorControllerToSpawn, FVector::ZeroVector,
																FRotator::ZeroRotator, SpawnParameters);
		FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(
			EAttachmentRule::KeepRelative, true);
		AttachmentTransformRules.LocationRule = EAttachmentRule::KeepRelative;
		AttachmentTransformRules.RotationRule = EAttachmentRule::KeepRelative;
		AttachmentTransformRules.ScaleRule = EAttachmentRule::KeepWorld;
		LeftControllerActor->AttachToComponent(VROrigin, AttachmentTransformRules);
		LeftControllerActor->bDebug = bDebug;
	}
	if (RightActorControllerToSpawn)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		RightControllerActor = GetWorld()->SpawnActor<ATWHandVR>(RightActorControllerToSpawn, FVector::ZeroVector,
																 FRotator::ZeroRotator, SpawnParameters);
		FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(
			EAttachmentRule::KeepRelative, true);
		AttachmentTransformRules.LocationRule = EAttachmentRule::KeepRelative;
		AttachmentTransformRules.RotationRule = EAttachmentRule::KeepRelative;
		AttachmentTransformRules.ScaleRule = EAttachmentRule::KeepWorld;
		RightControllerActor->AttachToComponent(VROrigin, AttachmentTransformRules);
		RightControllerActor->bDebug = bDebug;
	}
	StartPositionLocation = GetActorLocation();
}

//----------------------------------------------------------------------//
// Grab Item System 
//----------------------------------------------------------------------//

void ATWVRPawn::GrabLeftPressed()
{

	if (LeftControllerActor)
	{
		LeftControllerActor->GrabPressed();
		LeftControllerActor->WidgetInteractionComp->PressPointerKey("LeftMouseButton");
	}

}
void ATWVRPawn::GrabLeftReleased()
{
	if (LeftControllerActor)
	{
		LeftControllerActor->GrabReleased();
		LeftControllerActor->WidgetInteractionComp->ReleasePointerKey("LeftMouseButton");
	}
}
void ATWVRPawn::GrabRightPressed()
{
	if (RightControllerActor)
	{
		RightControllerActor->GrabPressed();
		RightControllerActor->WidgetInteractionComp->PressPointerKey("LeftMouseButton");
	}
}
void ATWVRPawn::GrabRightReleased()
{
	if (RightControllerActor)
	{
		RightControllerActor->GrabReleased();
		RightControllerActor->WidgetInteractionComp->ReleasePointerKey("LeftMouseButton");
	}
}
void ATWVRPawn::TriggerLeftGrabPressed()
{
	if (LeftControllerActor)
	{
		LeftControllerActor->TriggerGrabPressed();
		LeftControllerActor->WidgetInteractionComp->PressPointerKey("LeftMouseButton");
	}
}
void ATWVRPawn::TriggerLeftGrabReleased()
{
	if (LeftControllerActor)
	{
		LeftControllerActor->TriggerGrabReleased();
		LeftControllerActor->WidgetInteractionComp->ReleasePointerKey("LeftMouseButton");
	}
}
void ATWVRPawn::TriggerRightGrabPressed()
{
	if (RightControllerActor)
	{
		RightControllerActor->TriggerGrabPressed();
		RightControllerActor->WidgetInteractionComp->PressPointerKey("RightMouseButton");
	}
}
void ATWVRPawn::TriggerRightGrabReleased()
{
	if (RightControllerActor)
	{
		RightControllerActor->TriggerGrabReleased();
		RightControllerActor->WidgetInteractionComp->ReleasePointerKey("RightMouseButton");
	}
}
void ATWVRPawn::DistanceGrab()
{
	
}
//----------------------------------------------------------------------//
// Movement 
//----------------------------------------------------------------------//
void ATWVRPawn::MoveRight(float value)
{
	if(Controller && abs(value) >= 0.15f)
	{
		const FVector MovementRight = CameraComp->GetRightVector()  * (SpeedSensitivity*value);
		//FloatingPawnMovementComp->AddInputVector(MovementRight);
		const FVector Direction = FVector(CameraComp->GetRightVector().X,CameraComp->GetRightVector().Y,0);
		AddMovementInput(Direction, value * SpeedSensitivity);
	}
}

void ATWVRPawn::CharacterRot(float value)
{
	
	if(abs(value) < 0.1f)
	{
		CanRot = true;
	}
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

void ATWVRPawn::MoveForward(float value)
{
	if(FVector::Dist(StartPositionLocation, GetActorLocation()) > AutomaticTeleportSphereSize)
	{

		SetActorLocation(StartPositionLocation);
		
	}
	if(Controller && abs(value) >= 0.15f)
	{
		const FVector MovementForward = CameraComp->GetForwardVector()  * (SpeedSensitivity*value);
		//FloatingPawnMovementComp->AddInputVector(MovementForward);
		const FVector Direction = FVector(CameraComp->GetForwardVector().X,CameraComp->GetForwardVector().Y,0);
		AddMovementInput(Direction, value * SpeedSensitivity);
		//const FVector MovementForward = GetActorForwardVector()  * (SpeedSensitivity*value);
		//FloatingPawnMovementComp->AddInputVector(MovementForward);
	}
}

void ATWVRPawn::RightControllerAxisY(float value)
{
	
}
//----------------------------------------------------------------------//
// Menu 
//----------------------------------------------------------------------//
void ATWVRPawn::GameMenu()
{
	!bGameMenu ? WidgetComp->SetVisibility(true)  : WidgetComp->SetVisibility(false);
	bGameMenu = !bGameMenu;
}

//----------------------------------------------------------------------//
// Input 
//----------------------------------------------------------------------//
void ATWVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAction("GrabLeft", IE_Pressed, this, &ATWVRPawn::GrabLeftPressed);
	InputComponent->BindAction("GrabLeft", IE_Released, this, &ATWVRPawn::GrabLeftReleased);
	InputComponent->BindAction("GrabRight", IE_Pressed, this, &ATWVRPawn::GrabRightPressed);
	InputComponent->BindAction("GrabRight", IE_Released, this, &ATWVRPawn::GrabRightReleased);

	InputComponent->BindAction("TriggerLeft", IE_Pressed, this, &ATWVRPawn::TriggerLeftGrabPressed);
	InputComponent->BindAction("TriggerLeft", IE_Released, this, &ATWVRPawn::TriggerLeftGrabReleased);
	InputComponent->BindAction("TriggerRight", IE_Pressed, this, &ATWVRPawn::TriggerRightGrabPressed);
	InputComponent->BindAction("TriggerRight", IE_Released, this, &ATWVRPawn::TriggerRightGrabReleased);
	
	//InputComponent->BindAction("DistanceGrab", IE_Pressed, this,  &ATWVRPawn::DistanceGrab);
	
	InputComponent->BindAction("GrabByDistanceLeft", IE_Pressed, this,  &ATWVRPawn::GrabByDistanceLeftHand);
	InputComponent->BindAction("GrabByDistanceRight", IE_Pressed, this,  &ATWVRPawn::GrabByDistanceRightHand);
	
	InputComponent->BindAction("GameMenuVR", IE_Pressed, this, &ATWVRPawn::GameMenu);
	
	InputComponent->BindAxis("MovementAxisRight_X", this, &ATWVRPawn::CharacterRot);
	InputComponent->BindAxis("MovementAxisRight_Y", this, &ATWVRPawn::RightControllerAxisY);
	InputComponent->BindAxis("MovementAxisLeft_X", this, &ATWVRPawn::MoveRight);
	InputComponent->BindAxis("MovementAxisLeft_Y", this, &ATWVRPawn::MoveForward);
	
	
}

