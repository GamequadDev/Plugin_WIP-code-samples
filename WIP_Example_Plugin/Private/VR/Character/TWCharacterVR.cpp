// Copyright (C) GAMEQUAD - All Rights Reserved


#include "VR/Character/TWCharacterVR.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "AI/NavigationSystemBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/EngineGlobals.h"
#include "NavigationSystem.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "VR/Hand/TWHandVRPhysics.h"

ATWCharacterVR::ATWCharacterVR(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	VROrigin = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("VROrigin"));
	VROrigin->SetupAttachment(RootComponent);
	
	CameraComp = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Camera"));
	CameraComp->SetupAttachment(VROrigin);

	TeleportTraceNiagaraComp = ObjectInitializer.CreateDefaultSubobject<UNiagaraComponent>(this, TEXT("TeleportPathNiagaraComp"));
	TeleportTraceNiagaraComp->SetupAttachment(RootComponent);

	MyCustomActor= CreateOptionalDefaultSubobject<UChildActorComponent>(TEXT("MyCustomActorName"));
	MyCustomActor->SetupAttachment(RootComponent);

}

void ATWCharacterVR::BeginPlay()
{
	Super::BeginPlay();
	
	bool bIsVR = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
	
	if (bIsVR)
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
	}

	if (bAutomaticSetVRMode)
	{
		bIsVR ? isVRMode = true : isVRMode = false;
		
	}
	
	if (!isVRMode)
	{
		VROrigin->SetRelativeLocation(HeighOriginNoVR);
	}
	if (bDebug)
	{
		RootComponent->SetHiddenInGame(false);
	}

	if (LeftActorControllerToSpawn)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		LeftControllerActor = GetWorld()->SpawnActor<ATWHandVRPhysics>(LeftActorControllerToSpawn, FVector::ZeroVector,
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
		RightControllerActor = GetWorld()->SpawnActor<ATWHandVRPhysics>(RightActorControllerToSpawn, FVector::ZeroVector,
		                                                         FRotator::ZeroRotator, SpawnParameters);
		FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(
			EAttachmentRule::KeepRelative, true);
		AttachmentTransformRules.LocationRule = EAttachmentRule::KeepRelative;
		AttachmentTransformRules.RotationRule = EAttachmentRule::KeepRelative;
		AttachmentTransformRules.ScaleRule = EAttachmentRule::KeepWorld;
		RightControllerActor->AttachToComponent(VROrigin, AttachmentTransformRules);
		RightControllerActor->bDebug = bDebug;
	}

}

void ATWCharacterVR::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!bResetHeightController)
	{
		if(InternalIntGateMulti == 4)
		{
			if(LeftControllerActor)
			{
				const FVector LocLeftHand =  {LeftControllerActor->GetActorLocation().X, LeftControllerActor->GetActorLocation().Y, VROrigin->GetRelativeLocation().Z + GetActorLocation().Z};
				LeftControllerActor->SetActorLocation(LocLeftHand);
			}
			
			if(RightControllerActor)
			{
				const FVector LocRightHand =  {RightControllerActor->GetActorLocation().X, RightControllerActor->GetActorLocation().Y, VROrigin->GetRelativeLocation().Z + GetActorLocation().Z};
				RightControllerActor->SetActorLocation(LocRightHand);
			}
			
			bResetHeightController = true;
		}
		++InternalIntGateMulti;
		return;
	}
	
	const FVector CameraOffset = CameraComp->GetComponentLocation()-GetActorLocation();
	
	GetMovementComponent()->MoveUpdatedComponent( {CameraOffset.X, CameraOffset.Y, 0.0f},GetActorRotation(),true);
	VROrigin->AddWorldOffset( {CameraOffset.X *-1, CameraOffset.Y *-1, 0.0f});

	if(LeftControllerActor)
	{
		LeftControllerActor->AddActorWorldOffset({CameraOffset.X * -1, CameraOffset.Y * -1, 0.0f});
	}
	if(RightControllerActor)
	{
		RightControllerActor->AddActorWorldOffset({CameraOffset.X * -1, CameraOffset.Y * -1, 0.0f});
	}
}
//----------------------------------------------------------------------//
// Grab Item System 
//----------------------------------------------------------------------//

