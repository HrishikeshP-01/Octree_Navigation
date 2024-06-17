// Fill out your copyright notice in the Description page of Project Settings.


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

void AA_SparseOctree::SetVariables(int n_1, int m_1, float nodeSize_1, float maxSide_1, float minSide_1)
{
	n = n_1;
	m = m_1;
	nodeSize = nodeSize_1;

	levelCount = n - m;
	maxSide = maxSide_1;
	minSide = (minSide_1 >= 1) ? minSide_1 : 1.0f;
	nodeCount = 0;
	nodes.Empty();

	ignoreActors.Init(this, 1);
	//colors.Init(FColor::Red, 1);
}

FOctTreeNode& AA_SparseOctree::GenerateOctree(FOctTreeNode& parent, float currSide, FVector center, int level)
{
	nodeCount++;

	TArray<AActor*> outActors;
	bool isOccupied = UKismetSystemLibrary::BoxOverlapActors(GetWorld(), center, FVector(currSide), traceObjectTypes, AActor::StaticClass(), ignoreActors, outActors);
	DrawDebugBox(GetWorld(), center, FVector(currSide), FColor::Red, true);

	// If min side reached or the node is un-occupied, node is leaf node
	if (currSide <= minSide)
	{
		nodes.Add(FOctTreeNode(&parent, (int8)level, center));
		if (isOccupied) { nodes[nodeCount - 1].isOccupied = true; }
		return nodes[nodeCount - 1];
	}
	
	if (!isOccupied)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Occupied"));
		nodes.Add(FOctTreeNode(&parent, (int8)level, center));
		return nodes[nodeCount - 1];
	}
	
	// Else sub-divide the cell & try again
	nodes.Add(FOctTreeNodeWC(&parent, (int8)level, center));

	float newSide = currSide / 2;
	int newLevel = level + 1;

	// Set Children
	nodes[nodeCount - 1].SetChild(&GenerateOctree(nodes[nodeCount-1],newSide,center+ FVector(currSide / 2, currSide / 2, currSide / 2), newLevel),0); // 000
	nodes[nodeCount - 1].SetChild(&GenerateOctree(nodes[nodeCount - 1], newSide, center + FVector(currSide / 2, currSide / 2, -currSide / 2), newLevel), 1); // 001
	nodes[nodeCount - 1].SetChild(&GenerateOctree(nodes[nodeCount - 1], newSide, center + FVector(currSide / 2, -currSide / 2, currSide / 2), newLevel), 2); // 010
	nodes[nodeCount - 1].SetChild(&GenerateOctree(nodes[nodeCount - 1], newSide, center + FVector(currSide / 2, -currSide / 2, -currSide / 2), newLevel), 3); // 011
	nodes[nodeCount - 1].SetChild(&GenerateOctree(nodes[nodeCount - 1], newSide, center + FVector(-currSide / 2, currSide / 2, currSide / 2), newLevel), 4); // 100
	nodes[nodeCount - 1].SetChild(&GenerateOctree(nodes[nodeCount - 1], newSide, center + FVector(-currSide / 2, currSide / 2, -currSide / 2), newLevel), 5); // 101
	nodes[nodeCount - 1].SetChild(&GenerateOctree(nodes[nodeCount - 1], newSide, center + FVector(-currSide / 2, -currSide / 2, currSide / 2), newLevel), 6); // 110
	nodes[nodeCount - 1].SetChild(&GenerateOctree(nodes[nodeCount - 1], newSide, center + FVector(-currSide / 2, -currSide / 2, -currSide / 2), newLevel), 7); // 111

	return nodes[nodeCount - 1];
}

void AA_SparseOctree::VisualizeTree()
{
	UWorld* world = GetWorld();
	for (int i = 0;i < nodeCount;i++)
	{
		int colorNum = (nodes[i].level < colors.Num() - 1) ? nodes[i].level : 0;
		float side = maxSide / FMath::Pow(2, (double)nodes[i].level);
		UE_LOG(LogTemp, Warning, TEXT("Center: %s"), *nodes[i].center.ToString());
		UDebugOctree::DrawOctNode(world, nodes[i].center, side, colors[colorNum]);
	}
}

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

void AA_SparseOctree::GenerateGraph()
{
	for (int i = 0;i < nodeCount;i++)
	{
		if (!nodes[i].isLeaf || nodes[i].isOccupied) { continue; }
		graphNodeCount++;
		graphNodes.Add(FGraphNode(nodes[i].center));
		//if(int j=0;j<nodeCount;j++)
	}
}