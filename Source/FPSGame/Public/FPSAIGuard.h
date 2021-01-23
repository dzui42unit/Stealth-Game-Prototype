// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h" 

class AAIController;
class ATargetPoint;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	Suspicious = 1 UMETA(DisplayName = "Suspicious"),
	Alerted = 2  UMETA(DisplayName = "Alerted")
};

class UPawnSensingComponent;

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void	OnSeePawn(APawn* Pawn);

	UFUNCTION()
	void	OnHearNoise( APawn* NoiseInstigator, const FVector& Location, float Volume);

	FRotator	DefaultOrientation;
	FTimerHandle TimerResetOrientationHandler;

	UFUNCTION()
	void		ResetOrientation();

	UPROPERTY(ReplicatedUsing=OnRep_GuardState)
	EAIState	GuardState;
	// this functnion will be called on client each time when the GuardState variable is changed
	UFUNCTION()
	void		OnRep_GuardState();

	void		SetGuardState(EAIState newState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void		OnGuardStateChange(EAIState newState);

	AAIController* AICharController;

	UPROPERTY(EditInstanceOnly, Category = "AI")
	bool						bPatroll;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatroll"))
	ATargetPoint*			TargetPoint1;
	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatroll"))
	ATargetPoint*			TargetPoint2;

	ATargetPoint*			TargetToMove;

	FTimerHandle	MovementTimerHandler;

	void			MoveGuard();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
