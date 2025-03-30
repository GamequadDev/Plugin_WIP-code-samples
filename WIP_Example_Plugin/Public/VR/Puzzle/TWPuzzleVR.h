// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/TWActor.h"
#include "TWPuzzleVR.generated.h"

/**
 * 
 */
UCLASS()
class TWFRAMEWORK_API ATWPuzzleVR : public ATWActor
{
	GENERATED_BODY()


public:

	static AActor* GetPuzzleTopAttachParent(AActor* LocalActor);
	
};
