// Copyright (C) GAMEQUAD - All Rights Reserved


#include "VR/Game/TWGameControllerVRPuzzle.h"

#include "EngineUtils.h"
//#include "TWGameBlueprintFunctionLibrary.h"
#include "VR/Game/TWGameBPFunctionLibraryVRPuzzle.h"
#include "VR/Level/TWPuzzleLevelScriptActor.h"

#include "VR/Procedural/TWProceduralSpawnPuzzleShape.h"

void ATWGameControllerVRPuzzle::BeginPlay()
{
	Super::BeginPlay();
	
	MouldSpawnLocation = GetActorLocation(); 
	MouldSpawnRotator = GetActorRotation();
	FActorSpawnParameters ActorSpawnParameters;
	
	if(PawnVr)
	{
		//PawnVr->GetActorForwardVector() * 
		MouldSpawnLocation = PawnVr->GetActorLocation();
		MouldSpawnLocation += ShiftFromPawnVRToMould;
		MouldSpawnRotator = PawnVr->GetActorRotation();
	}
	
	////
	// Load Puzzle
	///
	UTWGameInstanceVRPuzzle* GameInstanceVRPuzzle = UTWGameBPFunctionLibraryVRPuzzle::GetGameInstanceVRPuzzle(GetWorld());
	if(GameInstanceVRPuzzle)
	{
		FName LevelName = FName(*GetWorld()->GetMapName());
		FName GameInstanceCurrentLevel = GameInstanceVRPuzzle->ReturnCurrentLevelName();
		
		if(LevelName == GameInstanceCurrentLevel)
		{
			CurrentSpawnedIndexPuzzleMould = GameInstanceVRPuzzle->ReturnMouldIndex();
		}
	} 
	
	ATWPuzzleLevelScriptActor* LevelBp = Cast<ATWPuzzleLevelScriptActor>(GetLevel()->GetLevelScriptActor());
	if(LevelBp)
	{
		for(int i = 0; i < CurrentSpawnedIndexPuzzleMould; i++)
		{
			LevelBp->PuzzleMouldSet(i);
		}
	}
	
	ControlPanels =GetControlPanelsFromLevel();
	if(CurrentSpawnedIndexPuzzleMould >= MainMouldToSpawnArray.Num())
	{
		for(ATWPuzzleVRControlPanelMould* ControlPanel : ControlPanels)
		{
			ControlPanel->CompleteAllPuzzle();
		}
	} 
	
	if(MainMouldToSpawnArray.IsValidIndex(CurrentSpawnedIndexPuzzleMould))
	{
		SpawnMainMould(MainMouldToSpawnArray[CurrentSpawnedIndexPuzzleMould],MouldSpawnLocation,MouldSpawnRotator,ActorSpawnParameters);
	}

	ControlPanels =GetControlPanelsFromLevel();

	if(ControlPanels.IsEmpty() == false)
	{
		for(ATWPuzzleVRControlPanelMould* ControlPanel : ControlPanels)
		{
			ControlPanel->SetPuzzlesInformationTable_Handle(PuzzleMouldsData, CurrentSpawnedIndexPuzzleMould);
			ControlPanel->SetNextMapPath_Handle(NextMapPath);
		}

		//
		
	
		
		//
	}
	
}
//-----------------------
//	Get Functions
//----------------------
TArray<ATWProceduralSpawnPuzzleShape*> ATWGameControllerVRPuzzle::GetProceduralSpawnObjectsFromLevel()
{
	TArray<ATWProceduralSpawnPuzzleShape*> LocalArray;
	for(TActorIterator<ATWProceduralSpawnPuzzleShape> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ATWProceduralSpawnPuzzleShape* ProceduralSpawn =  Cast<ATWProceduralSpawnPuzzleShape>(*ActorItr);
		if(ProceduralSpawn)
		{
			LocalArray.Add(ProceduralSpawn);
		}
	}
	return LocalArray;
}

