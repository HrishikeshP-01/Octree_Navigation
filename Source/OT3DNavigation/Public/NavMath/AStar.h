// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NodeBased/GraphNode.h"
#include "NavHelpers/NavMath.h"
#include "AStar.generated.h"

/**
 * Description: Class with functions related to A* navigation for node-based implementation of octrees
 * Purpose: Find the shortest path using A* for node-based implementation of octrees
 * Concerns: Performance can be improved using heaps, multi-threading etc.
 */
UCLASS()
class OT3DNAVIGATION_API UAStar : public UObject
{
	GENERATED_BODY()
public:
	UAStar();
	~UAStar();

public:
	static TArray<FVector> GetPath(TArray<FGraphNode>& nodes, FGraphNode& startNode, FGraphNode& endNode, bool& doesPathExist);
	static void ResetHeuristics(TArray<FGraphNode>& nodes);
	static void CalculateHeuristics_Manhattan(TArray<FGraphNode>& nodes, FGraphNode& startNode, FGraphNode& endNode);
	static void CalculateHeuristics_Euclidean(TArray<FGraphNode>& nodes, FGraphNode& startNode, FGraphNode& endNode);
};
