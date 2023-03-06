// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//include the FItem class here
#include "Item.h"
#include "FItems/FItem.h"
#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Pickupable.generated.h"

/**
 *
 */
UCLASS()
class PROJECTDOMINO_API APickupable : public AItem
{
	GENERATED_BODY()


public:

	APickupable();

	//getter for item
	FORCEINLINE UFItem* GetFItem() const { return storedFItem; }

	//Function that defines what happen when picked up, can be defined in Item
	UFUNCTION(BlueprintNativeEvent)
		void OnPickUp();

protected:
	virtual void OnPickUp_Implementation();

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
		FName ItemID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
		UFItem* storedFItem;
};
