// Fill out your copyright notice in the Description page of Project Settings.


#include "DensityNoise.h"
#include "../Grid/WorldMap.h"


UDensityNoise::UDensityNoise() {
	NoiseModule.SetNoiseType(FastNoise::Perlin);
}

float UDensityNoise::GetDensity(int x, int y, int z, AWorldMap* Map)
{

	return z - NoiseModule.GetNoise(x / Map->ScaleX, y / Map->ScaleY ) * Map->Multiplicator - Map->Offset;


	/*float ab = GetPerlinNoise(x, y);
	float bc = GetPerlinNoise(y, z);
	float ac = GetPerlinNoise(x, z);

	float ba = GetPerlinNoise(y, x);
	float cb = GetPerlinNoise(z, y);
	float ca = GetPerlinNoise(z, x);

	float abc = ab + bc + ac + ba + cb + ca;
	return abc / 6.0f;*/

}

