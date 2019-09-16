// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MarchingCubesProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "Generation/Grid/Chunk.h"
#include "Generation/Grid/WorldMap.h"
#include "Generation/Grid/Voxel.h"

AMarchingCubesProjectile::AMarchingCubesProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AMarchingCubesProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AMarchingCubesProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}
	/*if (!HAsHit)
	{
		HAsHit = true;*/
		if (OtherActor->IsA(AChunk::StaticClass()))
		{
			AChunk* Chunk = Cast<AChunk>(OtherActor);

			if (Chunk->Map->CanDestroy) {
				UVoxel* VoxelHit = Chunk->Map->FindVoxelByWolrdLocation(Hit.Location, Chunk);

				int rayon = Chunk->Map->RayonDig;;


				if (VoxelHit != nullptr)
				{
					if (Chunk->Map->ShowDebugDestroy) {
						DrawDebugSphere(GetWorld(), VoxelHit->WorldPosition, rayon, 26, FColor(255, 0, 0), true, 100, 0, 2);
					}
					int XStart = VoxelHit->X - 10;
					int YStart = VoxelHit->Y - 10;
					int ZStart = VoxelHit->Z - 10;

					for (size_t x = XStart; x < VoxelHit->X + 10; x++)
					{
						for (size_t y = YStart; y < VoxelHit->Y + 10; y++)
						{
							for (size_t z = ZStart; z < VoxelHit->Z + 10; z++)
							{
								UVoxel* VoxelCheck = Chunk->Map->FindVoxelByGridLocation(x, y, z);
								if (GetDistance(VoxelCheck->WorldPosition, VoxelHit->WorldPosition) < rayon)
								{
									VoxelCheck->Density = 0.5;
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
								UVoxel* VoxelTop = Chunk->Map->FindVoxelByGridLocation(x, y, z + 1);
								UVoxel* VoxelRight = Chunk->Map->FindVoxelByGridLocation(x, y + 1, z);
								UVoxel* VoxelForward = Chunk->Map->FindVoxelByGridLocation(x + 1, y, z);
								CheckAxis(VoxelCheck, VoxelForward, rayon, VoxelHit->WorldPosition, Chunk->Map->IsoValue, 0);
								CheckAxis(VoxelCheck, VoxelRight, rayon, VoxelHit->WorldPosition, Chunk->Map->IsoValue, 1);
								CheckAxis(VoxelCheck, VoxelTop, rayon, VoxelHit->WorldPosition, Chunk->Map->IsoValue, 2);
								

							}
						}
					}

					AChunk* ChunkTop = Chunk->FindChunkByDirection(EDirectionEnum::Top);
					AChunk* ChunkBottom = Chunk->FindChunkByDirection(EDirectionEnum::Bottom);
					AChunk* ChunkBehind = Chunk->FindChunkByDirection(EDirectionEnum::Behind);
					AChunk* ChunkForward = Chunk->FindChunkByDirection(EDirectionEnum::Forward);
					AChunk* ChunkLeft = Chunk->FindChunkByDirection(EDirectionEnum::Left);
					AChunk* ChunkRight = Chunk->FindChunkByDirection(EDirectionEnum::Right);

					Chunk->GenerateMesh();

					if (ChunkBehind != nullptr)
					{
						ChunkBehind->GenerateMesh();
					}
					if (ChunkForward != nullptr)
					{
						ChunkForward->GenerateMesh();

					}
					if (ChunkLeft != nullptr)
					{
						ChunkLeft->GenerateMesh();

					}
					if (ChunkRight != nullptr)
					{
						ChunkRight->GenerateMesh();

					}
					if (ChunkTop != nullptr)
					{
						ChunkTop->GenerateMesh();

					}
					if (ChunkBottom != nullptr)
					{
						ChunkBottom->GenerateMesh();

					}
				}
			}
			
		}
	//}
	
}

float AMarchingCubesProjectile::GetDistance(FVector A, FVector B)
{
	return sqrt(pow(B.X - A.X, 2) + pow(B.Y - A.Y, 2) + pow(B.Z - A.Z, 2));
}

void AMarchingCubesProjectile::CheckAxis(UVoxel * Voxel, UVoxel * NextVoxel, int rayon, FVector HitLocation, float IsoValue, int Axis)
{
	bool VoxelInCircle {false};
	bool NextVoxelInCircle {false};
	bool NeedNewdensity {false};
	FVector CrossingPoint {0.0f,0.0f,0.0f};

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

FVector AMarchingCubesProjectile::GetCrossingPoint(UVoxel * Voxel, UVoxel * NextVoxel, FVector CenterOfCircle, float rayon, bool IsVoxelCheckInSphere)
{
	int wantedResult {0};
	float mu {0.0f};
	FVector CrossingPoint {0.0f,0.0f,0.0f};
	float a = pow(NextVoxel->WorldPosition.X - Voxel->WorldPosition.X, 2.0f) + pow(NextVoxel->WorldPosition.Y - Voxel->WorldPosition.Y, 2.0f) + pow(NextVoxel->WorldPosition.Z - Voxel->WorldPosition.Z, 2.0f);
	float b = 2 * ((NextVoxel->WorldPosition.X - Voxel->WorldPosition.X)*(Voxel->WorldPosition.X - CenterOfCircle.X) + (NextVoxel->WorldPosition.Y - Voxel->WorldPosition.Y)*(Voxel->WorldPosition.Y - CenterOfCircle.Y) + (NextVoxel->WorldPosition.Z - Voxel->WorldPosition.Z)*(Voxel->WorldPosition.Z - CenterOfCircle.Z));
	float c = pow(CenterOfCircle.X, 2.0f) + pow(CenterOfCircle.Y, 2.0f) + pow(CenterOfCircle.Z, 2.0f) + pow(Voxel->WorldPosition.X, 2.0f) + pow(Voxel->WorldPosition.Y, 2.0f) + pow(Voxel->WorldPosition.Z, 2.0f) - 2 * (CenterOfCircle.X*Voxel->WorldPosition.X + CenterOfCircle.Y*Voxel->WorldPosition.Y + CenterOfCircle.Z*Voxel->WorldPosition.Z) - pow(rayon, 2.0f);
	if (IsVoxelCheckInSphere) {
		mu = (-b + (sqrt(pow(b, 2.0f) - 4 * a * c))) / (2 * a);
	}
	else{
		mu = (-b - (sqrt(pow(b, 2.0f) - 4 * a * c))) / (2 * a);
	}

	//mu = (-b + (sqrt(pow(b, 2.0f) - 4 * a * c))) / (2*a);
	CrossingPoint.X = Voxel->WorldPosition.X + mu * (NextVoxel->WorldPosition.X - Voxel->WorldPosition.X);
	CrossingPoint.Y = Voxel->WorldPosition.Y + mu * (NextVoxel->WorldPosition.Y - Voxel->WorldPosition.Y);
	CrossingPoint.Z = Voxel->WorldPosition.Z + mu * (NextVoxel->WorldPosition.Z - Voxel->WorldPosition.Z);

	return CrossingPoint;
}

