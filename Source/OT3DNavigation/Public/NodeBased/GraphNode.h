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
	TArray<FGraphNode*> neighbors;

	FGraphNode()
	{
		center = FVector::ZeroVector;
	}
	FGraphNode(FVector c)
	{
		center = c;
	}
};
