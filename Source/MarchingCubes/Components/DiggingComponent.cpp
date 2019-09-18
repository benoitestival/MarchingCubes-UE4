// Fill out your copyright notice in the Description page of Project Settings.


#include "DiggingComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "../Generation/Grid/Chunk.h"
#include "../Generation/Grid/WorldMap.h"
#include "../Generation/Grid/Voxel.h"


// Sets default values for this component's properties
UDiggingComponent::UDiggingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDiggingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDiggingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDiggingComponent::Dig()
{
	FHitResult HitResult;
	FVector Start(GetOwner()->GetActorLocation());
	FVector End = Start + GetOwner()->GetActorForwardVector() * 1000;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility)) {
		if (HitResult.GetActor()->IsA(AChunk::StaticClass())){
			
			AChunk* Chunk = Cast<AChunk>(HitResult.GetActor());

			if (Chunk->Map->CanDestroy) {
				UVoxel* VoxelHit = Chunk->Map->FindVoxelByWolrdLocation(HitResult.Location, Chunk);

				if (VoxelHit != nullptr)
				{
					if (ShowLog) {
						DrawDebugSphere(GetWorld(), VoxelHit->WorldPosition, Rayon, 26, FColor(255, 0, 0), true, 100, 0, 2);
					}
					int XStart = VoxelHit->X - OffsetForVoxel;
					int YStart = VoxelHit->Y - OffsetForVoxel;
					int ZStart = VoxelHit->Z - OffsetForVoxel;

					for (size_t x = XStart; x < VoxelHit->X + OffsetForVoxel; x++)
					{
						for (size_t y = YStart; y < VoxelHit->Y + OffsetForVoxel; y++)
						{
							for (size_t z = ZStart; z < VoxelHit->Z + OffsetForVoxel; z++)
							{
								UVoxel* VoxelCheck = Chunk->Map->FindVoxelByGridLocation(x, y, z);
								if (VoxelCheck != nullptr){
									if (GetDistance(VoxelCheck->WorldPosition, VoxelHit->WorldPosition) < Rayon){
										VoxelCheck->Density = 0.5;
									}
								}
							}
						}
					}

					for (size_t x = XStart; x < VoxelHit->X + 10; x++)
					{
						for (size_t y = YStart; y < VoxelHit->Y + 10; y++)
						{
							for (size_t z = ZStart; z < VoxelHit->Z + 10; z++)
							{
								UVoxel* VoxelCheck = Chunk->Map->FindVoxelByGridLocation(x, y, z);

								if (VoxelCheck != nullptr){
									UVoxel* VoxelTop = Chunk->Map->FindVoxelByGridLocation(x, y, z + 1);
									UVoxel* VoxelRight = Chunk->Map->FindVoxelByGridLocation(x, y + 1, z);
									UVoxel* VoxelForward = Chunk->Map->FindVoxelByGridLocation(x + 1, y, z);

									ActualizedChunks.AddUnique(VoxelCheck->Chunk);

									if (VoxelTop != nullptr) {
										CheckAxis(VoxelCheck, VoxelTop, Rayon, VoxelHit->WorldPosition, Chunk->Map->IsoValue, 2);
										ActualizedChunks.AddUnique(VoxelTop->Chunk);

									}
									if (VoxelForward != nullptr) {
										CheckAxis(VoxelCheck, VoxelForward, Rayon, VoxelHit->WorldPosition, Chunk->Map->IsoValue, 0);
										ActualizedChunks.AddUnique(VoxelForward->Chunk);

									}
									if (VoxelRight != nullptr) {
										CheckAxis(VoxelCheck, VoxelRight, Rayon, VoxelHit->WorldPosition, Chunk->Map->IsoValue, 1);
										ActualizedChunks.AddUnique(VoxelRight->Chunk);

									}
								}	
							}
						}
					}

					for (auto chunk : ActualizedChunks) {
						if (chunk != nullptr)
						{
							chunk->GenerateMesh();
						}
					}
				}
			}	
		}
	}
}

float UDiggingComponent::GetDistance(FVector A, FVector B)
{
	return sqrt(pow(B.X - A.X, 2) + pow(B.Y - A.Y, 2) + pow(B.Z - A.Z, 2));
}

