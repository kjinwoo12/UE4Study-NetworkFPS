# PlantBombMode
PlantBombMode is a GameModeBase.

# Index
- [Tutorial](#_Tutorial)
- [How to guide](#_How_to_guide)
- [Technical reference](#_Technical_reference)

# Tutorial
It needs to set `Classes` after create as BP.
```
GameState Class : BP_PlantBombState
Player Controller Class : BP_FpsPlayerController
Player State Class : BP_FpsPlaeyrState
HUD Class : BP_FpsCharacterHud
```

# How to guide

# Technical reference
## Variables
<details open>
<summary></summary>
</details>

||Name|Description|
|-|-|-|
|FTimerHandle|RoundEventTimer|
|const float|TransitionTimeForOnRoundReady|Default 3.f|
|const float|TransitionTimeForOnRoundStart|Default 15.f|
|const float|TimeForRoundRunTime|Default 160.f|
|TSubclassOf<`AGameModeBase`>|PreparedGameModeSubclass|

## Constructors
<details open>
<summary></summary>

|prarams|Description|
|-|-|
|none|none|

</details>

## Functions
<details open>
<summary></summary>

||Name|Description|
|-|-|-|
|virtual void|BeginPlay() override||
|virtual void|PostLogin(APlayerController* NewPlayer)||
|void|OnPlayerJoinTeam()||
|void|OnPlayerFull()||
|void|OnRoundReady()||
|void|OnRoundStart()||
|void|OnRoundEnd()||
|bool|IsPlayerFullOnTeam()||

</details>