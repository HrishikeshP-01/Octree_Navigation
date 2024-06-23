// Fill out your copyright notice in the Description page of Project Settings.


#include "NavMath/AStar.h"

UAStar::UAStar()
{
}

UAStar::~UAStar()
{
}

TArray<FVector> UAStar::GetPath(TArray<FGraphNode>& nodes, FGraphNode& startNode, FGraphNode& endNode, bool& doesPathExist)
{
	TArray<FGraphNode*> open;
	TSet<FVector> closed;
	TArray<FVector> path;

	// Find f_cost of all nodes, set parent of nodes to null
	ResetHeuristics(nodes);
	CalculateHeuristics_Euclidean(nodes, startNode, endNode);
	doesPathExist = false;

	open.Add(&startNode);
	FGraphNode* current = nullptr;

	while (open.Num() > 0)
	{
		// Get node from open with least f_cost
		FGraphNode* bestOption = open[0];
		float least_f_cost = open[0]->f_cost;
		for (int i = 0;i < open.Num();i++)
		{
			if (open[i]->f_cost < least_f_cost && !closed.Contains(open[i]->center))
			{
				least_f_cost = open[i]->f_cost;
				bestOption = open[i];
			}
		}

		// Set parent of bestNeighbor to current
		bestOption->parent = current;
		// Set current to bestNeighbor, pop bestNeighbor from open & add bestNeighbor center to closed
		current = bestOption;
		open.Remove(current);
		closed.Add(current->center);

		// Add neighbors of bestNeighbor to open if their centers are not present in closed
		for (int i = 0;i < current->neighbors.Num();i++)
		{
			if (!closed.Contains(current->neighbors[i]->center)) { open.Add(current->neighbors[i]); }
		}
		CalculateHeuristics_Euclidean(nodes, *current, endNode);

		// if current is the destination then calculate path using recursion, exit loop
		if (current == &endNode)
		{
			doesPathExist = true;
			while (current != nullptr)
			{
				path.Add(current->center);
				current = current->parent;
			}
			break;
		}
	}

	return path;
}

void UAStar::ResetHeuristics(TArray<FGraphNode>& nodes)
{
	for (int i = 0;i < nodes.Num();i++)
	{
		nodes[i].parent = nullptr;
		nodes[i].f_cost = 0.0f;
	}
}

void UAStar::CalculateHeuristics_Manhattan(TArray<FGraphNode>& nodes, FGraphNode& startNode, FGraphNode& endNode)
{
	float g_cost, h_cost;
	for (int i = 0;i < nodes.Num();i++)
	{
		g_cost = UNavMath::GetManhattanDistance(nodes[i].center, startNode.center);
		h_cost = UNavMath::GetManhattanDistance(nodes[i].center, endNode.center);
		nodes[i].f_cost = g_cost + h_cost;
	}
}

void UAStar::CalculateHeuristics_Euclidean(TArray<FGraphNode>& nodes, FGraphNode& startNode, FGraphNode& endNode)
{
	float g_cost, h_cost;
	for (int i = 0;i < nodes.Num();i++)
	{
		g_cost = FVector::DistSquared(startNode.center, nodes[i].center);
		h_cost = FVector::DistSquared(nodes[i].center, endNode.center);
		nodes[i].f_cost = g_cost + h_cost;
	}
}