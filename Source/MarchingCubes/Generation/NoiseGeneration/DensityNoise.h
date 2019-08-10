// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FastNoise.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DensityNoise.generated.h"

class AWorldMap;


UCLASS()
class MARCHINGCUBES_API UDensityNoise : public UObject
{
	GENERATED_BODY()

private:

public:

	
	FastNoise NoiseModule;

	UDensityNoise();

	UFUNCTION()
	float GetDensity(int x, int y, int z, AWorldMap* Map);

};
