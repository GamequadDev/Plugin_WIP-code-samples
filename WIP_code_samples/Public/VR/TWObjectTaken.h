// Copyright (C) GAMEQUAD - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "TWObjectTaken.generated.h"

USTRUCT(BlueprintType)
struct FFingerMesh
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FingerThumbPosMesh = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FingerMiddlePosMesh = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FingerPinkyPosMesh = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FingerRingPosMesh = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FingerIndexPosMesh = nullptr;
	
};

UCLASS()
class TWFRAMEWORK_API ATWObjectTaken : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATWObjectTaken(const class FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		FFingerMesh LeftFingerMeshesPos;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		FFingerMesh RightFingerMeshesPos;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* StaticMesh = nullptr;
	

};
