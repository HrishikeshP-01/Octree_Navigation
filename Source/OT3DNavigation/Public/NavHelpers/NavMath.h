// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavMath.generated.h"

/**
 * Description: Class that implements math-based functionality useful for solving problems related to navigation
 * Purpose: General function useful for solving problems related to navigation
 * Concerns: 
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
	UFUNCTION()
		static void GetVertices(FVector& center, float side, TArray<FVector>& vert);
};