void ATWCharacterVR::GrabLeftPressed()
{

	if(LeftControllerActor)
	{
		LeftControllerActor->GrabPressedPhysics();
		LeftControllerActor->WidgetInteractionComp->PressPointerKey("LeftMouseButton");
	}
	
}

void ATWCharacterVR::GrabLeftReleased()
{
	if(LeftControllerActor)
	{
		LeftControllerActor->GrabReleasedPhysics();
		LeftControllerActor->WidgetInteractionComp->ReleasePointerKey("LeftMouseButton");
	}
}
void ATWCharacterVR::GrabRightPressed()
{
	if(RightControllerActor)
	{
		RightControllerActor->GrabPressedPhysics();
		RightControllerActor->WidgetInteractionComp->PressPointerKey("LeftMouseButton");
	}
}

void ATWCharacterVR::GrabRightReleased()
{
	if(RightControllerActor)
	{
		RightControllerActor->GrabReleasedPhysics();
		RightControllerActor->WidgetInteractionComp->ReleasePointerKey("LeftMouseButton");
	}
}

//----------------------------------------------------------------------//
// Movement 
//----------------------------------------------------------------------//

void ATWCharacterVR::MoveRight(float value)
{
	if(Controller && abs(value) >= 0.15f)
	{
		const FVector Direction = FRotationMatrix(FRotator(0.f, CameraComp->GetComponentRotation().Yaw, 0.f)).GetUnitAxis((EAxis::Y));
		AddMovementInput(Direction, SpeedSensitivity*value);
	}
}



void ATWCharacterVR::MoveForward(float value)
{
	if(Controller && abs(value) >= 0.15f)
	{
		const FVector Direction = FRotationMatrix(FRotator(0.f, CameraComp->GetComponentRotation().Yaw, 0.f)).GetUnitAxis((EAxis::X));
		AddMovementInput(Direction, SpeedSensitivity* value);
	}
}

//----------------------------------------------------------------------//
// Rotation 
//----------------------------------------------------------------------//

void ATWCharacterVR::CharacterRot(float value)
{
	if(abs(value) < 0.1f)
	{
		CanRot = true;
	}
	if(abs(value) > 0.9f && CanRot)
	{
		CanRot = false;
		const float YawRotation = (round(value) * AngleRot);
		FRotator DeltaRotCharacter; DeltaRotCharacter.Roll = 0; DeltaRotCharacter.Pitch = 0; DeltaRotCharacter.Yaw = YawRotation;
		FHitResult* OutSweepHitResultLocal = nullptr;
		
		VROrigin->AddWorldRotation(DeltaRotCharacter,false, OutSweepHitResultLocal, ETeleportType::TeleportPhysics);
		
	}
}

//----------------------------------------------------------------------//
// Teleport 
//----------------------------------------------------------------------//

