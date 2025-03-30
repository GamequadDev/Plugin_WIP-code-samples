// Fill out your copyright notice in the Description page of Project Settings.


#include "VR/Game/TWGameBPFunctionLibraryVRPuzzle.h"

#include "VR/Game/TWGameInstanceVRPuzzle.h"

void UTWGameBPFunctionLibraryVRPuzzle::LoadGameVRPuzzle(const UObject* WorldContextObject, FString InSlot,int32 UserIndex)
{
	if (WorldContextObject != nullptr && WorldContextObject->GetWorld() != nullptr)
	{
		UTWGameInstanceVRPuzzle* GameInstance = Cast<UTWGameInstanceVRPuzzle>(WorldContextObject->GetWorld()->GetGameInstance());
		if (GameInstance != nullptr)
		{
			GameInstance->LoadGame(InSlot,UserIndex);
		}
	}
}

void UTWGameBPFunctionLibraryVRPuzzle::SaveGameVRPuzzle(const UObject* WorldContextObject, FString InSlot,int32 UserIndex)
{
	if (WorldContextObject != nullptr && WorldContextObject->GetWorld() != nullptr)
	{
		UTWGameInstanceVRPuzzle* GameInstance = Cast<UTWGameInstanceVRPuzzle>(WorldContextObject->GetWorld()->GetGameInstance());
		if (GameInstance != nullptr)
		{
			GameInstance->SaveGame(InSlot,UserIndex);
		}
	}
}

void UTWGameBPFunctionLibraryVRPuzzle::DeleteSaveVRPuzzle(const UObject* WorldContextObject,int32 UserIndex)
{
	if (WorldContextObject != nullptr && WorldContextObject->GetWorld() != nullptr)
	{
		UTWGameInstanceVRPuzzle* GameInstance = Cast<UTWGameInstanceVRPuzzle>(WorldContextObject->GetWorld()->GetGameInstance());
		if (GameInstance != nullptr)
		{
			FString StringLevelName = WorldContextObject->GetWorld()->GetMapName();
			StringLevelName.RemoveFromStart(WorldContextObject->GetWorld()->StreamingLevelsPrefix);

			GameInstance->DeleteGame(StringLevelName,UserIndex);
		}
	}
}

UTWGameInstanceVRPuzzle* UTWGameBPFunctionLibraryVRPuzzle::GetGameInstanceVRPuzzle(const UObject* WorldContextObject)
{
	UTWGameInstanceVRPuzzle* GameInstance = nullptr;
	if (WorldContextObject != nullptr && WorldContextObject->GetWorld() != nullptr)
	{
		GameInstance = Cast<UTWGameInstanceVRPuzzle>(WorldContextObject->GetWorld()->GetGameInstance());
	}

	return GameInstance;
}

void UTWGameBPFunctionLibraryVRPuzzle::ResetVRPuzzle(const UObject* WorldContextObject,int32 UserIndex)
{
	if (WorldContextObject != nullptr && WorldContextObject->GetWorld() != nullptr)
	{
		UTWGameInstanceVRPuzzle* GameInstance = Cast<UTWGameInstanceVRPuzzle>(WorldContextObject->GetWorld()->GetGameInstance());
		if (GameInstance != nullptr)
		{
			FString StringLevelName = WorldContextObject->GetWorld()->GetMapName();
			StringLevelName.RemoveFromStart(WorldContextObject->GetWorld()->StreamingLevelsPrefix);
			GameInstance->ResetSavedProperty();
			GameInstance->OpenLevel(FName(*StringLevelName),"");
		}
	}
}

ATWPuzzleVRBasicShape* UTWGameBPFunctionLibraryVRPuzzle::CreatePuzzleShape(const UObject* WorldContextObject,TSubclassOf<ATWPuzzleVRBasicShape> PuzzleBasicShapeForPrefab,UStaticMesh* Mesh, FTransform SpawnTransform)
{
	
	FActorSpawnParameters SpawnParameters;
	ATWPuzzleVRBasicShape* Actor = WorldContextObject->GetWorld()->SpawnActor<ATWPuzzleVRBasicShape>(PuzzleBasicShapeForPrefab,SpawnTransform.GetLocation(),SpawnTransform.Rotator(), SpawnParameters);
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject,ATWPuzzleVRBasicMould::StaticClass(),Actors);
	if(Actors[0])
	{
		Actor->AttachToActor(Actors[0],  FAttachmentTransformRules::KeepWorldTransform);
	}

	Actor->SetPuzzleShapeStatus(EPuzzleShapeStatus::PuzzleEditor);
	
	Actor->SetMeshMaterial(Mesh->GetMaterial(0));
	
	if(Mesh->GetStaticMaterials().Num() > 1)
	{
		Actor->SetBasicMaterial(Mesh->GetMaterial(0));
		TArray<UMaterialInterface*> Materials;
		for (int i = 0; i < Mesh->GetStaticMaterials().Num(); i++)
		{
			Materials[i] = Mesh->GetMaterial(i);
		}
		Actor->SetBasicMaterials(Materials);
	}
	else
	{
		Actor->SetBasicMaterial(Mesh->GetMaterial(0));
	}
			
	Actor->BasicStaticMesh->SetStaticMesh(Mesh);
			
	//Actor->GetRootComponent()->SetWorldScale3D(Mesh->GetComponentScale());
	//Actor->ScaleObjAnimReturn
			
	Actor->SetActorScale3D(SpawnTransform.GetScale3D());
	Actor->DefaultScaleObj = Actor->GetActorScale3D();
	//Actor->DefaultExtendedBounds = Actor->BasicStaticMesh->GetStaticMesh()->GetExtendedBounds();
	//Actor->SetPuzzleScaleObj(Actor->GetActorScale3D());
			
	Actor->SetupStatusAndMaterial();


	return Actor;
}