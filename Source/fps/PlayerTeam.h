#pragma once
#include "PlayerTeam.generated.h"

UENUM()
enum class EPlayerTeam : uint8
{
	TeamTerror UMETA(DisplayName = "Team Terror"),
	TeamCounterTerror UMETA(DisplayName = "Team Counter-Terror"),
	None UMETA(DisplayName = "None"),
};