// Copyright (C) GAMEQUAD - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Anim/TWAnimInstanceHandVR.h"
#include "MotionControllerComponent.h"
#include "NiagaraComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "VR/TWObjectTaken.h"
#include "Actor/TWActor.h"
#include "TWHandVR.generated.h"


USTRUCT(BlueprintType)
struct FFingersSocket
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TW VR")
	FName ThumbFinger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Finger")
	FName IndexFinger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Finger")
	FName MiddleFinger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Finger")
	FName PinkyFinger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Finger")
	FName RingFinger;
	
};

UENUM()
enum EFingersName
{
	Thumb,
	Index,
	Middle,
	Ring,
	Pinky
};

UENUM(BlueprintType)
enum class EHandState : uint8 {
	None       UMETA(DisplayName="None"),
	Idle        UMETA(DisplayName="Idle"),
	Point        UMETA(DisplayName="Point"),
	Grab         UMETA(DisplayName="Grab"),
	Point_UI        UMETA(DisplayName="Point_UI"),
};

UCLASS()
class TWFRAMEWORK_API ATWHandVR : public ATWActor
{
	GENERATED_BODY()
	
public:

	ATWHandVR(const class FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	
	virtual void GrabPressed();
	virtual void GrabReleased();
	virtual void TriggerGrabPressed();
	virtual void TriggerGrabReleased();
	/** 
		* Grab item by distance.
		*                      
		*/
	virtual void GrabByDistance();
	
	UFUNCTION()
	AActor* GetTheNearestObjectFromThis(TArray<AActor*> Array);

	UFUNCTION()
	AActor* GetTraceObjectByDistance() const {return ObjectTraceByDistance;};
	
	UFUNCTION()
	float GetDistanceToTraceObjectByDistance() const {return DistanceFromObject;};

	UFUNCTION()
	USceneComponent* GetPlaceForGrabDistanceObject() const {return PlaceForGrabDistanceObject;};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	bool bDebug = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* MotionControllerComp;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* HandComp = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	UWidgetInteractionComponent* WidgetInteractionComp = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	FFingersSocket FingersSocket;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* VROrigin;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	UTWAnimInstanceHandVR* AnimInstanceHand = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	bool bObjectInHand = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	bool bTriggerGrab = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	ATWObjectTaken* ObjectTakenHand = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	AActor* ObjectActorInHand = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	AActor* ChildOffObjectActorInHand = nullptr;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	bool bRighHand = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EHandState eHandState = EHandState::Idle;

protected:
	
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent*  PlaceForGrabDistanceObject;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		float AngleRot = 60;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		float SphereTraceRadius = 6;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		AActor* ObjectTraceByDistance = nullptr;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		float MinDistanceFromObjectAnim = 80;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		bool bDistanceLastCheck = false;
	
	UPROPERTY()
	AActor* LastObjectTraceByDistance = nullptr;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		bool bIsObjectTraceByDistance = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		float DistanceFromObject = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	float SphereRadiusObjectTraceByDistance = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* PointerNiagaraParticle = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* PointerNiagaraAssetNormal = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* PointerNiagaraAssetHold = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PointerMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		float DefaultEndLocationPointerParticle = 150.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	float MinDistancePointerMesh = 40;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	float SurfaceZShiftPointerMesh = 2;

	/** 
	* Setup var relate to grab by distance.
	*                      
	*/
	
	virtual void SetupTraceObjectByDistance(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, FHitResult& HitResult);


	/** 
	* Forward trace objects by sphere.
	*
	* @return                      Returns true on collision.
	*/
	
	virtual bool ForwardHandTraceByDistance(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, FHitResult& HitArray);

	/** 
	* Set end location for pointer particle.
	*
	*/
	
	virtual void EndLocationPointerParticle(FVector End);

	virtual void HandleInteractivePointObj(bool bStart,AActor* Actor,float Distance);
	
	virtual void SetActorToObjectInHand(AActor& Actor);
	virtual bool AttachItemToHand(AActor* Actor);
	virtual bool TraceMultiObjectsBySphereInHand(TArray<FHitResult>& HitResults);
	virtual AActor* CheckTraceObjects(TArray<FHitResult>& HitArray);
	virtual bool ConditionForCheckTraceObject(FHitResult& HitResult);

	bool ChangeNiagaraParticleAsset(UNiagaraComponent* Comp, UNiagaraSystem* Asset);

	/** 
	* SphereTraceSingleForObjects but with debug and ignore self.
	*
	* @return                       Bool is has collision.
	*/
	virtual bool DefaultSphereTraceSingle(const FVector& Start,const FVector& End, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, FHitResult& HitArray, float Radius, const bool = true);
	/** 
	* LineTraceSingleForObjects but with debug.
	*
	* @return                       Bool is has collision.
	*/
	virtual bool DefaultLineTrace(const FVector& Start,const FVector& End, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, FHitResult& HitResult);
	
};
