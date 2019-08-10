// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Utils/EnumUtils.h"
#include "GameFramework/Actor.h"
#include "WorldMap.generated.h"

class AChunk;
class UVoxel;
class UDensityNoise;

UCLASS()
class MARCHINGCUBES_API AWorldMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldMap();

	UPROPERTY()
	bool ReadyForGridGeneration {false};

	UPROPERTY()
	bool ReadyForMeshGeneration {false};

	UPROPERTY(Category = "Noise", EditAnywhere)
	float ScaleX {0.1f};

	UPROPERTY(Category = "Noise", EditAnywhere)
	float ScaleY {0.1f};

	UPROPERTY(Category = "Noise", EditAnywhere)
	int Multiplicator {50};

	UPROPERTY(Category = "Noise", EditAnywhere)
	int Offset {10};

	UPROPERTY(Category ="MapSize" , EditAnywhere)
	int nbrChunkX {1};

	UPROPERTY(Category = "MapSize", EditAnywhere)
	int nbrChunkY {1};

	UPROPERTY(Category = "MapSize", EditAnywhere)
	int nbrChunkZ {1};

	UPROPERTY(Category = "MapSize", EditAnywhere)
	int SizeChunkX {4};

	UPROPERTY(Category = "MapSize", EditAnywhere)
	int SizeChunkY {4};

	UPROPERTY(Category = "MapSize", EditAnywhere)
	int SizeChunkZ {4};

	UPROPERTY(Category = "MapSize", EditAnywhere)
	unsigned int SizeVoxel {40};

	UPROPERTY(Category = "Other", EditAnywhere)
	bool CanDestroy {true};

	UPROPERTY()
	unsigned int TotalChunkFaceElements {0};

	UPROPERTY()
	unsigned int TotalChunkElements {0};

	UPROPERTY()
	unsigned int TotalFaceChunks {0};

	UPROPERTY()
	unsigned int TotalChunks {0};

	UPROPERTY()
	UDensityNoise* DensityGenerator {nullptr};

	UPROPERTY()
	UEnumUtils* Utils {nullptr};

	UPROPERTY()
	USceneComponent* mesh {nullptr};

	UPROPERTY()
	TArray<AChunk*> Chunks;

	UPROPERTY()
	float Density [8] {-1,-1,-1,1,1,1,1,1};
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	void GenerateChunk();
	void FillChunk();
	void GenerateMapMesh();
	FVector MakeVoxelWorldPosition(int x, int y, int z);
	UVoxel* FindVoxelByGridLocation(int x, int y, int z);
	UVoxel* FindVoxelByWolrdLocation(FVector location, AChunk* chunk);
};