TArray<ATWPuzzleVRBasicShape*> ATWGameControllerVRPuzzle::GetPuzzleShapesFromMould(ATWPuzzleVRBasicMould* Mould)
{
	TArray<ATWPuzzleVRBasicShape*> LocalArray;
	//TArray<AActor*> ChildActors;
	TArray<AActor*> AttachActors;
	//Mould->GetAllChildActors(ChildActors);
	Mould->GetAttachedActors(AttachActors);

	/*
	for(AActor* Actor : ChildActors)
	{
		if(Actor)
		{
			ATWPuzzleVRBasicShape* PuzzleShapeActor = Cast<ATWPuzzleVRBasicShape>(Actor);
			if(PuzzleShapeActor)
			{
				LocalArray.Add(PuzzleShapeActor);
			}
		}
	}*/

	for(AActor* Actor : AttachActors)
	{
		if(Actor)
		{
			ATWPuzzleVRBasicShape* PuzzleShapeActor = Cast<ATWPuzzleVRBasicShape>(Actor);
			if(PuzzleShapeActor)
			{
				bool bSkipPuzzle = PuzzleShapeActor->bSkipPuzzleMouldToSpawn;
				if(!bSkipPuzzle)
				{
					LocalArray.Add(PuzzleShapeActor);
				}
				//PuzzleShapeActor
				
			}
		}
	}


	
	return LocalArray;
}

TArray<ATWPuzzleVRBasicMould*> ATWGameControllerVRPuzzle::GetPuzzleMouldOffMould(ATWPuzzleVRBasicMould* Mould)
{

	TArray<ATWPuzzleVRBasicMould*> LocalArray;
	TArray<AActor*> ChildActors;
	Mould->GetAllChildActors(ChildActors);
	for(AActor* Actor : ChildActors)
	{
		if(Actor)
		{
			ATWPuzzleVRBasicMould* PuzzleShapeActor = Cast<ATWPuzzleVRBasicMould>(Actor);
			if(PuzzleShapeActor)
			{
				LocalArray.Add(PuzzleShapeActor);
			}
		}
	}
	return LocalArray;
	
}

TArray<ATWPuzzleVRControlPanelMould*> ATWGameControllerVRPuzzle::GetControlPanelsFromLevel()
{
	TArray<ATWPuzzleVRControlPanelMould*> ControlPaneslMould;
	for(TActorIterator<ATWPuzzleVRControlPanelMould> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ControlPaneslMould.Add( Cast<ATWPuzzleVRControlPanelMould>(*ActorItr));
	}
	
	return ControlPaneslMould;
}

//-----------------------
//	Puzzle Spawn Functions
//----------------------

void ATWGameControllerVRPuzzle::SpawnMainMould(TSubclassOf<ATWPuzzleVRBasicMould> MouldToSpawn, FVector Location, FRotator Rotator, FActorSpawnParameters& ActorSpawnParameters)
{
	ATWPuzzleVRBasicMould* SpawnedMould = GetWorld()->SpawnActor<ATWPuzzleVRBasicMould>(MouldToSpawn, Location, Rotator, ActorSpawnParameters);
	SpawnedMainMouldArray.Add(SpawnedMould);
		
	if(SpawnedMainMouldArray.Last())
	{
		SpawnedMainMouldArray.Last()->SetMouldStatus(EPuzzleMouldStatus::MouldMain);
		
		ProceduralSpawnPuzzleShapesArray = GetProceduralSpawnObjectsFromLevel();
		
		if(SpawnedMainMouldArray.Last()->CheckHasMouldAsChild())
		{
			SetupChildMould();
		}
		else
		{
			PuzzleShapesToSpawnArray = GetPuzzleShapesFromMould(SpawnedMainMouldArray.Last());
			
			AddPuzzleShapeToSpawnArray(PuzzleShapesToSpawnArray,ProceduralSpawnPuzzleShapesArray );
			SpawnPuzzleShapes(ProceduralSpawnPuzzleShapesArray);
			
		}
	//	SpawnedMainMouldArray.Last()->SetupStatusAndNumber();
	}
}

