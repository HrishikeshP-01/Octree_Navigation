// Fill out your copyright notice in the Description page of Project Settings.
/**
* Description: Actor component that handles node-based octree creation & traversal
* Purpose: Generation, traversal & debugging of node-based octrees
* Concerns: Do the helper fns CreateLeafNode & CreateNodeWC need to return a value??
*/
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OctTreeNode.h"
#include "OctTreeNodeWC.h"
#include "Debug/DebugOctree.h"
#include "AC_Octree.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OT3DNAVIGATION_API UAC_Octree : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_Octree();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// Variables
protected:
	/**
	* Max size = 2^(3+n), Min size = 2^(3+m), n>m
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Octree|Parameters")
		int32 n;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Octree|Parameters")
		int32 m;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Octree|Parameters")
		int levelCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Octree|Parameters")
		float maxSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Octree|Parameters")
		float minSize;
public: // Remove later
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Octree|Parameters")
	TArray<FOctTreeNode> nodes;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Octree|Parameters")
	int nodeCount = 0;

// Functions
	UFUNCTION(BlueprintCallable)
		FOctTreeNode& GenerateSimpleOctree(float currSize, FOctTreeNode& parent, FVector center, int level);

// Debug Functionality
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Octree|Debug")
		TArray<FColor> colors;
	UFUNCTION(BlueprintCallable)
		void VisualizeTree();
	UFUNCTION(BlueprintCallable)
		void SetVariables(int act_n, int act_m, TArray<FColor> c);

};
