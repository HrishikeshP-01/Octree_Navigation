// Fill out your copyright notice in the Description page of Project Settings.
/**
 * Description: A class for general debug functionality while working with octrees
 * Purpose: Debug functions for octrees
 * Concerns: Learn how to implement libraries in UE
 */

#include "Debug/DebugOctree.h"
#include "DrawDebugHelpers.h"

UDebugOctree::UDebugOctree()
{
}

UDebugOctree::~UDebugOctree()
{
}

// Purpose: Visualizing Octree partitioning in space
void UDebugOctree::DrawOctNode(UWorld* world, FVector& center, float side, FColor color)
{
	DrawDebugBox(world, center, FVector(side, side, side)/2, color, true);
}

// Purpose: Visualizing centers of octnodes
void UDebugOctree::DrawPoint(UWorld* world, FVector& center, float radius, FColor color)
{
	DrawDebugSphere(world, center, radius, 8, color, true);
}

// Purpose: Visualizing path
void UDebugOctree::DrawLine(UWorld* world, FVector& start, FVector& end, FColor color)
{
	DrawDebugLine(world, start, end, color, true);
}