void ATWGameControllerVRPuzzle::SetupChildMould()
{
	SetPuzzleStatusForMainMould(SpawnedMainMouldArray.Last());
	FVector Loc = SpawnedMainMouldArray.Last()->GetActorLocation();
	Loc.Z = Loc.Z + 1000;
	SpawnedMainMouldArray.Last()->SetActorLocation(Loc);

	PuzzleMouldToSpawnArray = GetPuzzleMouldOffMould(SpawnedMainMouldArray.Last());
	FActorSpawnParameters ActorSpawnParameters;

	//CurrentSpawnIndexPuzzleMould = 0;
	PuzzleMouldChildArray.Add(GetWorld()->SpawnActor<ATWPuzzleVRBasicMould>(PuzzleMouldToSpawnArray[0]->GetClass(), MouldSpawnLocation,MouldSpawnRotator, ActorSpawnParameters));
	PuzzleShapesToSpawnArray = GetPuzzleShapesFromMould(PuzzleMouldChildArray[0]);
			
	AddPuzzleShapeToSpawnArray(PuzzleShapesToSpawnArray,ProceduralSpawnPuzzleShapesArray );
	SpawnPuzzleShapes(ProceduralSpawnPuzzleShapesArray);
}




void ATWGameControllerVRPuzzle::AddPuzzleShapeToSpawnArray(TArray<ATWPuzzleVRBasicShape*> PuzzleShapes,TArray<ATWProceduralSpawnPuzzleShape*> ProceduralSpawnArray)
{
	const int NumProceduralSpawnArray = ProceduralSpawnArray.Num();
	int CurrentToken = 0; // 0 1 2 

	for(ATWPuzzleVRBasicShape* LocalActor : PuzzleShapes)
	{
		if(LocalActor)
		{
			ProceduralSpawnArray[CurrentToken]->AddPuzzleShapeToSpawn(LocalActor);
			CurrentToken < NumProceduralSpawnArray - 1 ? CurrentToken++ : CurrentToken = 0;
		}
	}
}


void ATWGameControllerVRPuzzle::SpawnPuzzleShapes(TArray<ATWProceduralSpawnPuzzleShape*> ProceduralSpawnArray)
{
	for(ATWProceduralSpawnPuzzleShape* Actor : ProceduralSpawnArray)
	{
		if(Actor)
		{
			Actor->SpawnPuzzleShapes();
		}
		//GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Black, FString::Printf(TEXT("Liczba to stworzenia: %d "), NumPuzzleToSpawn));
	}
}



void ATWGameControllerVRPuzzle::ClearPuzzleShapeToSpawnArray(TArray<ATWProceduralSpawnPuzzleShape*> ProceduralSpawnArray)
{
	for(ATWProceduralSpawnPuzzleShape* ProceduralSpawn : ProceduralSpawnArray)
	{
		if(ProceduralSpawn)
		{
		
			ProceduralSpawn->ClearPuzzleShapeToSpawn();
		}
	}
}


//-----------------------
//	Set Functions
//----------------------

void ATWGameControllerVRPuzzle::SetPuzzleStatusForMainMould(ATWPuzzleVRBasicMould* MainMould)
{

	TArray<ATWPuzzleVRBasicMould*> MouldsArray = MainMould->GetMouldsChildFromMould();
	for(ATWPuzzleVRBasicMould* Mould : MouldsArray)
	{
		if(Mould)
		{
			TArray<ATWPuzzleVRBasicShape*> PuzzleShapeArray = Mould->GetPuzzleShapesChildFromMould();

			for(ATWPuzzleVRBasicShape* PuzzleShape : PuzzleShapeArray)
			{
				if(PuzzleShape)
				{
					PuzzleShape->SetPuzzleShapeStatus(EPuzzleShapeStatus::InMouldPlaceHolder);
				}
			}
		}
	}
}

//-----------------------
//	Complete and Next Functions
//----------------------

