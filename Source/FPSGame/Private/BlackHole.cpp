// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create spheres components;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	ActionRadiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Action radius sphere"));
	DeathRadiusShere = CreateDefaultSubobject<USphereComponent>(TEXT("Death radis sphere"));
	RootComponent = Mesh;
	ActionRadiusSphere->SetupAttachment(Mesh);
	DeathRadiusShere->SetupAttachment(Mesh);

	// set action radius sphere create collision only for overlap event
	ActionRadiusSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ActionRadiusSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ActionRadiusSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();

	DeathRadiusShere->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OnBlackHoleBeginOverLap);
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray< UPrimitiveComponent* > OveralappedObjects;
	float BlackHoleRadius = ActionRadiusSphere->GetScaledSphereRadius();
	ActionRadiusSphere->GetOverlappingComponents(OveralappedObjects);
	for (UPrimitiveComponent* elem : OveralappedObjects) {
		if (elem->IsSimulatingPhysics()) {
			elem->AddRadialForce(GetActorLocation(), BlackHoleRadius, BlackHoleForce, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
}

void ABlackHole::OnBlackHoleBeginOverLap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		OtherActor->Destroy();
	}
}
