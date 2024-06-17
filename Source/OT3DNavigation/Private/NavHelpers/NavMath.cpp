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