// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "Perception/PawnSensingComponent.h"
#include "Engine/TargetPoint.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnSeePawn);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnHearNoise);
	DefaultOrientation = GetActorRotation();

	GuardState = EAIState::Idle;

	AICharController = Cast<AAIController>(GetController());

	TargetToMove = TargetPoint1;
}

void AFPSAIGuard::MoveGuard()
{
	if (!bPatroll)
		return;

	if (!AICharController)
		return;

	if (!TargetPoint1 || !TargetPoint2)
		return;

	if (GuardState == EAIState::Suspicious) {
		AICharController->StopMovement();
		return;
	}


	if (AICharController->GetMoveStatus() == EPathFollowingStatus::Idle) {
		if (TargetToMove == TargetPoint1) {
			TargetToMove = TargetPoint2;
		} else {
			TargetToMove = TargetPoint1;
		}
		AICharController->MoveToLocation(TargetToMove->GetActorLocation(), 10.f, false);
	}
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority()) {
		if (GuardState == EAIState::Alerted) {
			AICharController->StopMovement();
			return;
		}
		else {
			MoveGuard();
		}
	}
}


void	AFPSAIGuard::OnSeePawn(APawn* Pawn)
{
	if (Pawn == nullptr)
		return;

	AFPSGameMode* Gm = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	Gm->CompleteMission(Pawn, false);

	DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 32.f, 12, FColor::Red, false, 10.f);

	SetGuardState(EAIState::Alerted);
}

void AFPSAIGuard::OnHearNoise(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted)
		return;

	DrawDebugSphere(GetWorld(), Location, 32.f, 12, FColor::Green, false, 10.f);

	FVector NoiseDirectionPoint = Location - GetActorLocation();
	NoiseDirectionPoint.Normalize();

	FRotator NewRotation = FRotationMatrix::MakeFromX(NoiseDirectionPoint).Rotator();

	NewRotation.Roll = 0;
	NewRotation.Pitch = 0;

	SetActorRotation(NewRotation);

	GetWorldTimerManager().ClearTimer(TimerResetOrientationHandler);
	GetWorldTimerManager().SetTimer(TimerResetOrientationHandler, this, &AFPSAIGuard::ResetOrientation, 2.f, false);

	SetGuardState(EAIState::Suspicious);
}

void AFPSAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted)
		return;

	SetActorRotation(DefaultOrientation);

	SetGuardState(EAIState::Idle);
}

void AFPSAIGuard::OnRep_GuardState()
{
	OnGuardStateChange(GuardState);
}

void AFPSAIGuard::SetGuardState(EAIState newState)
{
	if (GuardState == newState)
		return;
	GuardState = newState;
	OnRep_GuardState();
}

void AFPSAIGuard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuard, GuardState);
}
