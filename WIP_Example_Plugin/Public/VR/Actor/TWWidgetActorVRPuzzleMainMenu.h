// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VR/Actor/TWWidgetActorVRPuzzle.h"
#include "TWWidgetActorVRPuzzleMainMenu.generated.h"


UCLASS()
class TWFRAMEWORK_API ATWWidgetActorVRPuzzleMainMenu : public ATWWidgetActorVRPuzzle
{
	GENERATED_BODY()

public:
	
	ATWWidgetActorVRPuzzleMainMenu(const class FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="TW VR")
	UWidgetComponent* LeftWidgetComp = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="TW VR")
	UWidgetComponent* RightWidgetComp = nullptr;

protected:
	// Called when the game starts or when spawned
	
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
