// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebugOctree.generated.h"

/**
 * 
 */
UCLASS()
class OT3DNAVIGATION_API UDebugOctree : public UObject
{
	GENERATED_BODY()

public:
	UDebugOctree();
	~UDebugOctree();

	// Draw functions
public:
	UFUNCTION()
	static void DrawOctNode(UWorld* world, FVector& center, float side, FColor color);
	UFUNCTION()
		static void DrawPoint(UWorld* world, FVector& center, float radius, FColor color);
};
