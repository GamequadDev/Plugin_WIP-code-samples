// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VR/Pawn/TWVRPawn.h"
#include "VR/Game/TWGameInstanceVRPuzzle.h"
#include "TWVRPawnPuzzle.generated.h"

/**
 * 
 */
UCLASS()
class TWFRAMEWORK_API ATWVRPawnPuzzle : public ATWVRPawn
{
	GENERATED_BODY()


protected:
	
	virtual void BeginPlay() override;
	virtual void GrabLeftPressed() override;
	virtual void GrabLeftReleased() override;
	virtual void GrabRightPressed() override;
	virtual void GrabRightReleased() override;

	virtual void TriggerRightGrabPressed() override;
	virtual void TriggerRightGrabReleased() override;
	virtual void TriggerLeftGrabPressed() override;
	virtual void TriggerLeftGrabReleased() override;
	
	virtual void MoveRight(float value) override;
	virtual void MoveForward(float value) override;
	virtual void CharacterRot(float value) override;
	virtual void RightControllerAxisY(float value) override;

	virtual void DistanceGrab() override;
	virtual void GrabByDistanceLeftHand() override;
	virtual void GrabByDistanceRightHand() override;

	virtual void GameMenu() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VR")
	float FSpeedRotation = 6.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VR")
	float FSpeedObjectRotation = 2.0f;
public:
	ATWVRPawnPuzzle(const class FObjectInitializer& ObjectInitializer);
	
};