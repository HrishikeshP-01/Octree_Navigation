// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OctTreeNode.generated.h"

/**
 * Description: Struct to store a leaf node with no children
 * Purpose: This will be the parent for node with children
 * Tips: Struct name begins with F in UE
 *		 If you have a parameterized constructor, you need a default constructor else it'll throw an error
 */
USTRUCT(BlueprintType)
struct FOctTreeNode
{
	GENERATED_BODY()

	FOctTreeNode* parent;
	bool isLeaf;
	int8 level; // Instead of storing size int we can store the much smaller level data type & calculate size from that later
	FVector center;

	FOctTreeNode()
	{
		parent = nullptr;
		isLeaf = true;
		level = 0;
		center = FVector::ZeroVector;
	}
	FOctTreeNode(FOctTreeNode* p, int8 l, FVector c)
	{
		parent = p;
		isLeaf = true;
		level = l;
		center = c;
	}

	/**
	* Initial approach - children[] was only present in child class - OctTreeNodeWC
	* Demerit - We can't access children[] when we use pointers to parent class
	* 2nd approach - initialized children[] in parent class: FOctTreeNode* children[]
	* Error - We can't have empty arrays in base classes, 
	* assigning a value to the array defeats the whole purpose of having seperate leaf & non-leaf node implementations
	* Solution - Create a virtual function in parent class to access children[]
	* Declare children[] & the function definition for GetChild() in child class
	*/
	virtual FOctTreeNode* GetChild(int i) { return nullptr; }
	virtual void SetChild(FOctTreeNode* c, int i) { return; }
};