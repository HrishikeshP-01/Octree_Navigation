// Fill out your copyright notice in the Description page of Project Settings.
/**
 * Description: Class with functions related to A* navigation for node-based implementation of octrees
 * Purpose: Find the shortest path using A* for node-based implementation of octrees
 * Concerns: Performance can be improved using heaps, multi-threading etc.
 */

#include "NavMath/AStar.h"

UAStar::UAStar()
{
}

UAStar::~UAStar()
{
}

// Purpose: Find the shortest path between 2 nodes
TArray<FVector> UAStar::GetPath(TArray<FGraphNode>& nodes, FGraphNode& startNode, FGraphNode& endNode, bool& doesPathExist)
{
	TArray<FGraphNode*> open;
	TSet<FVector> closed;
	TArray<FVector> path;

	// Set parent of nodes to null & then calculate the f_cost for all nodes
	ResetHeuristics(nodes);
	CalculateHeuristics_Euclidean(nodes, startNode, endNode);
	doesPathExist = false;

	open.Add(&startNode);
	FGraphNode* current = nullptr;

	// As long as there are reachable nodes to be explored keep running the loop
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

		// Set parent of bestOption to current
		bestOption->parent = current;
		// Set current to bestNeighbor, remove bestOption from open & add bestOption center to closed
		current = bestOption;
		open.Remove(current);
		closed.Add(current->center);

		// Add neighbors of bestOption to open if their centers are not already present in closed
		for (int i = 0;i < current->neighbors.Num();i++)
		{
			if (!closed.Contains(current->neighbors[i]->center)) { open.Add(current->neighbors[i]); }
		}

		/**
		* Re-calculate f_cost for all nodes as new g_cost should be the distance from the current to the particular node
		* This way we can find which node is nearest to both - the latest entry in path[] & the end node - this will give us the next entry in path
		* Concerns: We don't need to re-calculate f_cost for all nodes, just the ones in open[] However the CalculateHeuristics() fn parameters don't match the type of open[]
		* We can implement a new heuristics calculation function & this will improve performance as the open.Num < nodes.Num
		*/ 
		CalculateHeuristics_Euclidean(nodes, *current, endNode);

		// if current is the destination then calculate path using recursion, exit loop
		if (current == &endNode)
		{
			doesPathExist = true;
			// Traverse the linked list to populate path, start from the last entry & traverse backwards till you find the 1st entry
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

// Purpose: Reset node parent & set f_cost to 0
void UAStar::ResetHeuristics(TArray<FGraphNode>& nodes)
{
	for (int i = 0;i < nodes.Num();i++)
	{
		nodes[i].parent = nullptr;
		nodes[i].f_cost = 0.0f;
	}
}

// Purpose: Use Manhattan distance while calculating f_cost
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

// Use Euclidean distance while calculating f_cost (better results)
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