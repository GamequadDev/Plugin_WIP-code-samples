// Copyright (C) GAMEQUAD - All Rights Reserved



#include "VR/Game/TWGameControllerVR.h"
#include "VR/Character/TWCharacterVR.h"
#include "EngineUtils.h"
#include "Actions/PawnActionsComponent.h"


ATWGameControllerVR::ATWGameControllerVR(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void ATWGameControllerVR::BeginPlay()
{
	Super::BeginPlay();

	for(TActorIterator<APawn> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		PawnVr =  Cast<ATWVRPawn>(*ActorItr);
		if(PawnVr)
		{
			break;
		}
	}
	
	PossesToVRPlayer();
}

//----------------------------------------------------------------------//
// Posses To VR Player
//----------------------------------------------------------------------//

void ATWGameControllerVR::PossesToVRPlayer()
{
	for(TActorIterator<APawn> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		PlayerVr =  Cast<ATWCharacterVR>(*ActorItr);
		if(PlayerVr)
		{
			GetPlayerController()->Possess(PlayerVr);
			break;
		}
	}
}