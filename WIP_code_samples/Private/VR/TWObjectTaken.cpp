// Copyright (C) GAMEQUAD - All Rights Reserved


#include "VR/TWObjectTaken.h"
#include "VR/Character/TWCharacterVR.h"

// Sets default values
ATWObjectTaken::ATWObjectTaken(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
	StaticMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	
	RightFingerMeshesPos.FingerThumbPosMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("RFingerThumbPosMesh"));
	RightFingerMeshesPos.FingerThumbPosMesh->SetupAttachment(StaticMesh);

	RightFingerMeshesPos.FingerIndexPosMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("RFingerIndexPosMesh"));
	RightFingerMeshesPos.FingerIndexPosMesh->SetupAttachment(StaticMesh);

	RightFingerMeshesPos.FingerMiddlePosMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("RFingerMiddlePosMesh"));
	RightFingerMeshesPos.FingerMiddlePosMesh->SetupAttachment(StaticMesh);

	RightFingerMeshesPos.FingerPinkyPosMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("RFingerPinkyPosMesh"));
	RightFingerMeshesPos.FingerPinkyPosMesh->SetupAttachment(StaticMesh);

	RightFingerMeshesPos.FingerRingPosMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("RFingerRingPosMesh"));
	RightFingerMeshesPos.FingerRingPosMesh->SetupAttachment(StaticMesh);

		
	LeftFingerMeshesPos.FingerThumbPosMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("LFingerThumbPosMesh"));
	LeftFingerMeshesPos.FingerThumbPosMesh->SetupAttachment(StaticMesh);

	LeftFingerMeshesPos.FingerIndexPosMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("LFingerIndexPosMesh"));
	LeftFingerMeshesPos.FingerIndexPosMesh->SetupAttachment(StaticMesh);

	LeftFingerMeshesPos.FingerMiddlePosMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("LFingerMiddlePosMesh"));
	LeftFingerMeshesPos.FingerMiddlePosMesh->SetupAttachment(StaticMesh);

	LeftFingerMeshesPos.FingerPinkyPosMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("LFingerPinkyPosMesh"));
	LeftFingerMeshesPos.FingerPinkyPosMesh->SetupAttachment(StaticMesh);

	LeftFingerMeshesPos.FingerRingPosMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("LFingerRingPosMesh"));
	LeftFingerMeshesPos.FingerRingPosMesh->SetupAttachment(StaticMesh);
	
}

// Called when the game starts or when spawned
void ATWObjectTaken::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATWObjectTaken::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
