// Fill out your copyright notice in the Description page of Project Settings.
/**
* Description: Actor component that handles node-based octree creation & traversal
* Purpose: Generation, traversal & debugging of node-based octrees
* Concerns: 
*/

#include "NodeBased/AC_Octree.h"

// Sets default values for this component's properties
UAC_Octree::UAC_Octree()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UAC_Octree::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAC_Octree::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAC_Octree::SetVariables(int act_n, int act_m, TArray<FColor> c)
{
	// Set variables that need to be assigned everytime something's tweaked in editor
	n = act_n;
	m = act_m;
	levelCount = n - m;
	maxSize = FMath::Pow(8, n); // Min possible value of a cube is 
	minSize = FMath::Pow(8, m);
	colors = c;
}

/**
* Tips: This logic ensures the root node is nodes[0]
*		The nodes are created & stored in Depth First Approach
* Optimizations: As nodes are stored in DFS, & child nodes are stored in a particular order we only need to know the level & parent to calculate size & center
*/
FOctTreeNode& UAC_Octree::GenerateSimpleOctree(float currSize, FOctTreeNode& parent, FVector center, int level)
{
	nodeCount++;

	if (currSize <= minSize)
	{
		nodes.Add(FOctTreeNode(&parent, (int8)level, center));
		return nodes[nodeCount - 1];
	}

	nodes.Add(FOctTreeNodeWC(&parent, (int8)level, center));

	float newSize = currSize / 8;
	float side = FMath::Pow(2, (float)level);
	int newLevel = level - 1;
	/**
	* newLevel = level-- is Wrong. Why?
	* Because this expanded is newLevel = level = level -1
	* newLevel is assigned the previous value of level & level is then decremented. 
	* newLevel = --level gives the same result
	*/

	// Set Children
	nodes[nodeCount - 1].SetChild(&GenerateSimpleOctree(currSize / 8, nodes[nodeCount - 1], center + FVector(side / 2, side / 2, side / 2), newLevel), 0); // 000
	nodes[nodeCount - 1].SetChild(&GenerateSimpleOctree(currSize / 8, nodes[nodeCount - 1], center + FVector(side / 2, side / 2, -side / 2), newLevel), 0); // 001
	nodes[nodeCount - 1].SetChild(&GenerateSimpleOctree(currSize / 8, nodes[nodeCount - 1], center + FVector(side / 2, -side / 2, side / 2), newLevel), 0); // 010
	nodes[nodeCount - 1].SetChild(&GenerateSimpleOctree(currSize / 8, nodes[nodeCount - 1], center + FVector(side / 2, -side / 2, -side / 2), newLevel), 0); // 011
	nodes[nodeCount - 1].SetChild(&GenerateSimpleOctree(currSize / 8, nodes[nodeCount - 1], center + FVector(-side / 2, side / 2, side / 2), newLevel), 0); // 100
	nodes[nodeCount - 1].SetChild(&GenerateSimpleOctree(currSize / 8, nodes[nodeCount - 1], center + FVector(-side / 2, side / 2, -side / 2), newLevel), 0); // 101
	nodes[nodeCount - 1].SetChild(&GenerateSimpleOctree(currSize / 8, nodes[nodeCount - 1], center + FVector(-side / 2, -side / 2, side / 2), newLevel), 0); // 110
	nodes[nodeCount - 1].SetChild(&GenerateSimpleOctree(currSize / 8, nodes[nodeCount - 1], center + FVector(-side / 2, -side / 2, -side / 2), newLevel), 0); // 111

	return nodes[nodeCount - 1];
}

void UAC_Octree::VisualizeTree()
{
	UWorld* world = GetWorld();
	for (int i = 0;i < nodes.Num();i++)
	{
		float size = FMath::Pow(8, (double)nodes[i].level);
		float side = FMath::Pow(2, (double)nodes[i].level);
		UE_LOG(LogTemp, Warning, TEXT("Center: %s"), *nodes[i].center.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Size: %f"), size);
		UE_LOG(LogTemp, Warning, TEXT("Side: %f"), side);
		UE_LOG(LogTemp, Warning, TEXT("Level Count: %d"), (int)nodes[i].level);
		UDebugOctree::DrawOctNode(world, nodes[i].center, side, colors[(int)nodes[i].level]);
	}
}