void ATWGameControllerVRPuzzle::CompleteStage(ATWPuzzleVRBasicMould* Mould)
{
	//int index = PuzzleMouldToSpawnArray.Find(Mould);
	int NextIndex = CurrentSpawnedIndexPuzzleMould + 1;

	if (NextIndex < PuzzleMouldToSpawnArray.Num()) //    2     3
	{
		FActorSpawnParameters ActorSpawnParameters;
		
		PuzzleMouldChildArray.Add(GetWorld()->SpawnActor<ATWPuzzleVRBasicMould>(
			PuzzleMouldToSpawnArray[NextIndex]->GetClass(), MouldSpawnLocation, MouldSpawnRotator,
			ActorSpawnParameters));
		
		//CurrentSpawnedIndexPuzzleMould++;

		PuzzleShapesToSpawnArray = GetPuzzleShapesFromMould(PuzzleMouldChildArray[PuzzleMouldChildArray.Num() - 1]);

		ClearPuzzleShapeToSpawnArray(ProceduralSpawnPuzzleShapesArray);
		AddPuzzleShapeToSpawnArray(PuzzleShapesToSpawnArray, ProceduralSpawnPuzzleShapesArray);
		SpawnPuzzleShapes(ProceduralSpawnPuzzleShapesArray);
	}
	else if (CurrentSpawnedIndexPuzzleMould + 1 == PuzzleMouldToSpawnArray.Num())
	{
		FVector Loc = SpawnedMainMouldArray.Last()->GetActorLocation();
		Loc.Z = Loc.Z + -1000;
		SpawnedMainMouldArray.Last()->SetActorLocation(Loc);
		//PuzzleMould->GetActorLocation();
		DrawDebugSphere(GetWorld(), SpawnedMainMouldArray.Last()->GetActorLocation(), 10.5f, 4, FColor::Red, false,
		                2.0f);
	}

	if(Mould->GetMouldStatus() == EPuzzleMouldStatus::MouldMain)
	{
		
		ATWPuzzleLevelScriptActor* LevelBp = Cast<ATWPuzzleLevelScriptActor>(GetLevel()->GetLevelScriptActor());
		if(LevelBp)
		{
			LevelBp->PuzzleMouldSet(CurrentSpawnedIndexPuzzleMould);
		}
		
		FActorSpawnParameters ActorSpawnParameters;
		
		PuzzleMouldChildArray.Empty();
		ClearPuzzleShapeToSpawnArray(ProceduralSpawnPuzzleShapesArray);


		if(ControlPanels.Num() > 1 && CurrentSpawnedIndexPuzzleMould < MainMouldToSpawnArray.Num())
		{
			for(ATWPuzzleVRControlPanelMould* Actor : ControlPanels)
			{
				Actor->NextMould(CurrentSpawnedIndexPuzzleMould + 1);
			}
		}
		
		///
		// Save
		////

		
		UTWGameInstanceVRPuzzle* GameInstanceVRPuzzle = UTWGameBPFunctionLibraryVRPuzzle::GetGameInstanceVRPuzzle(GetWorld());
		if(GameInstanceVRPuzzle)
		{
			FString StringLevelName = GetWorld()->GetMapName();
			StringLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

			FName LevelName = FName(*StringLevelName);
			
			GameInstanceVRPuzzle->ChangeSaveProperty(LevelName, NextIndex);
			GameInstanceVRPuzzle->SaveGame("SaveTWFPuzzleVR",0);
			
			GameInstanceVRPuzzle->SaveGameVRPuzzle(0, LevelName, NextIndex,-1, false);
			//GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Blue, StringLevelName);
			//GameInstanceVRPuzzle->SaveGame()
		}
		CurrentSpawnedIndexPuzzleMould++;
		
		if(CurrentSpawnedIndexPuzzleMould == MainMouldToSpawnArray.Num())
		{
			FString StringLevelName = GetWorld()->GetMapName();
			StringLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

			FName LevelName = FName(*StringLevelName);
			GameInstanceVRPuzzle->SaveGameVRPuzzle(0, LevelName, NextIndex, -1,true);
			
			CompleteAllPuzzles();
		}
		
		if(CurrentSpawnedIndexPuzzleMould < MainMouldToSpawnArray.Num())
		{
			SpawnMainMould(MainMouldToSpawnArray[CurrentSpawnedIndexPuzzleMould],MouldSpawnLocation,MouldSpawnRotator,ActorSpawnParameters);
		}

		
		TArray<AActor*> LocalActors;
		Mould->GetAttachedActors(LocalActors);
		for(AActor* LocalActor : LocalActors)
		{
			LocalActor->Destroy();
		}
		Mould->Destroy();
	}
	
}