void ATWCharacterVR::AxisRightY(float value)
{
	if(abs(value) >= 0.7f && value > 0.0)
	{
		if(!DoOnceTeleportTrace)
		{
			DoOnceTeleportTrace = true;
			bTeleportTrace = true;
			
			TeleportTraceNiagaraComp->SetVisibility(true);
			
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = this;
			
			TeleportVisualizer = GetWorld()->SpawnActor<AActor>(ToSpawnTeleportVisualizer,FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
		}
		FXRMotionControllerData RMotionContrData;
		UHeadMountedDisplayFunctionLibrary::GetMotionControllerData(GetWorld(),EControllerHand::Right, RMotionContrData);
		if(RMotionContrData.bValid)
		{
			TeleportTrace(RMotionContrData.AimPosition,RMotionContrData.AimRotation);
		}
	}
	else
	{
		if(bTeleportTrace)
		{
			bTeleportTrace = false;
		
			if(TeleportVisualizer)
			{
				TeleportVisualizer->Destroy();
			}
		
			TeleportTraceNiagaraComp->SetVisibility(false, false);
		
			if(bValidTeleportLoc)
			{
				const FRotator Rot(0, 0, GetActorRotation().Yaw);
				FVector DestLoc = ProjectedTeleportLoc - Rot.RotateVector(FVector{
					CameraComp->GetRelativeLocation().X, CameraComp->GetRelativeLocation().Y, 0.0f
				});

				this->TeleportTo(DestLoc, FRotator{0, 0, GetActorRotation().Yaw});
			}
		}
		DoOnceTeleportTrace = false;
	}
}

bool ATWCharacterVR::IsValidLocationInNavigation(FHitResult& HitResult, FVector& ProjectedLocation)
{
	
	ANavigationData* NavData = nullptr;
	FSharedConstNavQueryFilter QueryFilter = nullptr;
	TSubclassOf<UNavigationQueryFilter> FilterClass = nullptr;
	bool bOnNavigation = UNavigationSystemV1::K2_ProjectPointToNavigation(GetWorld(),HitResult.Location,ProjectedLocation, NavData, FilterClass, FVector::ZeroVector);
	return bOnNavigation; 
}

void ATWCharacterVR::TeleportTrace(FVector StartPos, FQuat AnimRotation)
{
	TArray<TEnumAsByte<EObjectTypeQuery> > ObTypes { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic) };
	TArray<AActor*> AToIgnore;
			
	AToIgnore.Add(GetOwner());
	if(LeftControllerActor){AToIgnore.Add(LeftControllerActor);}
	if(RightControllerActor) {AToIgnore.Add(RightControllerActor);}
			
	FHitResult HitResult;
	TArray<FVector> OutPathPositions; 
	FVector OutLastTraceDestination;
	FVector LaunchVelocity = AnimRotation.GetForwardVector() * LaunchSpeedTeleport;
			
	bool Hit = UGameplayStatics::Blueprint_PredictProjectilePath_ByObjectType(GetWorld(),HitResult, OutPathPositions,
		OutLastTraceDestination, StartPos, LaunchVelocity, true, LocalTeleportProjRadius, ObTypes, false, AToIgnore, EDrawDebugTrace::None, 1);
			
	if(bDebug)
	{
		UKismetSystemLibrary::DrawDebugCapsule(GetWorld(), HitResult.Location, 2,2,FRotator::ZeroRotator, FLinearColor::Blue, 2, 4);
	}
	FVector ProjectedLoc = FVector::ZeroVector;
	bValidTeleportLoc = IsValidLocationInNavigation( HitResult, ProjectedLoc);
	bValidTeleportLoc = bValidTeleportLoc &&  Hit;
	
	//GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Black, FString::Printf(TEXT("Bool: %s"), bValidTeleportLoc ? TEXT("true") : TEXT("false")));

	if(TeleportVisualizer)
	{
		TeleportVisualizer->GetRootComponent()->SetVisibility(bValidTeleportLoc, true);
		ProjectedTeleportLoc = {ProjectedLoc.X, ProjectedLoc.Y, ProjectedLoc.Z - NavMeshCellHeight};
		TeleportVisualizer->SetActorLocation(ProjectedTeleportLoc);
		//GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Blue, FString::Printf(TEXT("Teleport Exist: %s"), TeleportVisualizer ? TEXT("true") : TEXT("false")));
	}


	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(TeleportTraceNiagaraComp, "User.PointArray",OutPathPositions);
	
}



void ATWCharacterVR::GameMenu()
{






	
}

void ATWCharacterVR::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAction("GrabLeft", IE_Pressed, this, &ATWCharacterVR::GrabLeftPressed);
	InputComponent->BindAction("GrabLeft", IE_Released, this, &ATWCharacterVR::GrabLeftReleased);
	InputComponent->BindAction("GrabRight", IE_Pressed, this, &ATWCharacterVR::GrabRightPressed);
	InputComponent->BindAction("GrabRight", IE_Released, this, &ATWCharacterVR::GrabRightReleased);
	InputComponent->BindAction("GameMenuVR",IE_Released, this, &ATWCharacterVR::GameMenu);
	InputComponent->BindAxis("MovementAxisRight_X", this, &ATWCharacterVR::CharacterRot);
	InputComponent->BindAxis("MovementAxisLeft_X", this, &ATWCharacterVR::MoveRight);
	InputComponent->BindAxis("MovementAxisLeft_Y", this, &ATWCharacterVR::MoveForward);
	InputComponent->BindAxis("MovementAxisRight_Y", this, &ATWCharacterVR::AxisRightY);
}
