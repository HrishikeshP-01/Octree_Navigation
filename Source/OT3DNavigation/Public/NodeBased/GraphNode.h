// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GraphNode.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FGraphNode
{
	GENERATED_BODY()

	FVector center;
	int level;
	TArray<FGraphNode*> neighbors;

	FGraphNode()
	{
		center = FVector::ZeroVector;
		level = 0;
	}
	FGraphNode(FVector c, int l)
	{
		center = c;
		level = l;
	}
};
