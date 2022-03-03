# ATimeBomb
It is the actor planted in [APlantingArea](./PlantingArea.md). The time on UI is going to set to `MaxTime` after it is planted. Counter-Terror team win if it is diffused. But if it isn't diffused and time is gone then Terror team win.

# Index
- [Tutorial](#_Tutorial)
- [How to guide](#_How_to_Guide)
- [Technical reference](#_Technical_reference)

# Tutorial
## How to create a new BP.
1. Create a new Blueprint inherits `AInteractiveActor` at `C++ class/FPS/Actors`. After then follow the tutorial of [InteractiveActor.md](./InteractiveActor.md) first.
2. Set `BombMeshComponent`

# How_to_Guide

# Technical_reference

## Variables
<details open>
<summary></summary>

||Name|Description|
|-|-|-|
|UStaticMeshComponent*|BombMeshComponent||
|float|MaxTime|Time limit for explosion|
|bool|IsDismantling|Status for check the `ATimeBomb` is on interaction to diffuse|
|float|MaxDiffusingTime|How much time for diffusing|
|float|CurrentDiffusingTime|How much time is spend for diffusing|

</details>