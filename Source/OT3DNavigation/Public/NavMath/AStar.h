// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NodeBased/GraphNode.h"
#include "NavHelpers/NavMath.h"
#include "AStar.generated.h"

/**
 * 
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
