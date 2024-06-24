// Fill out your copyright notice in the Description page of Project Settings.
/**
* Description: Class for implementation of sparse octree
* Purpose: Implementation of sparse-octree & finding the best path between 2 points
* Improvements: Optimization. We can use sets or dicts instead of arrays & reduce time complexity
* You can delete octree nodes after creating the graph. I'm keeping it for debugging & visualization
*/

#include "NodeBased/A_SparseOctree.h"
#include "DrawDebugHelpers.h"

// Sets default values
AA_SparseOctree::AA_SparseOctree()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AA_SparseOctree::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AA_SparseOctree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
* Purpose: Every time the octree is created we need to reset node counts & empty the node arrays 
* Else data just keeps getting added as we use arr.Add() functions later
*/
void AA_SparseOctree::SetVariables(float maxSide_1, float minSide_1)
{
	maxSide = maxSide_1;
	minSide = (minSide_1 >= 1) ? minSide_1 : 1.0f;
	
	nodeCount = 0;
	nodes.Empty();
	graphNodeCount = 0;
	graphNodes.Empty();

	ignoreActors.Init(this, 1);
	//colors.Init(FColor::Red, 1);

	aStarPath.Empty();
}

/**
* Purpose: Function to generate a sparse octree
* Logic: 
* 1. Increment nodeCount
* 2. Check if the particular region is occupied 
* 3. If yes then that is a leaf node, create a leaf node & enter it into the array -> Exit
* 4. If the currSide <= minSide we can't subdivide any further so this is a leaf node, create a leaf node & enter it into the array -> Exit
* 5. If it's not a leaf node, create a non-leaf node & enter it into the array
* 6. Call the same function for positions equal to the center of a cube subdivided into 8 smaller cubes
* 7. Exit
* Improvements: Optimization - use dictionary to store center(key) & necessary info(value) this will make retrival very time-efficient 
*/
FOctTreeNode& AA_SparseOctree::GenerateOctree(FOctTreeNode& parent, float currSide, FVector center, int level)
{
	nodeCount++;

	TArray<AActor*> outActors;
	bool isOccupied = UKismetSystemLibrary::BoxOverlapActors(GetWorld(), center, FVector(currSide/2), traceObjectTypes, AActor::StaticClass(), ignoreActors, outActors);
	// If min side reached or the node is un-occupied, node is leaf node
	if (currSide <= minSide)
	{
		nodes.Add(FOctTreeNode(&parent, (int8)level, center));
		nodes[nodeCount - 1].isOccupied = isOccupied;
		return nodes[nodeCount - 1];
	}
	
	// If it's not occupied, no need to subdivide, this is a leaf node
	if (!isOccupied)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unoccupied: %s"), *center.ToString());
		nodes.Add(FOctTreeNode(&parent, (int8)level, center));
		return nodes[nodeCount - 1];
	}

	UE_LOG(LogTemp, Warning, TEXT("Occupied: %s"), *center.ToString());
	// Else sub-divide the cell & try again
	nodes.Add(FOctTreeNodeWC(&parent, (int8)level, center));

	float newSide = currSide / 2;
	int newLevel = level + 1;

	// Call the same function to create the children & set them as the neighbors of the current node
	nodes[nodeCount - 1].SetChild(&GenerateOctree(nodes[nodeCount-1],newSide,center+ FVector(newSide, newSide, newSide)/2, newLevel),0); // 000
	nodes[nodeCount - 1].SetChild(&GenerateOctree(nodes[nodeCount - 1], newSide, center + FVector(newSide, newSide, -newSide)/2, newLevel), 1); // 001
	nodes[nodeCount - 1].SetChild(&GenerateOctree(nodes[nodeCount - 1], newSide, center + FVector(newSide, -newSide, newSide)/2, newLevel), 2); // 010
	nodes[nodeCount - 1].SetChild(&GenerateOctree(nodes[nodeCount - 1], newSide, center + FVector(newSide, -newSide, -newSide)/2, newLevel), 3); // 011
	nodes[nodeCount - 1].SetChild(&GenerateOctree(nodes[nodeCount - 1], newSide, center + FVector(-newSide, newSide, newSide)/2, newLevel), 4); // 100
	nodes[nodeCount - 1].SetChild(&GenerateOctree(nodes[nodeCount - 1], newSide, center + FVector(-newSide, newSide, -newSide)/2, newLevel), 5); // 101
	nodes[nodeCount - 1].SetChild(&GenerateOctree(nodes[nodeCount - 1], newSide, center + FVector(-newSide, -newSide, newSide)/2, newLevel), 6); // 110
	nodes[nodeCount - 1].SetChild(&GenerateOctree(nodes[nodeCount - 1], newSide, center + FVector(-newSide, -newSide, -newSide)/2, newLevel), 7); // 111

	return nodes[nodeCount - 1];
}

// Purpose: Debug function to visualize the sparse octree
void AA_SparseOctree::VisualizeTree()
{
	UWorld* world = GetWorld();
	for (int i = 0;i < nodeCount;i++)
	{
		int colorNum = (nodes[i].level < colors.Num() - 1) ? nodes[i].level : 0;
		float side = maxSide / FMath::Pow(2, (double)nodes[i].level);
		UDebugOctree::DrawOctNode(world, nodes[i].center, side, colors[colorNum]);
	}
}

