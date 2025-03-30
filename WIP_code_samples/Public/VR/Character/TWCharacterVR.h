// Copyright (C) GAMEQUAD - All Rights Reserved

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "VR/Hand/TWHandVR.h"
#include "VR/TWObjectTaken.h"
#include "NiagaraComponent.h"
#include "NavigationSystem.h"
#include "VR/Hand/TWHandVRPhysics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TWCharacterVR.generated.h"

UCLASS()
class TWFRAMEWORK_API ATWCharacterVR : public ACharacter
{
	GENERATED_BODY()

public:
	ATWCharacterVR(const class FObjectInitializer& ObjectInitializer);
protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

private:

	void GrabLeftPressed();
	void GrabLeftReleased();
	void GrabRightPressed();
	void GrabRightReleased();

	void GameMenu();

	bool IsValidLocationInNavigation(FHitResult& HitResult,FVector& ProjectedLocation);
	void TeleportTrace(FVector StartPos, FQuat AnimRotation);
	void MoveRight(float value);
	void MoveForward(float value);
	void CharacterRot(float value);
	void AxisRightY(float value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* VROrigin;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	UNiagaraComponent* TeleportTraceNiagaraComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		float SpeedSensitivity = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		float AngleRot = 60;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		float Radius = 6;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		bool bObjectInLeftHand = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		bool bObjectInRightHand = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		bool isVRMode = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		bool bDebug = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		FVector HeighOriginNoVR = FVector {0,0,60};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		ATWObjectTaken* ObjectTakenRHand = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		ATWObjectTaken* ObjectTakenLHand = nullptr;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
		ATWHandVRPhysics* LeftControllerActor;
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
		ATWHandVRPhysics* RightControllerActor;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
		TSubclassOf<ATWHandVR> LeftActorControllerToSpawn;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<ATWHandVR> RightActorControllerToSpawn;

	

private:
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	float TeleportRange = 500;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	bool bAutomaticSetVRMode = true;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	float LocalTeleportProjRadius = 3.6;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	float LaunchSpeedTeleport = 650;

	UPROPERTY(NoClear,BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	AActor* TeleportVisualizer = nullptr;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<AActor> ToSpawnTeleportVisualizer = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyCustomActor", meta = (AllowPrivateAccess = "true"))
	class UChildActorComponent* MyCustomActor;
	
	
	//	Local //
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	float NavMeshCellHeight = 8.0;

	FVector ProjectedTeleportLoc;
	
	float LenghtOfTrace = 20;
	float LenghtTraceFingerToObj = 20;


	
	bool CanRot;
	
	// Teleport
	bool DoOnceTeleportTrace = false;
	bool bTeleportTrace;
	bool bValidTeleportLoc = false;
	
	bool bResetHeightController = false;
	int InternalIntGateMulti = 0;
	
	
}; 

