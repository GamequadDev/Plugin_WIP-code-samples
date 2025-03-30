// Copyright (C) GAMEQUAD - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "VR/Hand/TWHandVR.h"
#include "TWHandVRPuzzle.generated.h"

UCLASS()
class TWFRAMEWORK_API ATWHandVRPuzzle : public ATWHandVR
{
	GENERATED_BODY()
public:

	UFUNCTION()
	void HandleGrabByDistance(){this->GrabByDistance();};
	
protected:
	
	ATWHandVRPuzzle(const class FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	
	virtual bool AttachItemToHand(AActor* Actor) override;
	virtual bool TraceMultiObjectsBySphereInHand(TArray<FHitResult>& HitResults) override;
	virtual void GrabPressed() override;
	virtual void GrabReleased() override;
	virtual void TriggerGrabPressed() override;
	virtual void TriggerGrabReleased() override;

	virtual void GrabByDistance() override;
	virtual void SetActorToObjectInHand(AActor& Actor) override;

	virtual AActor* CheckTraceObjects(TArray<FHitResult>& HitArray) override;
	virtual void HandleInteractivePointObj(bool bStart,AActor* Actor,float Distance);

	UPROPERTY()
	AActor*	LastActorGrabByDistance = nullptr;
	
};
