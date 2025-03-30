// Copyright (C) GAMEQUAD - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "VR/Puzzle/TWPuzzleVR.h"
#include "VR/Puzzle/TWPuzzleVRBasicPrefab.h"
#include "Engine/DataTable.h"
#include "Components/TimelineComponent.h"
//#include "GameFramework/Actor.h"

#include "TWPuzzleVRBasicMould.generated.h"


USTRUCT(BlueprintType)
struct  FPuzzleMapData : public FTableRowBase 
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATWPuzzleVRBasicMould> Mould;

};

UENUM()
enum EPuzzleMouldStatus
{
	NoneMould UMETA(DisplayName = "None"),

	MouldMain UMETA(DisplayName = "MouldMain"),
	MouldPlaceHolder UMETA(DisplayName = "MouldPlaceHolder"),
	MouldSet UMETA(DisplayName = "MouldSet"),
	MouldElement UMETA(DisplayName = "MouldElement"),
	
};

UCLASS()
class TWFRAMEWORK_API ATWPuzzleVRBasicMould : public ATWPuzzleVR
{
	GENERATED_BODY()
	
public:	

	ATWPuzzleVRBasicMould(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	int NumPuzzleElements = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	bool bComplete = false;
	
	

	UFUNCTION()
	int GetNumPuzzleElementsCurrent() const {return NumPuzzleElementsCurrent;};
	
	UFUNCTION()
	EPuzzleMouldStatus GetMouldStatus() const {return MouldStatus;};
	
	UFUNCTION()
	TArray<ATWPuzzleVRBasicMould*> GetMouldsChildFromMould();

	UFUNCTION()
	TArray<class ATWPuzzleVRBasicShape*> GetPuzzleShapesChildFromMould();
	
	UFUNCTION()
	void ChangeNumPuzzleElementsCurrent(int IntValue);
	
	UFUNCTION()
	bool SetMouldToPlaceHolder();

	UFUNCTION()
	void SetMeshMaterialAsPlaceHolderMaterialForChildren();
	
	UFUNCTION()
	void SetMeshMaterialAsPlaceHolderMaterialIlluminatedForChildren();
	
	UFUNCTION()
	void SetMaterialToAllBasicShapeChild(UMaterial* Material);

	UFUNCTION()
	void SetMouldStatus(const EPuzzleMouldStatus Status) {MouldStatus = Status;};
	
	UFUNCTION()
	void AddMouldToMouldPlaceHoldersArray(ATWPuzzleVRBasicMould* Mould){MouldPlaceHoldersArray.Add(Mould);};
	
	UFUNCTION()
	bool CheckHasMouldAsChild();
	
	UFUNCTION()
	bool IsMouldPlaceHoldersArrayContainsThisItem(ATWPuzzleVRBasicMould* Mould){return MouldPlaceHoldersArray.Contains(Mould);};
	
	UFUNCTION()
	void RemoveMouldFromMouldPlaceHoldersArray(ATWPuzzleVRBasicMould* Mould){MouldPlaceHoldersArray.Remove(Mould);};

	UFUNCTION()
	void CompletePuzzle();

	UFUNCTION()
	void SetTheNearestMouldActor();

	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EPuzzleMouldStatus> MouldStatus = EPuzzleMouldStatus::NoneMould;
	
	UFUNCTION()
	void SetMouldCollisionProfileToAllChildShape();

	//UFUNCTION()
	//void SetBasicMaterialToPuzzleShapeChildren(ATWPuzzleVRBasicMould* Mould);
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	int NumPuzzleElementsCurrent= 0 ;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	bool HasMouldChild = false ;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	TArray<ATWPuzzleVRBasicMould*> MouldPlaceHoldersArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	ATWPuzzleVRBasicMould* TheNearestPuzzleMouldActor = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	ATWPuzzleVRBasicMould* LastTheNearestPuzzleMouldActor = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	float FTolerance = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATWPuzzleVRBasicPrefab> PuzzlePrefab;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATWPuzzleVRBasicShape> PuzzleBasicShapeForPrefab;

	FTimeline CurveTimelineSpawnMouldScaleAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* CurveFloatSpawnScaleAnim;
	
	UFUNCTION()
	void TimelineSpawnScaleAnim(float Value);

	UFUNCTION()
	void SetupTimelineSpawnScaleAnim();

	
	
	UPROPERTY()
	FVector StartScaleObj;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnPuzzlePiecesFromPrefab();

	UFUNCTION()
	void SetupStatusAndNumber();

	UFUNCTION()
	void StopAnimScale();
};
