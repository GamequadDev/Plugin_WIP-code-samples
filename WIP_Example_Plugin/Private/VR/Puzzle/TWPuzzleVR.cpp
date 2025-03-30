// Fill out your copyright notice in the Description page of Project Settings.


#include "VR/Puzzle/TWPuzzleVR.h"

AActor* ATWPuzzleVR::GetPuzzleTopAttachParent(AActor* LocalActor)
{
	AActor* ValidParentActor = nullptr;
	AActor* ActorParent = LocalActor;
	TArray<AActor*> ParentActors;
	
	int ParentLevel = 0;
	
	ParentActors.Add(ActorParent);
	
	if(LocalActor->GetAttachParentActor())
	{
		ActorParent = LocalActor->GetAttachParentActor();
		ParentActors.Add(ActorParent);
		++ParentLevel;
		
		while (ActorParent->GetAttachParentActor())
		{
			ActorParent = ActorParent->GetAttachParentActor();
			ParentActors.Add(ActorParent);
			++ParentLevel;
		}
	}
	
	for(int32 Index = ParentActors.Num() - 1; Index != -1; Index--)
	{	
		bool IsPuzzle = ParentActors[Index]->GetClass()->IsChildOf(StaticClass());
		
		//UE_LOG(LogTemp, Warning, TEXT("Puzzle twpuzzle class: %s"), *StaticClass()->GetName());
		//UE_LOG(LogTemp, Warning, TEXT("Grab puzzle class: %s"), *ParentActors[Index]->GetClass()->GetName());
		//UE_LOG(LogTemp, Warning, TEXT("Bool value is: %s"), IsPuzzle? TEXT("true")  :TEXT("false") );
		
		if(IsPuzzle)
		{
			ValidParentActor = ParentActors[Index];
			return ValidParentActor;
		}
	}
	
	return ValidParentActor;
	
}
