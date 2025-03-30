// Copyright (C) GAMEQUAD - All Rights Reserved


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "VR/Hand/TWHandVR.h"
#include "TWVRPawn.generated.h"

UCLASS()
class TWFRAMEWORK_API ATWVRPawn : public APawn
{
	GENERATED_BODY()

public:

	ATWVRPawn(const class FObjectInitializer& ObjectInitializer);



protected:

	virtual void BeginPlay() override;

	virtual void GrabLeftPressed();
	virtual void GrabLeftReleased();
	virtual void GrabRightPressed();
	virtual void GrabRightReleased();
	
	virtual void TriggerRightGrabPressed();
	virtual void TriggerRightGrabReleased();
	virtual void TriggerLeftGrabPressed();
	virtual void TriggerLeftGrabReleased();

	virtual void DistanceGrab();
	virtual void GrabByDistanceLeftHand(){};
	virtual void GrabByDistanceRightHand(){};
	
	virtual void MoveRight(float value);
	virtual void MoveForward(float value);
	virtual void RightControllerAxisY(float value);
	virtual void CharacterRot(float value);
	
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void GameMenu();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bGameMenu = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* WidgetComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		 UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USceneComponent* VROrigin;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USceneComponent* DefaultSceneRoot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		float SpeedSensitivity = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		bool CanRot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		float AngleRot = 60;
	
	float LenghtOfTrace = 20;
	float Radius = 20;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	bool bObjectInLeftHand = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	bool bObjectInRightHand = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	bool isVRMode = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	bool bDebug = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	FVector StartPositionLocation = FVector {0,0,0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	float AutomaticTeleportSphereSize = 1000;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	FVector HeighOriginNoVR = FVector {0,0,60};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	ATWObjectTaken* ObjectTakenRHand = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	ATWObjectTaken* ObjectTakenLHand = nullptr;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	ATWHandVR* LeftControllerActor;
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	ATWHandVR* RightControllerActor;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	UFloatingPawnMovement* FloatingPawnMovementComp;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<ATWHandVR> LeftActorControllerToSpawn;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<ATWHandVR> RightActorControllerToSpawn;

};