void ATWGameControllerVRPuzzle::CompleteAllPuzzles()
{

	ATWPuzzleLevelScriptActor* LevelBp = Cast<ATWPuzzleLevelScriptActor>(GetLevel()->GetLevelScriptActor());
	if(LevelBp)
	{
		LevelBp->LevelComplete();
	}

	for(ATWPuzzleVRControlPanelMould* Actor : ControlPanels)
	{
		Actor->CompleteAllPuzzle();
	}
	
	//GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Blue, FString::Printf(TEXT("Completa All Puzzle") ));
	
}


//-----------------------
//	Console Function
//----------------------


void ATWGameControllerVRPuzzle::PutRandomPuzzle()
{

	//GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Blue, FString::Printf(TEXT("Game Controller") ));
	
	TArray<ATWPuzzleVRBasicShape*> PuzzleInMould;
	TArray<ATWPuzzleVRBasicShape*> PuzzlePieces;
	TArray<ATWPuzzleVRBasicMould*> MouldSet;
	TArray<ATWPuzzleVRBasicMould*> MouldMain;
	TArray<ATWPuzzleVRBasicMould*> MouldElements;
	TArray<ATWPuzzleVRBasicMould*> MouldPlaceHolders;
	
	for(TActorIterator<ATWPuzzleVRBasicMould> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr)
		{
			ATWPuzzleVRBasicMould* Mould = Cast<ATWPuzzleVRBasicMould>(*ActorItr);
			Mould->StopAnimScale();
			switch (Mould->GetMouldStatus())
			{
			case EPuzzleMouldStatus::MouldSet:
				MouldSet.Add(Mould);
				break;
			case EPuzzleMouldStatus::MouldMain:
				MouldMain.Add(Mould);
				break;
			case EPuzzleMouldStatus::MouldElement:
				MouldElements.Add(Mould);
				break;
			case EPuzzleMouldStatus::MouldPlaceHolder:
				MouldPlaceHolders.Add(Mould);
				break;
			default:
				break;
			}
		}
	}

	if (MouldElements.Num() == 0)
	{
		for (ATWPuzzleVRBasicMould* Mould : MouldSet)
		{
			for (ATWPuzzleVRBasicMould* MouldPlaceHolder : MouldPlaceHolders)
			{
				Mould->SetActorRotation(MouldPlaceHolder->GetActorRotation());
				Mould->SetActorLocation(MouldPlaceHolder->GetActorLocation());
				Mould->SetTheNearestMouldActor();
				if (Mould)
				{
					const bool bIsSet = Mould->SetMouldToPlaceHolder();
					if (bIsSet)
					{
						Mould->Destroy();
						break;
					}
				}
			}
		}
	}
	
	for(TActorIterator<ATWPuzzleVRBasicShape> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if(ActorItr)
		{
			ATWPuzzleVRBasicShape* Puzzle = Cast<ATWPuzzleVRBasicShape>(*ActorItr);
			if(Puzzle->GetPuzzleShapeStatus() == EPuzzleShapeStatus::PuzzlePlaceHolder)
			{
				PuzzleInMould.Add(Puzzle);		
			}else if(Puzzle->GetPuzzleShapeStatus() == EPuzzleShapeStatus::PuzzleElement)
			{
				PuzzlePieces.Add(Puzzle);
				Puzzle->StopAnimScale();
			}
			
		}
	}	

	int numPuzzle = (PuzzlePieces.Num() - 1);
	int randomNum = FMath::RandRange(0, numPuzzle);
	ATWPuzzleVRBasicShape* PuzzleRandomElement = PuzzlePieces[randomNum];

	
		for(ATWPuzzleVRBasicShape* PuzzleMould : PuzzleInMould)
		{
			PuzzleRandomElement->SetActorLocation(PuzzleMould->GetActorLocation());
			PuzzleRandomElement->SetActorRotation(PuzzleMould->GetActorRotation());
			PuzzleRandomElement->SetTheNearestPuzzleBasicShapeActor();
			//FPlatformProcess::Sleep(2);
			if(PuzzleRandomElement)
			{
				if(PuzzleRandomElement->SetPuzzleToPlaceHolder())
				{
					PuzzleRandomElement->Destroy();
					break;
				}
			}
		}
}

