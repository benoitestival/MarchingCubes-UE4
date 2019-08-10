// Fill out your copyright notice in the Description page of Project Settings.


#include "Voxel.h"
#include "Chunk.h"
#include "WorldMap.h"


//UVoxel::UVoxel(FVector Coordinates, float IsoValue) {
//	Position = Coordinates;
//	Density = IsoValue;
//}

UVoxel::UVoxel() {

}

UVoxel* UVoxel::FindVoxelByDirection(EDirectionEnum Direction)
{
	for (auto DirectionFromMap : this->Chunk->Map->Utils->DirectionMap)
	{
		if (Direction == DirectionFromMap.Key)
		{
			FVector OffsetDirection = DirectionFromMap.Value;
			return this->FindVoxelByOffset(OffsetDirection.X,OffsetDirection.Y,OffsetDirection.Z);
		}
	}
	return nullptr;
}

UVoxel* UVoxel::FindVoxelByOffset(int x, int y, int z)
{
	return this->Chunk->Map->FindVoxelByGridLocation(this->X + x, this->Y + y, this->Z + z);
}

