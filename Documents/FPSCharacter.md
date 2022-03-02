# AFpsCharacter
It is general playable character class.

# Index
- [Tutorial](#_Tutorial)
- [How to guide](#_How_to_Guide)
- [Technical reference](#_Technical_reference)

# Tutorial
## Step 1. Create a new FpsCharacter BP.
1. Create a new Blueprint inherits FpsCharacter at `C++ Class/FPS`.
2. Set Skeletal Meshs
    - HandsMeshComponent : Show at first-person camera.
    - Mesh : Show this to other players.
3. Set Height and Radius of CapsuleComponent

## Step 2. Set a new FpsCharacter in your project
1. Create a new Blueprint inherits `AFpsCharacter`.
2. Open the Project Settings ( Edit -> Project settings )
3. Enter Project -> Map & Mode
4. Change Default Pawn Class to Blueprint AFpsCharacter.

## Step 3. Bind keys
1. Open the Project Settings.
2. Enter Engine -> Input
3. Bind actions like below
- Action Mapping
    - Action
        - Mouse 1
    - Subaction 
        - Mouse 2
    - Jump
        - Space
    - Reload
        - R
    - Interaction
        - F
    - DropWeapon 
        - G
    - Crouch
        - Left Ctrl
    - GunShop
        - B
- Trigger Mapping
    - LookUp 
        - Mouse Y -1.0
    - Turn 
        - Mosue X 1.0
    - MoveForward
        - W 1.0
        - S -1.0
    - MoveRight
        - A -1.0
        - D 1.0

If you need other actions or want to change actions, You can also change actions at `AFpsCharacter::SetupPlayerInputComponent`.

# How_to_Guide

# Technical_reference
## Variables
<details open>
<summary></summary>

||Name|Description|
|-|-|-|
|const FVector|DefaultLocationOfBodyMeshComponent|Location of `BodyMeshComponent` for initializing|
|const FRotator|DefaultRotatorOfBodyMeshComponent|Rotator of `BodyMeshComponent` for initializing|
|const float|InteractionReach||
|UCameraComponent*|CameraComponent||
|USkeletalMeshComponent*|BodyMeshComponent|Body mesh for players. But owner can't see|
|[AHands](./Hands.md)*|Hands||
|[AHandsModelForBody](./HandsModelForBody.md)*|HandsModelForBody|Hands Model for other players can see with `BodyMeshComponent`|
|UCharacterMovementComponent*|MovementComponent||
|TSubclassOf\<[AHUD]>|HudSubclass|HUD blueprint. HUD will be added when the player possesses on the character.|
|float|MaxHealth||
|float|Health|Current health of character|
|float|MaxArmor||
|float|Armor|Current Armor of character|
|EFpsCharacterStatus|CharacterStatus|Alive: controllable all<br/>Stopped: Controllable only camera. Can't move<br/>Freeze: Can't control<br/>Dead: Same with Freeze but the character is dead.|
|FTimerHandle|RespawnTimerHandle||
|FTransform|SpawnTransform|Transform for spawning character|
|float|AimPitch||
|float|AimYaw||
|int|CurrentHandsIndex||
|int|PreviousHandsIndex||
|TArray<AHands*>|Inventory|`AHands` instances for swap hands|
|FCollisionQueryParams|LineTraceCollisionQUeryParams|It is for detecting `InteractiveTarget` using Line Trace.|
|[AInteractiveActor](./InteractiveActor.md)*|InteractiveTarget|Default is null. When the PlayerController of the Character get InteractiveActor from Line trace|=
|[AGunShop](./GunShop.md)*|GunShop||
|bool|Plantable|For using [AHandedTimeBomb](HandedTimeBomb.md). If it is false, can't use the TimeBomb.|


</details>
