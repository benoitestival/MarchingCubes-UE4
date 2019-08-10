// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldMap.h"
#include "Chunk.h" 
#include "Voxel.h"
#include "../NoiseGeneration/DensityNoise.h" 
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
AWorldMap::AWorldMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<USceneComponent>(TEXT("Mesh"));
	RootComponent = mesh;
}

// Called when the game starts or when spawned
void AWorldMap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorldMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorldMap::PostInitializeComponents()
{
	Chunks.Init(nullptr, 0);
	Super::PostInitializeComponents();
	DensityGenerator = NewObject<UDensityNoise>();
	Utils = NewObject<UEnumUtils>();
	GenerateChunk();
	FillChunk();
	GenerateMapMesh();
	UE_LOG(LogTemp, Warning, TEXT("Hello"));
}

void AWorldMap::GenerateChunk()
{
	if (nbrChunkX >= 1 && nbrChunkY >= 1 && nbrChunkZ >= 1)
	{
		if (nbrChunkX == nbrChunkY)
		{
			FVector ActorLocation = GetActorLocation();
			FRotator ActorRotation(0, 0, 0);
			AChunk* Chunk = NewObject<AChunk>();
			UWorld *WRLD = GetWorld();
			TotalChunkFaceElements = SizeChunkZ * SizeChunkY;
			TotalChunkElements = TotalChunkFaceElements * SizeChunkX;
			TotalFaceChunks = nbrChunkZ * nbrChunkY;
			TotalChunks = TotalFaceChunks * nbrChunkX;
			
			for (size_t x = 0; x < nbrChunkX ; x++)
			{
				for (size_t y = 0; y < nbrChunkY; y++)
				{
					for (size_t z = 0; z < nbrChunkZ; z++)
					{
						//Expiremental
						//FVector ActorLocation(GetActorLocation().X * x * SizeVoxel, GetActorLocation().Y * y * SizeVoxel, GetActorLocation().Z * z * SizeVoxel);
						AActor* Actor = WRLD->SpawnActor(AChunk::StaticClass(), &ActorLocation, &ActorRotation);
						Chunk = Cast<AChunk>(Actor);
						Chunk->Map = this;
						Chunk->ChunkX = x;
						Chunk->ChunkY = y;
						Chunk->ChunkZ = z;
						Chunk->StartX = x * SizeChunkX;
						Chunk->StartY = y * SizeChunkY;
						Chunk->StartZ = z * SizeChunkZ;
						Chunk->SizeChunkX = SizeChunkX;
						Chunk->SizeChunkY = SizeChunkY;
						Chunk->SizeChunkZ = SizeChunkZ;
						Chunks.Add(Chunk);
					}
				}
			}
			ReadyForGridGeneration = true;
		}
		else
		{
			ReadyForGridGeneration = false;
		}
	}
	else
	{
		ReadyForGridGeneration = false;
	}
}

void AWorldMap::FillChunk() 
{
	int i {0};
	if (ReadyForGridGeneration)
	{
		for (auto chunk : Chunks)
		{
			for (size_t x = chunk->StartX; x < chunk->StartX + SizeChunkX; x++)
			{
				for (size_t y = chunk->StartY; y < chunk->StartY + SizeChunkY; y++)
				{
					for (size_t z = chunk->StartZ; z < chunk->StartZ + SizeChunkZ; z++)
					{
						UVoxel* Voxel = NewObject<UVoxel>();
						Voxel->X = x;
						Voxel->Y = y;
						Voxel->Z = z;
						Voxel->Chunk = chunk;
						Voxel->WorldPosition = MakeVoxelWorldPosition(x,y,z);
						Voxel->Density = DensityGenerator->GetDensity(x,y,z,this);
						chunk->Voxels.Add(Voxel);
					}
				}
				
			}
		}
		ReadyForMeshGeneration = true;
	}
}

void AWorldMap::GenerateMapMesh()
{
	if (ReadyForMeshGeneration)
	{
		for (auto chunk: Chunks)
		{
			chunk->GenerateMesh();
		}
	}
}

FVector AWorldMap::MakeVoxelWorldPosition(int x, int y, int z)
{
	return FVector(GetActorLocation().X + x * SizeVoxel, GetActorLocation().Y + y * SizeVoxel, GetActorLocation().Z + z * SizeVoxel);
}

UVoxel* AWorldMap::FindVoxelByGridLocation(int x, int y, int z)
{
	
	if (x >= 0 && y >= 0 && z >= 0 && x <= SizeChunkX*nbrChunkX - 1 && y <= SizeChunkY*nbrChunkY -1 && z <= SizeChunkZ*nbrChunkZ -1)
	{
		int indexChunk = (x / SizeChunkX * TotalFaceChunks) + (y / SizeChunkY * nbrChunkZ) + z / SizeChunkZ;
		AChunk* Chunk = Chunks[indexChunk];
		int indexVoxel = (x - Chunk->StartX) * TotalChunkFaceElements + (y - Chunk->StartY) * SizeChunkZ + (z - Chunk->StartZ);
		UVoxel* Voxel = Chunk->Voxels[indexVoxel];
		return Voxel;
	}
	else
	{
		return nullptr;
	}
}

UVoxel * AWorldMap::FindVoxelByWolrdLocation(FVector location, AChunk * chunk)
{
	if (chunk != nullptr)
	{
		FVector ChunkLocation = chunk->Voxels[0]->WorldPosition;
		FVector LocationResizeToChunk = location - ChunkLocation;
		int X = LocationResizeToChunk.X / SizeVoxel;
		int Y = LocationResizeToChunk.Y / SizeVoxel;
		int Z = LocationResizeToChunk.Z / SizeVoxel;
		return chunk->FindVoxelInChunk(X,Y,Z);
	}
	else
	{
		return nullptr;
	}
	
}





