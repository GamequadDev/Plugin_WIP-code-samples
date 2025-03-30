// Copyright (C) GAMEQUAD - All Rights Reserved


#include "VR/Game/TWGameInstanceVRPuzzle.h"

#include "VR/Game/TWGameControllerVRPuzzle.h"
#include "VR/Puzzle/TWPuzzleVRBasicShape.h"
#include "EngineUtils.h"
#include "VR/Hand/TWHandVRPuzzle.h"
#include "VR/Save/TWSaveGameVRPuzzle.h"

void UTWGameInstanceVRPuzzle::Init()
{
	Super::Init();
}

/////////
//
// Commands
//
/////////////

void UTWGameInstanceVRPuzzle::TWFPuzzleVR_PutTogetherPuzzle()
{
	
	if(GetGameControllerVRPuzzle())
	{
		GetGameControllerVRPuzzle()->PutTogetherPuzzle();
	}
	
}

void UTWGameInstanceVRPuzzle::TWFPuzzleVR_PutRandomPuzzle()
{
	
	if(GetGameControllerVRPuzzle())
	{
		GetGameControllerVRPuzzle()->PutRandomPuzzle();
	}
	
}

void UTWGameInstanceVRPuzzle::TWFPuzzleVR_HandsGrabByDistance()
{
	for (TActorIterator<ATWHandVRPuzzle> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ATWHandVRPuzzle * theActor = *ActorItr;
		if(theActor)
		{
			theActor->HandleGrabByDistance();
		}
	}
	
}


void UTWGameInstanceVRPuzzle::TWFPuzzleVR_ControlPanelSwitchUI(int index)
{
	if(GetGameControllerVRPuzzle())
	{
		GetGameControllerVRPuzzle()->SwitchUMGControlPanel(index);
	}
}


ATWGameControllerVRPuzzle* UTWGameInstanceVRPuzzle::GetGameControllerVRPuzzle()
{
	if (GameControllerVRPuzzle == nullptr)
	{
		for (TActorIterator<ATWGameControllerVRPuzzle> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			GameControllerVRPuzzle = *ActorItr;
			return GameControllerVRPuzzle;
		}
	}

	return GameControllerVRPuzzle;
}




//----------------------------------------------------------------------//
// Save and Load Functions
//----------------------------------------------------------------------//

void UTWGameInstanceVRPuzzle::SaveGame(const FString& InSlotName, const int32 UserIndex)
{
	
	if (UTWSaveGameVRPuzzle* SaveGameInstance = Cast<UTWSaveGameVRPuzzle>(UGameplayStatics::CreateSaveGameObject(UTWSaveGameVRPuzzle::StaticClass())))
	{
		SaveGameInstance->LastLevelData.LevelName = CurrentLevelName;
		SaveGameInstance->LastLevelData.MouldIndex = MouldIndex;
		SaveGameInstance->LastLevelData.ChildMouldIndex = ChildMouldIndex;

		UTWSaveGameVRPuzzle* LoadedGame = Cast<UTWSaveGameVRPuzzle>(UGameplayStatics::LoadGameFromSlot(InSlotName, UserIndex));
		if(LoadedGame)
		{
			SaveGameInstance->ProgressAllLevels = LoadedGame->ProgressAllLevels;
			
		}
		
		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, InSlotName, UserIndex);
	}
}

bool UTWGameInstanceVRPuzzle::LoadGame(const FString& InSlotName, const int32 UserIndex)
{
	if(LoadGameData(InSlotName,UserIndex))
	{
		if (CurrentLevelName.IsValid() && !CurrentLevelName.IsNone())
		{
			OpenCurrentLevel(CurrentLevelName);
			return true;
		}
		else
		{
			UE_LOG(LogTWF, Log, TEXT("Error load Map: %s"), *CurrentLevelName.ToString());
		}
	}
	else
	{
		UE_LOG(LogTWF, Log, TEXT("Error game load %s"), *InSlotName);
	}
	return false;
}

