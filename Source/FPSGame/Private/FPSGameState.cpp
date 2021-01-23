// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"
#include "FPSPlayerController.h"
#include "Engine/World.h"

void	AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bSuccess)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++) {
		AFPSPlayerController* Pc = Cast< AFPSPlayerController>(It->Get());
		if (Pc && Pc->IsLocalController()) {
			APawn* Pawn = Pc->GetPawn();
			if (Pawn) {
				Pawn->DisableInput(nullptr);
			}
			Pc->OnMissionCompleted(InstigatorPawn, bSuccess);
		}
	}
}