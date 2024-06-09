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
	DrawDebugBox(world, center, FVector(side, side, side), color, true);
}