bool UTWGameInstanceVRPuzzle::LoadGameData(const FString& InSlotName, const int32 UserIndex)
{
	if (UTWSaveGameVRPuzzle* LoadedGame = Cast<UTWSaveGameVRPuzzle>(UGameplayStatics::LoadGameFromSlot(InSlotName, UserIndex)))
	{
		CurrentLevelName = LoadedGame->LastLevelData.LevelName;
		MouldIndex = LoadedGame->LastLevelData.MouldIndex;
		ChildMouldIndex = LoadedGame->LastLevelData.ChildMouldIndex;
		return true;
	}
	else
	{
		UE_LOG(LogTWF, Log, TEXT("Error game data load %s"), *InSlotName);
		return false;
	}
}

void UTWGameInstanceVRPuzzle::ChangeSaveProperty(FName Level, int NewMouldIndex, int NewChildMouldIndex)
{
	CurrentLevelName = Level;
	MouldIndex = NewMouldIndex;
	ChildMouldIndex = NewChildMouldIndex;
}

FName UTWGameInstanceVRPuzzle::SearchLevelPathFromTable(FName LevelName, UTWLevelsData* LevelsData)
{
	
	for(FSTWLevelMapData LevelMapData : LevelsData->LevelsData)
	{
		if(LevelMapData.ID == LevelName)
		{
			return LevelMapData.Name;
		}
	}
	return "none";
}

void UTWGameInstanceVRPuzzle::SaveGameVRPuzzle(const int32 UserIndex,const FName& Level,
	int NewMouldIndex, int NewChildMouldIndex, bool bCompleteLevel)
{
	
	if(UTWSaveGameVRPuzzle* LoadedGame = Cast<UTWSaveGameVRPuzzle>(UGameplayStatics::LoadGameFromSlot(Level.ToString(), UserIndex)))
	{
		LoadedGame->LastLevelData.bFinish = bCompleteLevel;
		LoadedGame->LastLevelData.LevelName = Level;
		LoadedGame->LastLevelData.MouldIndex = NewMouldIndex;
		LoadedGame->LastLevelData.ChildMouldIndex = NewChildMouldIndex;

		FString text1 = LoadedGame->LastLevelData.bFinish ? TEXT("True") : TEXT("False");
		
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Level.ToString());
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, LoadedGame->LastLevelData.LevelName.ToString());
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, text1);
		
		UGameplayStatics::AsyncSaveGameToSlot(LoadedGame, Level.ToString(), UserIndex);
	}
	else
	{
		if(UTWSaveGameVRPuzzle* SaveGameInstance = Cast<UTWSaveGameVRPuzzle>(UGameplayStatics::CreateSaveGameObject(UTWSaveGameVRPuzzle::StaticClass())))
		{
			//FSaveGameLevelPuzzleData Data;
			SaveGameInstance->LastLevelData.bFinish = bCompleteLevel;
			SaveGameInstance->LastLevelData.LevelName = Level;
			SaveGameInstance->LastLevelData.MouldIndex = NewMouldIndex;
			SaveGameInstance->LastLevelData.ChildMouldIndex = NewChildMouldIndex;
			
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Level.ToString());	
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, SaveGameInstance->LastLevelData.LevelName.ToString());
			
			//SaveGameInstance->ProgressAllLevels.Add(Level,Data);
			
			UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, Level.ToString(), UserIndex);
		}
		
	}
	
}

EPuzzleLevelStatus UTWGameInstanceVRPuzzle::CheckLevelStatus(const FString& InSlotName, const int32 UserIndex)
{

	if (UTWSaveGameVRPuzzle* LoadedGame = Cast<UTWSaveGameVRPuzzle>(UGameplayStatics::LoadGameFromSlot(InSlotName, UserIndex)))
	{
		const bool bFinish = LoadedGame->LastLevelData.bFinish;
		const int IndexMould = LoadedGame->LastLevelData.MouldIndex;
		if(bFinish)
		{
			return EPuzzleLevelStatus::Complete;
		}

		if(IndexMould > 0)
		{
			return EPuzzleLevelStatus::Progress;
		}
		
	}
	return EPuzzleLevelStatus::None;
	
}

void UTWGameInstanceVRPuzzle::ResetSavedProperty()
{
	MouldIndex = 0;
	ChildMouldIndex = 0;
}
