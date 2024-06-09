// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OctTreeNode.h"
#include "OctTreeNodeWC.generated.h"

/**
 * Description - Struct to represent nodes that have children (non-leaf nodes)
 *				 In this implementation every non-leaf node has 8 children
 * Purpose - Keeping the leaf & non-leaf nodes seperate lets us define the children array for non-leaf nodes only
 *			 This saves space considerably.
 * Tips - As structs are like classes they can be inherited
 *		  If you have a parameterized constructor, you need a default constructor else it'll throw an error
 * Future Improvements - Instead of assuming 8 children for nodes, use dynamic array for children
 *						 This would allow us to remove the children that are occupied/non-traversable from memory
 *						 But will this optimization be impactful?
 */
USTRUCT(BlueprintType)
struct FOctTreeNodeWC : public FOctTreeNode
{
	GENERATED_BODY()
	
	FOctTreeNode* children[8];

	FOctTreeNodeWC()
	{
		parent = nullptr;
		isLeaf = false;
		level = 0;
		center = FVector::ZeroVector;
	}
	FOctTreeNodeWC(FOctTreeNode* p, int8 l, FVector c)
	{
		parent = p;
		isLeaf = false;
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
	FOctTreeNode* FOctTreeNodeWC::GetChild(int i)
	{
		return children[i];
	}
	void SetChild(FOctTreeNode* c, int i)
	{
		children[i] = c;
	}
};