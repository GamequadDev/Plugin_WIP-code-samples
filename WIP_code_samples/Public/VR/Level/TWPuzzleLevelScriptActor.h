// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/TWLevelScriptActor.h"
#include "TWPuzzleLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class TWFRAMEWORK_API ATWPuzzleLevelScriptActor : public ATWLevelScriptActor
{
	GENERATED_BODY()
	
public:
    	
    	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="VR")
    	void PuzzleMouldSet(int MouldIndex);
    
    	virtual void PuzzleMouldSet_Implementation(int MouldIndex){};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="VR")
	void LevelComplete();
	virtual void LevelComplete_Implementation(){};

};
