// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTDOMINO_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RegenStamina(float DeltaTime);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	/* Current Health */
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health;

	/* Max Health */
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth;

	/* Current Stamina */
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Stamina;

	/* Max Stamina */
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float DodgeCost;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float StaminaRegenRate;

public:	

	void RecieveDamage(float Damage);
	void UseStamina(float StaminaCost);
	void Heal(float HealAmount);
	float GetHealthPercent();
	float GetStaminaPercent();
	bool IsAlive();

	/** Getters */
	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }
	FORCEINLINE float GetStamina() const { return Stamina; }

};
