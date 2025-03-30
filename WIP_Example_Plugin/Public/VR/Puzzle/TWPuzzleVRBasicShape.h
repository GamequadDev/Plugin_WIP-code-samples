// Copyright (C) GAMEQUAD - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "VR/Puzzle/TWPuzzleVRBasicMould.h"
//#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TimelineComponent.h"
#include "TWPuzzleVRBasicShape.generated.h"

UENUM()
enum EPuzzleShapeStatus
{
	NonePuzzle UMETA(DisplayName = "None"),
	InMouldPlaceHolder UMETA(DisplayName = "MouldPlaceHolder"),
	InChildMould  UMETA(DisplayName = "ChildMould"),
	
	PuzzleElement UMETA(DisplayName = "PuzzleElement"),
	PuzzlePlaceHolder UMETA(DisplayName = "PuzzlePlaceHolder"),
	PuzzleEditor UMETA(DisplayName = "PuzzleEditor"),
	
	PuzzleSet UMETA(DisplayName = "Set"),
	
};

UCLASS()
class TWFRAMEWORK_API ATWPuzzleVRBasicShape : public ATWPuzzleVR
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATWPuzzleVRBasicShape(const class FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EPuzzleShapeStatus> PuzzleStatus = EPuzzleShapeStatus::NonePuzzle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	float FTolerance = 6.0f;

	
	FTimeline CurveTimelineScaleObjAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* CurveFloatScaleObjAnim;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* CurveFloatOverlayMaterial;
	
	FVector ScaleObjAnimReturn = FVector::One();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	float MaxScaleObjAnim = 1.5f;

	FTimeline CurveTimelineSpawnScaleAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* CurveFloatSpawnScaleAnim;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	bool bPlaceHolder = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	bool bPuzzleElementSet = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	bool bLoadSave = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	ATWPuzzleVRBasicShape* PuzzleBasicShapeActorOverlap = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	TArray<ATWPuzzleVRBasicShape*> PuzzlePlaceHoldersArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	TArray<ATWPuzzleVRBasicMould*> MouldPlaceHoldersArray;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* BasicMaterial;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	TArray<UMaterialInterface*> BasicMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	bool bMultiMaterials = false;

	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	//* BasicMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	UMaterial* PlaceHolderMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	UMaterial* PlaceHolderMaterialIlluminated;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	UMaterial* PlaceHolderMaterialFit;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	UMaterialInstance * OverlapMaterial;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	ATWPuzzleVRBasicShape* TheNearestPuzzleBasicShapeActor = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	ATWPuzzleVRBasicShape* LastTheNearestPuzzleBasicShapeActor = nullptr;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	ATWPuzzleVRBasicMould* ActorParentMould;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	USoundBase* PuzzleToPlaceHolderSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* DynOverlayMaterialTemp;

	UPROPERTY()
	TArray<FString>  HandsInteractive;
	
	UFUNCTION()
	ATWPuzzleVRBasicShape* ComparePuzzleShapesClassAndLenghtOffThis(TArray<ATWPuzzleVRBasicShape*> PuzzleShapeArray);

	UFUNCTION()
	ATWPuzzleVRBasicShape* ComparePuzzleShapeStaticMeshAndLenghtOffThis(TArray<ATWPuzzleVRBasicShape*> PuzzleShapeArray);

	FVector SmoothLocationTarget;


#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StaticMesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BasicStaticMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	bool bSkipPuzzleMouldToSpawn = false;


	
	//UFUNCTION()
	//TEnumAsByte<EPuzzleShapeStatus> CheckPuzzleStatus() const {return PuzzleStatus;};
	
	//UFUNCTION()
	//void SetPuzzleStatus(TEnumAsByte<EPuzzleShapeStatus> Status) {PuzzleStatus = Status;};

	
	UFUNCTION()
	void MeshOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION()
	void MeshOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void SetMeshMaterial(UMaterialInterface* Material);

	UFUNCTION()
	void SetMeshMaterials(TArray<UMaterialInterface*> Materials);

	UFUNCTION()
	void SetOneMaterialMeshMaterials(UMaterialInterface* Material);
	
	UFUNCTION()
	void SetMeshCollisionProfile(FName  Profile) const {this->BasicStaticMesh->SetCollisionProfileName(Profile);};

	UFUNCTION()
	void SetBoolPuzzleElement(bool PuzzleElementSet){bPuzzleElementSet = PuzzleElementSet;};
	
	UFUNCTION()
	void SetPuzzleShapeStatus(const EPuzzleShapeStatus Status){PuzzleStatus = Status;};

	UFUNCTION()
	void SetPuzzleScaleObj(const FVector Scale){ScaleObjAnimReturn = Scale;};
	
	UFUNCTION()
	EPuzzleShapeStatus GetPuzzleShapeStatus(){return PuzzleStatus;};
	//UFUNCTION()
	//bool CheckIsPlaceHolder(); 

	UFUNCTION()
	bool SetPuzzleToPlaceHolder();
	
	UFUNCTION()
	bool CheckValidPlaceHolder();
	
	UFUNCTION()
	void ReplacePlaceHolder();

	UFUNCTION()
	bool IsPlaceHolderFit();

	/** 
	* Set variable BasicMaterial
	*
	* @param Material            Material 
	 */
	UFUNCTION()
	void SetBasicMaterial(UMaterialInterface* Material){BasicMaterial = Material;};

	UFUNCTION()
	void SetBasicMaterials(TArray<UMaterialInterface*> Materials){BasicMaterials = Materials;};

	UFUNCTION()
	void SetOverlapAnimMaterial();
	
	
	/** 
	* Get variable  BasicMaterial
	*
	* @return                         Get variable  BasicMaterial
	 */
	
	UFUNCTION()
	UMaterialInterface* GetBasicMaterial() const {return BasicMaterial ;};
	
	UFUNCTION()
	ATWPuzzleVRBasicShape* GetTheNearestObjectFromPlaceHoldersArray(TArray<ATWPuzzleVRBasicShape*> Array);

	UFUNCTION()
	int GetNumberMaterialsStaticMesh() const {return BasicStaticMesh->GetMaterials().Num();};
	

	UFUNCTION()
	void SetMeshMaterialAsBasicMaterial(){SetMeshMaterial(BasicMaterial);};
	
	UFUNCTION() 
	void SetMeshMaterialAsPlaceHolderMaterial(){SetMeshMaterial(PlaceHolderMaterial);};
	
	UFUNCTION() 
	void SetMeshMaterialAsPlaceHolderMaterialIlluminated(){SetMeshMaterial(PlaceHolderMaterialIlluminated);};

	UFUNCTION()
	void SetTheNearestPuzzleBasicShapeActor();

	UFUNCTION()
	void SetupStatusAndMaterial();

	UFUNCTION()
	void SetupTimelineScaleObjAnim();

	UFUNCTION()
	void SetupTimelineSpawnScaleAnim();
	
	UFUNCTION()
	void HandlePointInteractive(bool bStart, FString Name, float Distance);
	
	UFUNCTION()
	void EnableTimelineScaleObjAnim(bool bEnable);

	//UFUNCTION()
	//void EnableTimelineScaleObjAnim(bool bEnable)
	
	UFUNCTION()
	void PostSpawnSetup();

	UFUNCTION()
	void SmoothTeleport(bool bEnable, FVector Target);

	UFUNCTION()
	void StopAnimScale();


	FVector DefaultScaleObj = FVector::Zero();
	
protected:
	
	UFUNCTION()
	void TimelineScaleObjAnim(float Value);
	
	UFUNCTION()
	void TimelineOverlayMaterial(float Value);

	UFUNCTION()
	void TimelineOverlayMaterialEnd();

	UFUNCTION()
	void TimelineSpawnScaleAnim(float Value);


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TWF VR", meta = (AllowPrivateAccess = "true"))
	float SpeedSmoothMove = 500;
	
	UPROPERTY()
	UTimelineComponent* TimelineCompOverlayMaterial;
	
	
	UPROPERTY()
	FOnTimelineFloat HandleTimelineOverMaterial{};

	
};



