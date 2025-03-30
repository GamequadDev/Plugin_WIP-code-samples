// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VR/Hand/TWHandVR.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "TWHandVRPhysics.generated.h"

/**
 * 
 */
UCLASS()
class TWFRAMEWORK_API ATWHandVRPhysics : public ATWHandVR
{
	GENERATED_BODY()

public:

	ATWHandVRPhysics(const class FObjectInitializer& ObjectInitializer);
	
	virtual void Tick(float DeltaTime) override;

	void GrabPressedPhysics();
	void GrabReleasedPhysics();

	void SetFingerPos(bool isRight,EFingersName FingerEum,FName FingerSocket, USkeletalMeshComponent* PhysicsHand, ATWObjectTaken* Object);
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* PhysicsHandComp = nullptr;
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	UPhysicsConstraintComponent* PhysicsConstraintComp;
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	UPhysicsConstraintComponent* GrabbingConstraintComp;

protected:
	virtual void BeginPlay() override;
	
};
