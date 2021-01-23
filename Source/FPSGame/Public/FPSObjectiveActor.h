// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSObjectiveActor.generated.h"

class UStaticMeshCompoent;
class USphereComponent;
class UParticleSystem;

UCLASS()
class FPSGAME_API AFPSObjectiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSObjectiveActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// mesh and sphere component
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent*		MeshComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USphereComponent*			CollisionSphere;

	// particles for overlap event
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem* OverlapParticles;
	// spawn overlap particles
	void SpawnOverlapParticles() const;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Event when this actor overlaps another actor, for example a player walking into a trigger.
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
