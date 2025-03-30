// Copyright (C) GAMEQUAD - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "VR/Character/TWCharacterVR.h"
#include "VR/Pawn/TWVRPawn.h"
#include "Game/TWGameController.h"
#include "TWGameControllerVR.generated.h"

/**
 * 
 */
UCLASS()
class TWFRAMEWORK_API ATWGameControllerVR : public ATWGameController
{
	GENERATED_BODY()


public:


protected:
	
	ATWGameControllerVR(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "TWF Game")
		void PossesToVRPlayer();
	
	UPROPERTY(NoClear,BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	ATWCharacterVR* PlayerVr = nullptr;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	ATWVRPawn* PawnVr = nullptr;


	
};
