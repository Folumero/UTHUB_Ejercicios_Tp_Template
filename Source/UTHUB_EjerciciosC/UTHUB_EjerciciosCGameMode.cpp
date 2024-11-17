// Copyright Epic Games, Inc. All Rights Reserved.

#include "UTHUB_EjerciciosCGameMode.h"
#include "UTHUB_EjerciciosCCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUTHUB_EjerciciosCGameMode::AUTHUB_EjerciciosCGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
