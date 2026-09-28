// Fill out your copyright notice in the Description page of Project Settings.


#include "PATCBaseEntity.h"

#include "PanicClinic_EIK/Components/PATCHealthComponent.h"


// Sets default values
APATCBaseEntity::APATCBaseEntity()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	HealthComponent = CreateDefaultSubobject<UPATCHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void APATCBaseEntity::BeginPlay()
{
	Super::BeginPlay();
	
}

void APATCBaseEntity::OnDeath_Implementation()
{
	//This is the base entity, this function will be overriden for more specific behavior.
	//The destruction (or whatever we want to do with a dead actor) should happen after being triggered by the animation.	
	
	MulticastTriggerDeathAnim();
	
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &APATCBaseEntity::KillEntity, 1.f, false);
	}
}

void APATCBaseEntity::OnDamageTaken_Implementation()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Server spreads damage FX"));
		MulticastTriggerHitFX();
	}
}

void APATCBaseEntity::OnHealthReceived_Implementation()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Server spreads healing FX"));
		MulticastTriggerHealFX();
	}
}



void APATCBaseEntity::MulticastTriggerDeathAnim_Implementation()
{
	//Plays the death animation locally.
	PlayDeathAnim();
}

void APATCBaseEntity::MulticastTriggerHitFX_Implementation()
{
	//Plays the hit FX locally.
	PlayHitFX();
}

void APATCBaseEntity::MulticastTriggerHealFX_Implementation()
{
	//Plays healing FX locally.
	PlayHealFX();
}

void APATCBaseEntity::KillEntity()
{
	if (HasAuthority())
	{
		Destroy();
	}
}

// Called every frame
void APATCBaseEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

