// Copyright (C) GAMEQUAD - All Rights Reserved


#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "TWUIManagerVR.generated.h"

UCLASS()
class TWFRAMEWORK_API ATWUIManagerVR : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATWUIManagerVR(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="TWF VR")
	USceneComponent* BasicSceneComp = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="TWF VR")
	UWidgetComponent* BasicWidgetComp = nullptr;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};