# AHandedTimeBomb
Inherits from [AHands](./Hands.md). It is for doing action to planting bomb clicking `mouse 1` when [AFpsCharacter](./FPSCharacter.md) hold the `AHandedTimeBomb`.

# Index
- [Tutorial](#_Tutorial)
- [How to guide](#_How_to_Guide)
- [Technical reference](#_Technical_reference)

# Tutorial
## How to create a new BP.
1. Create a new Blueprint inherits `AHandedTimeBomb` at `C++ class/FPS/Weapon`. After then follow the tutorial of [Hands.md](./Hands.md) first.
2. Set default variables.
    - Set `PlantingAnimation`. The action of AHandedTimeBomb is going to play with the `PlantingAnimation` when you press the button `Mouse 1`.
    - Set `TimeBombSubclass`. It will be spawned after finished `PlantingAnimation`.

# How_to_Guide

# Technical_reference

## Variables
<details open>
<summary></summary>

||Name|Description|
|-|-|-|
|UAnimMontage*|PlantingAnimation|Running with starting action|
|TSubclassOf\<[ATimeBomb](./TimeBomb.md)\>|TimeBombSubclass|going to be spawned after finished action|
|FTimerHandle|PlantingTimer||

</details>