// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "../../Utils/EnumUtils.h"
#include "Chunk.generated.h"

class UVoxel;
class UMarchingCubesClass;
class AWorldMap;

UCLASS()
class MARCHINGCUBES_API AChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	
	UPROPERTY()
	unsigned int ChunkX {0};

	UPROPERTY()
	unsigned int ChunkY {0};

	UPROPERTY()
	unsigned int ChunkZ {0};

	UPROPERTY()
	unsigned int StartX {0};

	UPROPERTY()
	unsigned int StartY {0};

	UPROPERTY()
	unsigned int StartZ {0};

	UPROPERTY()
	int SizeChunkX {0};

	UPROPERTY()
	int SizeChunkY {0};

	UPROPERTY()
	int SizeChunkZ {0};

	UPROPERTY()
	float IsoValue {0};

	UPROPERTY()
	UProceduralMeshComponent* proceduralComponent {nullptr};

	UPROPERTY()
	AWorldMap* Map {nullptr};

	UPROPERTY()
	UMarchingCubesClass* MarchingCubes {nullptr};

	UPROPERTY()
	TArray<UVoxel*> Voxels;

	UPROPERTY()
	TArray<FVector> Vertices;

	UPROPERTY()
	TArray<FVector> Normales;

	UPROPERTY()
	TArray<int> Triangles;

	AChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform & Transform) override;

	void GenerateMesh();
	void InitializeArray();
	UVoxel* FindVoxelInChunk(int x, int y, int z);
	AChunk* FindChunkByCoord(int x, int y, int z);
	AChunk* FindChunkByDirection(EDirectionEnum Direction);
};
