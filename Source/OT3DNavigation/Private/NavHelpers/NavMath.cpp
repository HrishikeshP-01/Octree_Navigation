// Fill out your copyright notice in the Description page of Project Settings.


#include "NavHelpers/NavMath.h"

UNavMath::UNavMath()
{
}

UNavMath::~UNavMath()
{
}

float UNavMath::GetManhattanDistance(FVector& A, FVector& B)
{
	return (FMath::Abs(A.X - B.X) + FMath::Abs(A.Y - B.Y) + FMath::Abs(A.Z - B.Z));
}

void UNavMath::GetVertices(FVector& center, float side, TArray<FVector>& vert)
{
	vert.Add(center + FVector(side, side, side) / 2); //000
	vert.Add(center + FVector(side, side, -side) / 2); //001
	vert.Add(center + FVector(side, -side, side) / 2); //010
	vert.Add(center + FVector(side, -side, -side) / 2); //011
	vert.Add(center + FVector(-side, side, side) / 2); //100
	vert.Add(center + FVector(-side, side, -side) / 2); //101
	vert.Add(center + FVector(-side, -side, side) / 2); //110
	vert.Add(center + FVector(-side, -side, -side) / 2); //111
}