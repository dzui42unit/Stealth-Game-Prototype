// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSObjectiveActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"

// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create a mesh component and disable its collision
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Objective Mesh component"));
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision component"));
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// create a mesh component and set it as root component
	CollisionSphere->SetupAttachment(MeshComponent);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = MeshComponent;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();
}

void AFPSObjectiveActor::SpawnOverlapParticles() const
{
	if (OverlapParticles) {
		UGameplayStatics::SpawnEmitterAtLocation(this, OverlapParticles, GetActorLocation());
	}
}

// Called every frame
void AFPSObjectiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// run this operation only on the server side 
	if (HasAuthority()) {

		AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor);
		
		if (MyCharacter) {
			MyCharacter->bIsObjectTaken = true;
			Destroy();
		}
	}

	// spawn particles both on client and server side
	SpawnOverlapParticles();
}

