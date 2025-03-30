// Copyright (C) GAMEQUAD - All Rights Reserved


#pragma once

#include "CoreMinimal.h"
#include "Actor/TWWidgetActor.h"
#include "Components/WidgetComponent.h"
#include "TWWidgetActorVR.generated.h"

UCLASS()
class TWFRAMEWORK_API ATWWidgetActorVR : public ATWWidgetActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATWWidgetActorVR(const class FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="TW VR")
	UWidgetComponent* WidgetComp;

protected:
	// Called when the game starts or when spawned
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="TW VR")
	USceneComponent* SceneRootComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
