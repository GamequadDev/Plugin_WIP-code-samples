// Copyright (C) GAMEQUAD - All Rights Reserved


#include "VR/UI/TWUIManagerVR.h"

// Sets default values
ATWUIManagerVR::ATWUIManagerVR(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BasicSceneComp = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("BasicSceneComp"));
	BasicSceneComp->SetupAttachment(RootComponent);

	BasicWidgetComp = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("BasicWidgetComp"));
	BasicWidgetComp->SetupAttachment(BasicSceneComp);
}

// Called when the game starts or when spawned
void ATWUIManagerVR::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATWUIManagerVR::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

