// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnumUtils.generated.h"

UENUM(BlueprintType)
enum class EDirectionEnum : uint8
{
	Top UMETA(DisplayName = "Top"),

	TopRight UMETA(DisplayName = "TopRight"),

	Right UMETA(DisplayName = "Right"),

	BottomRight UMETA(DisplayName = "BottomRight"),

	Bottom UMETA(DisplayName = "Bottom"),

	BottomLeft UMETA(DisplayName = "BottomLeft"),

	Left UMETA(DisplayName = "Left"),

	TopLeft UMETA(DisplayName = "TopLeft"),

	Forward UMETA(DispalyName = "Forward"),

	ForwardTop UMETA(DisplayName = "ForwardTop"),

	ForwardTopRight UMETA(DisplayName = "ForwardTopRight"),

	ForwardRight UMETA(DisplayName = "ForwardRight"),

	ForwardBottomRight UMETA(DisplayName = "ForwardBottomRight"),

	ForwardBottom UMETA(DisplayName = "ForwardBottom"),

	ForwardBottomLeft UMETA(DisplayName = "ForwardBottomLeft"),

	ForwardLeft UMETA(DisplayName = "ForwardLeft"),

	ForwardTopLeft UMETA(DisplayName = "ForwardTopLeft"),

	Behind UMETA(DisplayName = "Behind"),

	BehindTop UMETA(DisplayName = "BehindTop"),

	BehindTopRight UMETA(DisplayName = "BehindTopRight"),

	BehindRight UMETA(DisplayName = "BehindRight"),

	BehindBottomRight UMETA(DisplayName = "BehindBottomRight"),

	BehindBottom UMETA(DisplayName = "BehindBottom"),

	BehindBottomLeft UMETA(DisplayName = "BehindBottomLeft"),

	BehindLeft UMETA(DisplayName = "BehindLeft"),

	BehindTopLeft UMETA(DisplayName = "BehindTopLeft")
};


UCLASS()
class MARCHINGCUBES_API UEnumUtils : public UObject
{
	GENERATED_BODY()
	
public:

	TMap<EDirectionEnum, FVector> DirectionMap{
		{EDirectionEnum::Top, FVector(0,0,1)},
		{EDirectionEnum::TopRight, FVector(0,1,1)},
		{EDirectionEnum::Right, FVector(0,1,0)},
		{EDirectionEnum::BottomRight, FVector(0,1,-1)},
		{EDirectionEnum::Bottom, FVector(0,0,-1)},
		{EDirectionEnum::BottomLeft, FVector(0,-1,-1)},
		{EDirectionEnum::Left, FVector(0,-1,0)},
		{EDirectionEnum::TopLeft, FVector(0,-1,1)},
		{EDirectionEnum::Forward, FVector(1,0,0)},
		{EDirectionEnum::ForwardTop, FVector(1,0,1)},
		{EDirectionEnum::ForwardTopRight, FVector(1,1,1)},
		{EDirectionEnum::ForwardRight, FVector(1,1,0)},
		{EDirectionEnum::ForwardBottomRight, FVector(1,1,-1)},
		{EDirectionEnum::ForwardBottom, FVector(1,0,-1)},
		{EDirectionEnum::ForwardBottomLeft, FVector(1,-1,-1)},
		{EDirectionEnum::ForwardLeft, FVector(1,-1,0)},
		{EDirectionEnum::ForwardTopLeft, FVector(1,-1,1)},
		{EDirectionEnum::Behind, FVector(-1,0,0)},
		{EDirectionEnum::BehindTop, FVector(-1,0,1)},
		{EDirectionEnum::BehindTopRight, FVector(-1,1,1)},
		{EDirectionEnum::BehindRight, FVector(-1,1,0)},
		{EDirectionEnum::BehindBottomRight, FVector(-1,1,-1)},
		{EDirectionEnum::BehindBottom, FVector(-1,0,-1)},
		{EDirectionEnum::BehindBottomLeft, FVector(-1,-1,-1)},
		{EDirectionEnum::BehindLeft, FVector(-1,-1,0)},
		{EDirectionEnum::BehindTopLeft, FVector(-1,-1,1)}
	};
};
