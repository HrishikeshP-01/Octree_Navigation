// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GraphNode.generated.h"
/**
 * Description: Struct that represents the nodes that form the graph made by un-occupied leaf nodes of the octree
 * Purpose: Store center of un-occupied leaf nodes & the unoccupied spatial neighbors of the node
 */
USTRUCT(BlueprintType)
struct FGraphNode
{
	GENERATED_BODY()

	FVector center;
	int level;
	TArray<FGraphNode*> neighbors;
	// Variables used during path calculation
	FGraphNode* parent;
	float f_cost;

	FGraphNode()
	{
		center = FVector::ZeroVector;
		level = 0;
		parent = nullptr;
		f_cost = 0.0f;
	}
	FGraphNode(FVector c, int l)
	{
		center = c;
		level = l;
		parent = nullptr;
		f_cost = 0.0f;
	}
};