// Purpose: Debug function to see which nodes are occupied or which nodes are unoccupied depending on DrawUnoccupiedOctNodes variable
void AA_SparseOctree::VisualizeOccupancy()
{
	UWorld* world = GetWorld();
	for (int i = 0;i < nodeCount;i++)
	{
		float side = maxSide / FMath::Pow(2, (double)nodes[i].level);
		if (nodes[i].isOccupied && nodes[i].isLeaf)
		{
			UDebugOctree::DrawOctNode(world, nodes[i].center, side, FColor::Red);
		}
		else if (DrawUnoccupiedOctNodes)
		{
			UDebugOctree::DrawOctNode(world, nodes[i].center, side, FColor::Black);
		}
	}
}

/**
* Purpose: Generate graph which stores spatial data
* We need a way to represent the unoccupied spatial neighbors of a node
*/
void AA_SparseOctree::GenerateGraph()
{
	CreateGraphNodes();
	CreateGraphEdges();
}

/**
* Pupose: graph[] just needs to contain the nodes of the octree that are both - leaf nodes & unoccupied
*/
void AA_SparseOctree::CreateGraphNodes()
{
	for (int i = 0;i < nodeCount;i++)
	{
		if (!nodes[i].isLeaf || nodes[i].isOccupied) { continue; }
		graphNodeCount++;
		graphNodes.Add(FGraphNode(nodes[i].center, nodes[i].level));
	}
}

// Purpose: Find & assign the neigbors of a node
void AA_SparseOctree::CreateGraphEdges()
{
	float distanceThreshold, side_i, side_j, diff_X, diff_Y, diff_Z;
	for (int i = 0;i < graphNodeCount;i++)
	{
		FGraphNode& current = graphNodes[i];
		side_i = maxSide / FMath::Pow(2, (double)current.level);
		for (int j = 0;j < graphNodeCount;j++)
		{
			if (i == j) { continue; }
			side_j = maxSide / FMath::Pow(2, (double)graphNodes[j].level);
			distanceThreshold = (side_i > side_j) ? side_i / 2 : side_j / 2;
			diff_X = FMath::Abs(current.center.X - graphNodes[j].center.X);
			diff_Y = FMath::Abs(current.center.Y - graphNodes[j].center.Y);
			diff_Z = FMath::Abs(current.center.Z - graphNodes[j].center.Z);
			if ((diff_X == (side_i + side_j) / 2 && diff_Y < distanceThreshold && diff_Z < distanceThreshold) ||
				(diff_Y == (side_i + side_j) / 2 && diff_X < distanceThreshold && diff_Z < distanceThreshold) ||
				(diff_Z == (side_i + side_j) / 2 && diff_X < distanceThreshold && diff_Y < distanceThreshold))
			{
				current.neighbors.Add(&graphNodes[j]);
			}
		}
	}
}

// Purpose: Debug function to visualize the graph & center of the nodes
void AA_SparseOctree::VisualizeGraph()
{
	UWorld* world = GetWorld();
	for (int i = 0;i < graphNodeCount;i++)
	{
		if (VisualizeGraphAsBoxes)
		{
			float side = maxSide / FMath::Pow(2, (double)graphNodes[i].level);
			UDebugOctree::DrawOctNode(world, graphNodes[i].center, side, FColor::Green);
		}
		//else
		{
			// Costly
			UDebugOctree::DrawPoint(world, graphNodes[i].center, 10.0f, FColor::Green);
		}
	}
}

// Purpose: Debug function to visualize the neighbors of a particular node
void AA_SparseOctree::VisualizeNeighborNodes(UPARAM(ref) FGraphNode& node)
{
	UWorld* world = GetWorld();
	float side = maxSide / FMath::Pow(2, (double)node.level);
	UE_LOG(LogTemp, Warning, TEXT("Node Center: %s"), *node.center.ToString());
	UDebugOctree::DrawOctNode(world, node.center, side, FColor::Red);
	for(int i=0;i<node.neighbors.Num();i++)
	{
		side = maxSide / FMath::Pow(2, (double)node.neighbors[i]->level);
		UE_LOG(LogTemp, Warning, TEXT("Neighbor Center: %s"), *node.neighbors[i]->center.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Manhattan: %f"), UNavMath::GetManhattanDistance(node.center, node.neighbors[i]->center));
		UE_LOG(LogTemp, Warning, TEXT("side: %f"), side);
		UDebugOctree::DrawOctNode(world, node.neighbors[i]->center, side, FColor::Blue);
	}
}

/**
* Purpose: Debug function to visualize the vertices of a node
* This was created when I was considering to use the vertices area method to determine if a point was on the face of the cube which would help in finding the node neighbors
* The final implementation of node neighbors is not depedant on vertices so this function is not useful anymore
*/ 
void AA_SparseOctree::VisualizeVertices()
{
	UWorld* world = GetWorld();
	TArray<FVector> verts;
	for (int i = 0;i < graphNodeCount;i++)
	{
		float side = maxSide / FMath::Pow(2, (double)graphNodes[i].level);
		UNavMath::GetVertices(graphNodes[i].center, side, verts);
		for (int j = 0;j < 8;j++)
		{
			UDebugOctree::DrawPoint(world, verts[j], 10.f, FColor::Cyan);
		}
		verts.Empty();
	}
}

// Purpose: Get the shortest path between 2 nodes
void AA_SparseOctree::FindPathBetweenNodes(UPARAM(ref) FGraphNode& startNode, UPARAM(ref) FGraphNode& endNode)
{
	aStarPath = UAStar::GetPath(graphNodes, startNode, endNode, doesPathExist);
}

// Purpose: Debug function to visualize the shortest path between 2 nodes
void AA_SparseOctree::VisualizePath()
{
	UWorld* world = GetWorld();
	for (int i = 0;i < aStarPath.Num() - 1;i++)
	{
		UDebugOctree::DrawLine(world, aStarPath[i], aStarPath[i + 1], FColor::Black);
	}
}