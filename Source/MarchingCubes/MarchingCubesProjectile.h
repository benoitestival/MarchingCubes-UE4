// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MarchingCubesProjectile.generated.h"

class UVoxel;

UCLASS(config=Game)
class AMarchingCubesProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	bool HAsHit{ false };

public:
	AMarchingCubesProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	float GetDistance(FVector A, FVector B);
	void CheckAxis(UVoxel* Voxel, UVoxel* NextVoxel, int rayon, FVector HitLocation, float IsoValue, int Axis);
	FVector GetCrossingPoint(UVoxel* Voxel, UVoxel* NextVoxel, FVector CenterOfCircle, float rayon ,bool IsVoxelCheckInSphere);
	

};

