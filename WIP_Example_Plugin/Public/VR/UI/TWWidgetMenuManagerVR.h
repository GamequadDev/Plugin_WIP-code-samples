// Copyright (C) GAMEQUAD - All Rights Reserved


#pragma once

#include "CoreMinimal.h"
#include "VR/Actor/TWWidgetActorVR.h"
#include "VR/UI/TWUIManagerVR.h"
#include "TWWidgetMenuManagerVR.generated.h"

/**
 * 
 */
UCLASS()
class TWFRAMEWORK_API ATWWidgetMenuManagerVR : public ATWUIManagerVR
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATWWidgetMenuManagerVR(const FObjectInitializer& ObjectInitializer);

	UUserWidget* GetMiddleDisplayWidget(){return MiddleDisplay->WidgetComp->GetWidget();};


protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="VR")
	ATWWidgetActorVR* MiddleDisplay = nullptr; 
	
};