void UDiggingComponent::CheckAxis(UVoxel * Voxel, UVoxel * NextVoxel, int rayon, FVector HitLocation, float IsoValue, int Axis)
{
	bool VoxelInCircle{ false };
	bool NextVoxelInCircle{ false };
	bool NeedNewdensity{ false };
	FVector CrossingPoint{ 0.0f,0.0f,0.0f };

	if (GetDistance(Voxel->WorldPosition, HitLocation) < rayon) {
		VoxelInCircle = true;

	}
	if (GetDistance(NextVoxel->WorldPosition, HitLocation) < rayon) {
		NextVoxelInCircle = true;
	}

	if (VoxelInCircle || NextVoxelInCircle)
	{
		if (Voxel->Density < 0 || NextVoxel->Density < 0)
		{

			if (VoxelInCircle && !NextVoxelInCircle) {
				CrossingPoint = GetCrossingPoint(Voxel, NextVoxel, HitLocation, rayon, true);
				NeedNewdensity = true;
			}
			else if (!VoxelInCircle && NextVoxelInCircle) {
				CrossingPoint = GetCrossingPoint(Voxel, NextVoxel, HitLocation, rayon, false);
				NeedNewdensity = true;
			}

			if (NeedNewdensity) {

				float muVoxel{ 0.0f };
				float muNextVoxel{ 0.0f };

				switch (Axis)
				{
				case 0:
					muVoxel = (CrossingPoint.X - Voxel->WorldPosition.X) / (NextVoxel->WorldPosition.X - Voxel->WorldPosition.X);
					muNextVoxel = (CrossingPoint.X - NextVoxel->WorldPosition.X) / (Voxel->WorldPosition.X - NextVoxel->WorldPosition.X);
					break;
				case 1:
					muVoxel = (CrossingPoint.Y - Voxel->WorldPosition.Y) / (NextVoxel->WorldPosition.Y - Voxel->WorldPosition.Y);
					muNextVoxel = (CrossingPoint.Y - NextVoxel->WorldPosition.Y) / (Voxel->WorldPosition.Y - NextVoxel->WorldPosition.Y);
					break;
				case 2:
					muVoxel = (CrossingPoint.Z - Voxel->WorldPosition.Z) / (NextVoxel->WorldPosition.Z - Voxel->WorldPosition.Z);
					muNextVoxel = (CrossingPoint.Z - NextVoxel->WorldPosition.Z) / (Voxel->WorldPosition.Z - NextVoxel->WorldPosition.Z);
					break;
				default:
					break;
				}

				if (Voxel->HasBeenDig)
				{
					if (!NextVoxel->HasBeenDig) {
						NextVoxel->Density = (Voxel->Density*muNextVoxel - IsoValue) / (muNextVoxel - 1);
						NextVoxel->HasBeenDig = true;
					}
				}
				else
				{
					Voxel->Density = (NextVoxel->Density*muVoxel - IsoValue) / (muVoxel - 1);
					Voxel->HasBeenDig = true;
				}
			}
		}
	}
}

FVector UDiggingComponent::GetCrossingPoint(UVoxel * Voxel, UVoxel * NextVoxel, FVector CenterOfCircle, float rayon, bool IsVoxelCheckInSphere)
{
	int wantedResult{ 0 };
	float mu{ 0.0f };
	FVector CrossingPoint{ 0.0f,0.0f,0.0f };
	float a = pow(NextVoxel->WorldPosition.X - Voxel->WorldPosition.X, 2.0f) + pow(NextVoxel->WorldPosition.Y - Voxel->WorldPosition.Y, 2.0f) + pow(NextVoxel->WorldPosition.Z - Voxel->WorldPosition.Z, 2.0f);
	float b = 2 * ((NextVoxel->WorldPosition.X - Voxel->WorldPosition.X)*(Voxel->WorldPosition.X - CenterOfCircle.X) + (NextVoxel->WorldPosition.Y - Voxel->WorldPosition.Y)*(Voxel->WorldPosition.Y - CenterOfCircle.Y) + (NextVoxel->WorldPosition.Z - Voxel->WorldPosition.Z)*(Voxel->WorldPosition.Z - CenterOfCircle.Z));
	float c = pow(CenterOfCircle.X, 2.0f) + pow(CenterOfCircle.Y, 2.0f) + pow(CenterOfCircle.Z, 2.0f) + pow(Voxel->WorldPosition.X, 2.0f) + pow(Voxel->WorldPosition.Y, 2.0f) + pow(Voxel->WorldPosition.Z, 2.0f) - 2 * (CenterOfCircle.X*Voxel->WorldPosition.X + CenterOfCircle.Y*Voxel->WorldPosition.Y + CenterOfCircle.Z*Voxel->WorldPosition.Z) - pow(rayon, 2.0f);
	if (IsVoxelCheckInSphere) {
		mu = (-b + (sqrt(pow(b, 2.0f) - 4 * a * c))) / (2 * a);
	}
	else {
		mu = (-b - (sqrt(pow(b, 2.0f) - 4 * a * c))) / (2 * a);
	}

	CrossingPoint.X = Voxel->WorldPosition.X + mu * (NextVoxel->WorldPosition.X - Voxel->WorldPosition.X);
	CrossingPoint.Y = Voxel->WorldPosition.Y + mu * (NextVoxel->WorldPosition.Y - Voxel->WorldPosition.Y);
	CrossingPoint.Z = Voxel->WorldPosition.Z + mu * (NextVoxel->WorldPosition.Z - Voxel->WorldPosition.Z);

	return CrossingPoint;
}

