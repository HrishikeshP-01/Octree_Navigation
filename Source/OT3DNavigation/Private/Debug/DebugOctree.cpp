// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/DebugOctree.h"
#include "DrawDebugHelpers.h"

UDebugOctree::UDebugOctree()
{
}

UDebugOctree::~UDebugOctree()
{
}

void UDebugOctree::DrawOctNode(UWorld* world, FVector& center, float side, FColor color)
{
	DrawDebugBox(world, center, FVector(side, side, side)/2, color, true);
}

void UDebugOctree::DrawPoint(UWorld* world, FVector& center, float radius, FColor color)
{
	DrawDebugSphere(world, center, radius, 8, color, true);
}

void UDebugOctree::DrawLine(UWorld* world, FVector& start, FVector& end, FColor color)
{
	DrawDebugLine(world, start, end, color, true);
}