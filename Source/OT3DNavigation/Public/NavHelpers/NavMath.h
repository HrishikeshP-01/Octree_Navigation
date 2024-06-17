// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavMath.generated.h"

/**
 * 
 */
UCLASS()
class OT3DNAVIGATION_API UNavMath : public UObject
{
	GENERATED_BODY()
public:
	UNavMath();
	~UNavMath();

public:
	UFUNCTION()
	static float GetManhattanDistance(FVector& a, FVector& b);
};
