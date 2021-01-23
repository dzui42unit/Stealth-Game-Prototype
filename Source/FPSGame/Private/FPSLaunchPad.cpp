// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"
#include "FPSCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pad Mesh"));
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Comp"));
	RootComponent = PadMesh;
	OverlapComp->SetupAttachment(PadMesh);
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	OverlapComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void AFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OnPadZoneBeginOverlap);
}

void AFPSLaunchPad::OnPadZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FRotator ActorRotation = GetActorRotation();
	ActorRotation.Pitch += LaunchAngle;
	FVector LaunchForceVector = ActorRotation.Vector() * LaunchForce;

	if (OtherActor) {
		AFPSCharacter* MyChar = Cast<AFPSCharacter>(OtherActor);
		if (MyChar) {
			MyChar->LaunchCharacter(LaunchForceVector, true, true);
			UGameplayStatics::SpawnEmitterAtLocation(this, LaunchParticels, GetActorLocation());
		}
	}

	if (OtherComp && OtherComp->IsSimulatingPhysics()) {
		OtherComp->AddImpulse(LaunchForceVector, NAME_None, true);
		UGameplayStatics::SpawnEmitterAtLocation(this, LaunchParticels, GetActorLocation());
	}
}
