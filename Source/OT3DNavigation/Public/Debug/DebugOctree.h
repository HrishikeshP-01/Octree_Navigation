// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebugOctree.generated.h"

/**
 * Description: A class for general debug functionality while working with octrees
 * Purpose: Debug functions for octrees
 * Concerns: Learn how to implement libraries in UE
 */
UCLASS()
class OT3DNAVIGATION_API UDebugOctree : public UObject
{
	GENERATED_BODY()

public:
	UDebugOctree();
	~UDebugOctree();

	// Draw functions
	/**
	* Note: Functions are static so as to use them in classes without creating a DebugOctree object
	*/
public:
	UFUNCTION()
		static void DrawOctNode(UWorld* world, FVector& center, float side, FColor color);
	UFUNCTION()
		static void DrawPoint(UWorld* world, FVector& center, float radius, FColor color);
	UFUNCTION()
		static void DrawLine(UWorld* world, FVector& start, FVector& end, FColor color);
};
