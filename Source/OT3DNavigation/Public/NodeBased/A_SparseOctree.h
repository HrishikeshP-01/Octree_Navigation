// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OctTreeNode.h"
#include "OctTreeNodeWC.h"
#include "Debug/DebugOctree.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GraphNode.h"
#include "A_SparseOctree.generated.h"

UCLASS()
class OT3DNAVIGATION_API AA_SparseOctree : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AA_SparseOctree();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Variables for octree:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Octree|Parameters")
		int32 m;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Octree|Parameters")
		int32 n;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Octree|Parameters")
		float nodeSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Octree|Parameters")
		int levelCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Octree|Parameters")
		float maxSide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Octree|Parameters")
		float minSide = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Octree|Graph")
		TArray<FOctTreeNode> nodes;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Octree|Graph")
		int nodeCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Octree|CollisionDetection")
		TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Octree|CollisionDetection")
		TArray<AActor*> ignoreActors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Octree|Debug")
		TArray<FColor> colors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Octree|Debug")
		bool DrawUnoccupiedOctNodes = true;

	// Functions for octree
	UFUNCTION(BlueprintCallable)
		FOctTreeNode& GenerateOctree(FOctTreeNode& parent, float currSide, FVector center, int level);
	UFUNCTION(BlueprintCallable)
		void VisualizeTree();
	UFUNCTION(BlueprintCallable)
		void SetVariables(int n_1, int m_1, float nodeSize_1, float maxSide_1, float minSide_1);
	UFUNCTION(BlueprintCallable)
		void VisualizeOccupancy();

	// Logic for graph
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph")
		TArray<FGraphNode> graphNodes;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph")
		int graphNodeCount = -1;
	UFUNCTION(BlueprintCallable)
		void GenerateGraph();
};