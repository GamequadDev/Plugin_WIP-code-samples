// Copyright (C) GAMEQUAD - All Rights Reserved



#include "VR/Actor/TWWidgetActorVR.h"

// Sets default values
ATWWidgetActorVR::ATWWidgetActorVR(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComp = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("SceneRootComp"));
	SetRootComponent(SceneRootComp);
	
	WidgetComp = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("Widget"));
	WidgetComp->SetupAttachment(SceneRootComp);

}

// Called when the game starts or when spawned
void ATWWidgetActorVR::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATWWidgetActorVR::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