void ATWGameControllerVRPuzzle::PutTogetherPuzzle()
{

	//GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Blue, FString::Printf(TEXT("Game Controller") ));
	
	TArray<ATWPuzzleVRBasicShape*> PuzzleInMould;
	TArray<ATWPuzzleVRBasicShape*> PuzzlePieces;
	TArray<ATWPuzzleVRBasicMould*> MouldSet;
	TArray<ATWPuzzleVRBasicMould*> MouldMain;
	TArray<ATWPuzzleVRBasicMould*> MouldElements;
	TArray<ATWPuzzleVRBasicMould*> MouldPlaceHolders;
	
	for(TActorIterator<ATWPuzzleVRBasicMould> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr)
		{
			ATWPuzzleVRBasicMould* Mould = Cast<ATWPuzzleVRBasicMould>(*ActorItr);
			Mould->StopAnimScale();
			switch (Mould->GetMouldStatus())
			{
			case EPuzzleMouldStatus::MouldSet:
				MouldSet.Add(Mould);
				break;
			case EPuzzleMouldStatus::MouldMain:
				MouldMain.Add(Mould);
				break;
			case EPuzzleMouldStatus::MouldElement:
				MouldElements.Add(Mould);
				break;
			case EPuzzleMouldStatus::MouldPlaceHolder:
				MouldPlaceHolders.Add(Mould);
				break;
			default:
				break;
			}
		}
	}

	if (MouldElements.Num() == 0)
	{
		for (ATWPuzzleVRBasicMould* Mould : MouldSet)
		{
			for (ATWPuzzleVRBasicMould* MouldPlaceHolder : MouldPlaceHolders)
			{
				Mould->SetActorRotation(MouldPlaceHolder->GetActorRotation());
				Mould->SetActorLocation(MouldPlaceHolder->GetActorLocation());
				Mould->SetTheNearestMouldActor();
				if (Mould)
				{
					const bool bIsSet = Mould->SetMouldToPlaceHolder();
					if (bIsSet)
					{
						Mould->Destroy();
						break;
					}
				}
			}
		}
	}
	
	for(TActorIterator<ATWPuzzleVRBasicShape> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if(ActorItr)
		{
			ATWPuzzleVRBasicShape* Puzzle = Cast<ATWPuzzleVRBasicShape>(*ActorItr);
			if(Puzzle->GetPuzzleShapeStatus() == EPuzzleShapeStatus::PuzzlePlaceHolder)
			{
				PuzzleInMould.Add(Puzzle);		
			}else if(Puzzle->GetPuzzleShapeStatus() == EPuzzleShapeStatus::PuzzleElement)
			{
				PuzzlePieces.Add(Puzzle);
				Puzzle->StopAnimScale();
			}
		}
	}	
	
	
	for(ATWPuzzleVRBasicShape* Puzzle : PuzzlePieces)
	{
		for(ATWPuzzleVRBasicShape* PuzzleMould : PuzzleInMould)
		{
			Puzzle->SetActorLocation(PuzzleMould->GetActorLocation());
			Puzzle->SetActorRotation(PuzzleMould->GetActorRotation());
			Puzzle->SetTheNearestPuzzleBasicShapeActor();
			//FPlatformProcess::Sleep(2);
			if(Puzzle)
			{
				if(Puzzle->SetPuzzleToPlaceHolder())
				{
					Puzzle->Destroy();
					break;
				}
			}
		}
	}
	
	
	
}



void ATWGameControllerVRPuzzle::SwitchUMGControlPanel(int index)
{
	for(ATWPuzzleVRControlPanelMould* Actor : ControlPanels)
	{
		Actor->SwitchUMG(index);
	}
}