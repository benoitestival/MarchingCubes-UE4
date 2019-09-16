// Fill out your copyright notice in the Description page of Project Settings.


#include "DensityNoise.h"
#include "../Grid/WorldMap.h"


UDensityNoise::UDensityNoise() {
	NoiseModule.SetNoiseType(FastNoise::Perlin);
}

float UDensityNoise::GetDensity(int x, int y, int z, AWorldMap* Map)
{
	float frequency {1.0f};
	float amplitude {1.0f};
	float noiseHeight {0.0f};

	for (size_t i = 0; i < Map->octaves; i++)
	{
		float Noise = z - NoiseModule.GetNoise(x / Map->ScaleX * frequency, y / Map->ScaleY * frequency) * Map->HeightMultiplicator - (Map->HeightMultiplicator / 2);
		noiseHeight += Noise * amplitude;
		amplitude *= Map->persistance;
		frequency *= Map->lacunarity;
	}
	
	//Clamp
	if (noiseHeight > 100.0f)
	{
		noiseHeight = 100.0f;
	}
	if (noiseHeight < -100.0f)
	{
		noiseHeight = -100.0f;
	}
	noiseHeight /= 100;

	if (Map->ShowLogNoise)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f"), noiseHeight);
	}
	
	if (Map->UseRectShape) {
		if (noiseHeight < 0)
		{
			noiseHeight = -1;
		}
		if (noiseHeight > 0)
		{
			noiseHeight = 1;
		}
	}

	return noiseHeight;

}

