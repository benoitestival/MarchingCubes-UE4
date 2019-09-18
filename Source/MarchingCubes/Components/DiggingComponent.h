// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DiggingComponent.generated.h"

class UVoxel;
class AChunk;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MARCHINGCUBES_API UDiggingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDiggingComponent();

	UPROPERTY(EditAnywhere)
	bool ShowLog {false};

	UPROPERTY(EditAnywhere)
	int OffsetForVoxel {10};

	UPROPERTY(EditAnywhere)
	float Rayon {150.0f};

	UPROPERTY()
	TArray<AChunk*> ActualizedChunks;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Dig();
	float GetDistance(FVector A, FVector B);
	void CheckAxis(UVoxel* Voxel, UVoxel* NextVoxel, int rayon, FVector HitLocation, float IsoValue, int Axis);
	FVector GetCrossingPoint(UVoxel* Voxel, UVoxel* NextVoxel, FVector CenterOfCircle, float rayon, bool IsVoxelCheckInSphere);
		
};
