// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "FPSGameState.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();

	GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bSuccess)
{

	if (!InstigatorPawn)
		return ;

	InstigatorPawn->DisableInput(nullptr);

	OnMissionCompleted(InstigatorPawn, bSuccess);

	if (!SpectatingCameraActor)
		return ;

	// find spectator actor and set a view target to it
	AActor* newViewTarget = nullptr;
	TArray<AActor*>	ActorsFound;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpectatingCameraActor, ActorsFound);
	if (ActorsFound.Num()) {
		newViewTarget = ActorsFound[0];

		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++) {
			APlayerController* Pc = Cast<APlayerController>(It->Get());
			if (Pc) {
				Pc->SetViewTargetWithBlend(newViewTarget, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
			}
		}
	}

	AFPSGameState* Gs = GetGameState<AFPSGameState>();
	if (Gs) {
		Gs->MulticastOnMissionComplete(InstigatorPawn, bSuccess);
	}
}
