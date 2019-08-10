// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"
#include "WorldMap.h"
#include "Voxel.h"
#include "../MarchingCubes/MarchingCubesClass.h"


// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MarchingCubes = NewObject<UMarchingCubesClass>();
	/*mesh = CreateDefaultSubobject<USceneComponent>(TEXT("Mesh"));
	RootComponent = mesh;*/
	
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChunk::OnConstruction(const FTransform & Transform)
{
	proceduralComponent = NewObject<UProceduralMeshComponent>(this, "Voxel");
	proceduralComponent->RegisterComponent();
	RootComponent = proceduralComponent;
	RootComponent->SetWorldTransform(Transform);

	Super::OnConstruction(Transform);
}

void AChunk::GenerateMesh()
{
	
	TArray<FVector2D> UVs;
	TArray<FColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	Vertices.Empty();
	Triangles.Empty();
	Normales.Empty();

	for (size_t x = 0; x < SizeChunkX; x++)
	{
		for (size_t y = 0; y < SizeChunkY; y++)
		{
			for (size_t z = 0; z < SizeChunkZ; z++)
			{
				UVoxel* Voxel = FindVoxelInChunk(x,y,z);
				if (Voxel != nullptr)
				{
					UVoxel* VoxelRight = Voxel->FindVoxelByDirection(EDirectionEnum::Right);
					UVoxel* VoxelTop = Voxel->FindVoxelByDirection(EDirectionEnum::Top);
					UVoxel* VoxelTopRight = Voxel->FindVoxelByDirection(EDirectionEnum::TopRight);
					UVoxel* VoxelForward = Voxel->FindVoxelByDirection(EDirectionEnum::Forward);
					UVoxel* VoxelForwardRight = Voxel->FindVoxelByDirection(EDirectionEnum::ForwardRight);
					UVoxel* VoxelForwardTop = Voxel->FindVoxelByDirection(EDirectionEnum::ForwardTop);
					UVoxel* VoxelForwardTopRight = Voxel->FindVoxelByDirection(EDirectionEnum::ForwardTopRight);
					if (VoxelRight != nullptr && VoxelTop != nullptr && VoxelTopRight != nullptr && VoxelForward != nullptr && VoxelForwardRight != nullptr && VoxelForwardTop != nullptr && VoxelForwardTopRight != nullptr)
					{
						TArray<UVoxel*> CubeVoxels;
						CubeVoxels.Add(VoxelForward);
						CubeVoxels.Add(VoxelForwardRight);
						CubeVoxels.Add(VoxelRight);
						CubeVoxels.Add(Voxel);
						CubeVoxels.Add(VoxelForwardTop);
						CubeVoxels.Add(VoxelForwardTopRight);
						CubeVoxels.Add(VoxelTopRight);
						CubeVoxels.Add(VoxelTop);
						MarchingCubes->March(CubeVoxels, IsoValue, this);

					}
				}
			}
		}
	}
	proceduralComponent->ClearAllMeshSections();
	proceduralComponent->CreateMeshSection(0, Vertices, Triangles, Normales, UVs, VertexColors, Tangents, true);

}

void AChunk::InitializeArray()
{
	Vertices.Init(FVector(0,0,0), 0);
	Triangles.Init(0, 0);
}

UVoxel * AChunk::FindVoxelInChunk(int x, int y, int z)
{
	if (x >= 0 && y >= 0 && z >= 0 && x <= SizeChunkX - 1 && y <= SizeChunkY - 1 && z <= SizeChunkZ - 1)
	{
		int indexVoxel = x * (SizeChunkY*SizeChunkZ) + y * SizeChunkZ + z;
		UVoxel* Voxel = Voxels[indexVoxel];
		return Voxel;
	}
	else
	{
		return nullptr;
	}
	
}

AChunk * AChunk::FindChunkByCoord(int x, int y, int z)
{
	if (x >= 0 && y >= 0 && z >= 0 && x <= Map->nbrChunkX -1  && y <= Map->nbrChunkY -1  && z <= Map->nbrChunkZ - 1)
	{
		int indexChunk = (x * Map->TotalFaceChunks) + (y  * Map->nbrChunkZ) + z;
		AChunk* Chunk = Map->Chunks[indexChunk];
		return Chunk;
	}
	else
	{
		return nullptr;
	}
}

AChunk * AChunk::FindChunkByDirection(EDirectionEnum Direction)
{

	for (auto DirectionFromMap : this->Map->Utils->DirectionMap)
	{
		if (Direction == DirectionFromMap.Key)
		{
			FVector OffsetDirection = DirectionFromMap.Value;
			return this->FindChunkByCoord(this->ChunkX + OffsetDirection.X, this->ChunkY + OffsetDirection.Y, this->ChunkZ + OffsetDirection.Z);
		}
	}
	return nullptr;
}



