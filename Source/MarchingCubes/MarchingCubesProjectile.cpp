// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MarchingCubesProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
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
	if (!HAsHit)
	{
		HAsHit = true;
		if (OtherActor->IsA(AChunk::StaticClass()))
		{
			AChunk* Chunk = Cast<AChunk>(OtherActor);

			if (Chunk->Map->CanDestroy) {
				UVoxel* VoxelHit = Chunk->Map->FindVoxelByWolrdLocation(Hit.Location, Chunk);

				int rayon = 100;


				if (VoxelHit != nullptr)
				{
					int XStart = VoxelHit->X - 3;
					int YStart = VoxelHit->Y - 3;
					int ZStart = VoxelHit->Z - 3;

					for (size_t x = XStart; x < VoxelHit->X + 3; x++)
					{
						for (size_t y = YStart; y < VoxelHit->Y + 3; y++)
						{
							for (size_t z = ZStart; z < VoxelHit->Z + 3; z++)
							{
								UVoxel* VoxelCheck = Chunk->Map->FindVoxelByGridLocation(x, y, z);
								if (VoxelCheck != nullptr)
								{
									float distance = GetDistance(VoxelHit->WorldPosition, VoxelCheck->WorldPosition);
									if (distance < rayon)
									{
										VoxelCheck->Density = 5;

									}
								}

							}
						}
					}

					/*for (size_t x = XStart; x < VoxelHit->X + 3; x++)
					{
						for (size_t y = YStart; y < VoxelHit->Y + 3; y++)
						{
							for (size_t z = ZStart; z < VoxelHit->Z + 3; z++)
							{
								UVoxel* VoxelCheck = Chunk->Map->FindVoxelByGridLocation(x, y, z);
								if (VoxelCheck != nullptr)
								{
									UVoxel* VoxelForward = VoxelCheck->FindVoxelByDirection(EDirectionEnum::Forward);
									UVoxel* VoxelRight = VoxelCheck->FindVoxelByDirection(EDirectionEnum::Right);
									UVoxel* VoxelTop = VoxelCheck->FindVoxelByDirection(EDirectionEnum::Top);

									if (VoxelForward != nullptr)
									{
										CreateXCrossing(VoxelCheck, VoxelForward, rayon, Hit.Location);

									}
									if (VoxelRight != nullptr)
									{
										CreateYCrossing(VoxelCheck, VoxelRight, rayon, Hit.Location);

									}
									if (VoxelTop != nullptr)
									{
										CreateZCrossing(VoxelCheck, VoxelTop, rayon, Hit.Location);

									}
								}

							}
						}
					}*/

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
	}
	
}

float AMarchingCubesProjectile::GetDistance(FVector A, FVector B)
{
	return sqrt(pow(B.X - A.X, 2) + pow(B.Z - A.Z, 2));
}

void AMarchingCubesProjectile::CreateXCrossing(UVoxel * Voxel, UVoxel * NextVoxelForward, int rayon, FVector HitLocation)
{

	int RayonCarre = rayon * rayon;
	if (Voxel->Density < NextVoxelForward->Density)
	{
		float z2 = Voxel->WorldPosition.Z - HitLocation.Z;
		z2 *= z2;
		if (Voxel->Density < 0)
		{
			float x = Voxel->WorldPosition.X - HitLocation.X;
			if (x * x + z2 <= RayonCarre)
			{
				x = HitLocation.X + sqrt(RayonCarre - z2);

				float XDiff = abs(NextVoxelForward->X - x);
				NextVoxelForward->Density = XDiff/Voxel->Chunk->Map->SizeVoxel;



			}
		}
		if (NextVoxelForward->Density < 0)
		{
			float x = NextVoxelForward->WorldPosition.X - HitLocation.X;
			if (x * x + z2 <= RayonCarre)
			{
				x = HitLocation.X + sqrt(RayonCarre - z2);

				float XDiff = abs(Voxel->X - x);
				Voxel->Density = XDiff / Voxel->Chunk->Map->SizeVoxel;

			}
		}
	}

}

void AMarchingCubesProjectile::CreateYCrossing(UVoxel * Voxel, UVoxel * NextVoxelRight, int rayon, FVector HitLocation)
{


	int RayonCarre = rayon * rayon;
	if (Voxel->Density < NextVoxelRight->Density)
	{
		float z2 = Voxel->WorldPosition.Z - HitLocation.Z;
		z2 *= z2;
		if (Voxel->Density < 0)
		{
			float y = Voxel->WorldPosition.Y - HitLocation.Y;
			if (y * y + z2 <= RayonCarre)
			{
				y = HitLocation.Y + sqrt(RayonCarre - z2);

				float YDiff = abs(NextVoxelRight->Y - y);
				NextVoxelRight->Density = YDiff / Voxel->Chunk->Map->SizeVoxel;

			}
		}
		if (NextVoxelRight->Density < 0)
		{
			float y = NextVoxelRight->WorldPosition.Y - HitLocation.Y;
			if (y * y + z2 <= RayonCarre)
			{
				y = HitLocation.Y + sqrt(RayonCarre - z2);
				
				float YDiff = abs(Voxel->Y - y);
				Voxel->Density = YDiff / Voxel->Chunk->Map->SizeVoxel;

			}
		}
	}
}

void AMarchingCubesProjectile::CreateZCrossing(UVoxel * Voxel, UVoxel * NextVoxelTop, int rayon, FVector HitLocation)
{


	int RayonCarre = rayon * rayon;
	if (Voxel->Density < NextVoxelTop->Density)
	{
		float y2 = Voxel->WorldPosition.Y - HitLocation.Y;
		y2 *= y2;
		if (Voxel->Density < 0)
		{
			float z = Voxel->WorldPosition.Z - HitLocation.Z;
			if (z * z + y2 <= RayonCarre)
			{
				z = HitLocation.Z + sqrt(RayonCarre - y2);

				float ZDiff = abs(NextVoxelTop->Z - z);
				NextVoxelTop->Density = ZDiff / Voxel->Chunk->Map->SizeVoxel;

			}
		}
		if (NextVoxelTop->Density < 0)
		{
			float z = NextVoxelTop->WorldPosition.Z - HitLocation.Z;
			if (z * z + y2 <= RayonCarre)
			{
				z = HitLocation.Y + sqrt(RayonCarre - y2);
				float ZDiff = abs(Voxel->Z - z);
				Voxel->Density = ZDiff / Voxel->Chunk->Map->SizeVoxel;

			}
		}
	}
}
