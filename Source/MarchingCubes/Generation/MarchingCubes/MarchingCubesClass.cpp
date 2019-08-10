// Fill out your copyright notice in the Description page of Project Settings.


#include "MarchingCubesClass.h"
#include "../Grid/Chunk.h"
#include "../Grid/Voxel.h"
#include "../Grid/WorldMap.h"

UMarchingCubesClass::UMarchingCubesClass() {

}

void UMarchingCubesClass::March(TArray<UVoxel*>& Voxels, float isoValue, AChunk* chunk)
{
	int cubeIndex = CalculateCubeIndex(Voxels, isoValue);
	if (cubeIndex != 0 && cubeIndex != 255)
	{
		GenerateCubeShape(Voxels, cubeIndex, chunk, isoValue);
	}
}

int UMarchingCubesClass::CalculateCubeIndex(TArray<UVoxel*>& Voxels, float isoValue)
{
	int cubeIndex {0};

	for (size_t i = 0; i < Voxels.Num(); i++)
	{
		if (Voxels[i]->Density < isoValue)
		{
			cubeIndex += pow(2,i);
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("%d"), cubeIndex);
	return cubeIndex;
}

void UMarchingCubesClass::GenerateCubeShape(TArray<UVoxel*>& Voxels, int cubeIndex, AChunk * chunk, float isoValue)
{
	int VerticeIndex = chunk->Vertices.Num();
	FVector TempVertices [12];

	if (edgeTable[cubeIndex] & 1)
	{
		TempVertices[0] = (CalculateInterpolation(Voxels[0], Voxels[1], isoValue));
	}
	if (edgeTable[cubeIndex] & 2)
	{
		TempVertices[1] = (CalculateInterpolation(Voxels[1], Voxels[2], isoValue));
	}
	if (edgeTable[cubeIndex] & 4)
	{
		TempVertices[2] = (CalculateInterpolation(Voxels[2], Voxels[3], isoValue));
	}
	if (edgeTable[cubeIndex] & 8)
	{
		TempVertices[3] = (CalculateInterpolation(Voxels[3], Voxels[0], isoValue));
	}
	if (edgeTable[cubeIndex] & 16)
	{
		TempVertices[4] = (CalculateInterpolation(Voxels[4], Voxels[5], isoValue));
	}
	if (edgeTable[cubeIndex] & 32)
	{
		TempVertices[5] = (CalculateInterpolation(Voxels[5], Voxels[6], isoValue));
	}
	if (edgeTable[cubeIndex] & 64)
	{
		TempVertices[6] = (CalculateInterpolation(Voxels[6], Voxels[7], isoValue));
	}
	if (edgeTable[cubeIndex] & 128)
	{
		TempVertices[7] = (CalculateInterpolation(Voxels[7], Voxels[4], isoValue));
	}
	if (edgeTable[cubeIndex] & 256)
	{
		TempVertices[8] = (CalculateInterpolation(Voxels[0], Voxels[4], isoValue));
	}
	if (edgeTable[cubeIndex] & 512)
	{
		TempVertices[9] = (CalculateInterpolation(Voxels[1], Voxels[5], isoValue));
	}
	if (edgeTable[cubeIndex] & 1024)
	{
		TempVertices[10] = (CalculateInterpolation(Voxels[2], Voxels[6], isoValue));
	}
	if (edgeTable[cubeIndex] & 2048)
	{
		TempVertices[11] = (CalculateInterpolation(Voxels[3], Voxels[7], isoValue));
	}

	for (size_t i = 0; triTable[cubeIndex][i] != -1; i += 3) {

		chunk->Vertices.Add(TempVertices[triTable[cubeIndex][i]]);
		chunk->Triangles.Add(VerticeIndex);
		VerticeIndex++;

		chunk->Vertices.Add(TempVertices[triTable[cubeIndex][i + 1]]);
		chunk->Triangles.Add(VerticeIndex);
		VerticeIndex++;

		chunk->Vertices.Add(TempVertices[triTable[cubeIndex][i + 2]]);
		chunk->Triangles.Add(VerticeIndex);
		VerticeIndex++;

		FVector Vertice1 = TempVertices[triTable[cubeIndex][i]];
		FVector Vertice2 = TempVertices[triTable[cubeIndex][i + 1]];
		FVector Vertice3 = TempVertices[triTable[cubeIndex][i + 2]];

		FVector AB(Vertice2.X - Vertice1.X, Vertice2.Y - Vertice1.Y, Vertice2.Z - Vertice1.Z);
		FVector AC(Vertice3.X - Vertice1.X, Vertice3.Y - Vertice1.Y, Vertice3.Z - Vertice1.Z);
		FVector NormaleTriangle = AB ^ AC;
		NormaleTriangle.Normalize(1.0f);

		chunk->Normales.Add(-NormaleTriangle);
		chunk->Normales.Add(-NormaleTriangle);
		chunk->Normales.Add(-NormaleTriangle);
			
	}
}

FVector UMarchingCubesClass::CalculateInterpolation(UVoxel * Voxel1, UVoxel * Voxel2, float isoValue)
{
	FVector Vertice;
	if (abs(isoValue - Voxel1->Density) < 0.000001f)
	{
		return Voxel1->WorldPosition;
	}
	if (abs(isoValue - Voxel2->Density) < 0.000001f)
	{
		return Voxel2->WorldPosition;
	}
	if (abs(Voxel1->Density - Voxel2->Density) < 0.000001f)
	{
		return Voxel1->WorldPosition;
	}
	float mu = (isoValue - Voxel1->Density) / (Voxel2->Density - Voxel1->Density);
	Vertice.X = Voxel1->WorldPosition.X + mu * (Voxel2->WorldPosition.X - Voxel1->WorldPosition.X);
	Vertice.Y = Voxel1->WorldPosition.Y + mu * (Voxel2->WorldPosition.Y - Voxel1->WorldPosition.Y);
	Vertice.Z = Voxel1->WorldPosition.Z + mu * (Voxel2->WorldPosition.Z - Voxel1->WorldPosition.Z);
	return Vertice;
}
