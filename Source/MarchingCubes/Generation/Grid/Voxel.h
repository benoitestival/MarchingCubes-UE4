// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../Utils/EnumUtils.h"
#include "Voxel.generated.h"

class AChunk;

/**
 * 
 */
UCLASS()
class MARCHINGCUBES_API UVoxel : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	bool HasBeenDig {false};

	UPROPERTY()
	unsigned int X {0};

	UPROPERTY()
	unsigned int Y {0};

	UPROPERTY()
	unsigned int Z {0};

	UPROPERTY()
	float Density {0.0f};

	UPROPERTY()
	AChunk* Chunk {nullptr};

	UPROPERTY()
	FVector WorldPosition {0.0f, 0.0f, 0.0f};

	UVoxel();

	UVoxel* FindVoxelByDirection(EDirectionEnum Direction);

	UVoxel* FindVoxelByOffset(int x, int y, int z);
};

