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
    - PickUpWeapon
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
|const FVector|DefaultLocationOfHandsMeshComponent|Location of `HandsMeshComponent` for initializing|
|const FRotator|DefaultRotatorOfHandsMeshComponent|Rotator of `HandsMeshComponent` for initializing|
|const FVector|DefaultLocationOfBodyMeshComponent|Location of `BodyMeshComponent` for initializing|
|const FRotator|DefaultRotatorOfBodyMeshComponent|Rotator of `BodyMeshComponent` for initializing|
|const FName|NamePelvis|The name of pelvis of `BodyMeshComponent`|
|UCameraComponent*|CameraComponent||
|USkeletalMeshComponent*|HandsMeshComponent|Hands mesh for owner can only see|
|USkeletalMeshComponent*|BodyMeshComponent|Body mesh for players. But owner can't see|
|[AWeaponBase](./WeaponBase.md)*|PrimaryWeapon|Weapon that player equipped|
|[AWeaponModelForBody](./WeaponModelForBody.md)*|WeaponModelForBody|Weapon Model for other players can see with `BodyMeshComponent`|
|[APickUpWeapon](./PickUpWeapon.md)*|PickableWeapon|Default is null. When the character is able to pickup the weapon actor, It is changed as the weapon instance.|
|UCharacterMovementComponent*|MovementComponent||
|TSubclassOf\<[AHUD]>|HudSubclass|HUD blueprint. HUD will be added when the player possesses on the character.|
|float|MaxHealth||
|float|Health|Current health of character|
|float|MaxArmor||
|float|Armor|Current Armor of character|
|bool|IsDead||
|FTimerHandle|RespawnTimerHandle||
|FTransform|SpawnTransform|Transform for spawning character|
|float|AimPitch||
|float|AimYaw||
|[AGunShop](./GunShop.md)*|GunShop||


</details>

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
|void|InitializeCollisionComponent||
|void|InitializeMovementComponent||
|void|InitializeCamera||
|void|InitializeHandsMesh||
|void|InitializeBodyMesh||
|void|InitializeGameplayVariable||
|void|InitializeGunShop||
|virtual void|BeginPlay||
|void|GetLifetimeReplicatedProps<br/>(<br/>&emsp;TArray\<FLifetimeProperty>& OutLifetimeProps<br/>)||
|virtual void|Tick<br/>(<br/>&emsp;float DeltaTime<br/>)|| 
|void|TickCrosshair||
|void|ClientRPCUpdateCameraToServer||
|void|ServerRPCSetCameraRotation<br/>(<br/>&emsp;FQuat CameraRotation<br/>)||
|void|UpdateActorDirectionByAim<br/>(<br/>&emsp;float DeltaTime<br/>)||
|virtual void|SetupPlayerInputComponentTick<br/>(<br/>&emsp;UInputComponent* PlayerInputComponent<br/>)||
|void|MoveForwardTick<br/>(<br/>&emsp;float Value<br/>)||
|void|MoveRightTick<br/>(<br/>&emsp;float Value<br/>)||
|void|AddControllerPitchInputTick<br/>(<br/>&emsp;float Value<br/>)||
|void|AddControllerYawInputTick<br/>(<br/>&emsp;float Value<br/>)||
|void|Jump||
|void|CrouchPressed||
|void|CrouchReleased||
|void|ActionPressed||
|void|ActionReleased||
|void|SubactionPressed||
|void|SubactionReleased||
|void|ReloadPressed||
|void|PickUpWeaponPressed||
|void|DropWeaponPressed||
|void|GunShopPressed||
|void|OnPossessed||
|void|OnPlayerFull||
|void|OnRoundStart||
|void|ServerRPCStartAction||
|void|ServerRPCStopAction||
|void|ServerRPCStartSubaction||
|void|ServerRPCStopSubaction||
|void|ServerRPCStartReload||
|void|ServerRPCPickUpWeapon||
|void|ServerRPCDropWeapon||
|void|MulticastRPCSetActorRotation<br/>(<br/>&emsp;FRotator<br/>)||
|void|OnRep_InitializePrimaryWeapon||
|virtual float|TakeDamage<br/>(<br/>&emsp;float Damage,<br/>&emsp;FDamageEvent const& DamageEvent,<br/>&emsp;AController* EventInstigator,<br/>&emsp;AActor* DamageCauser<br/>)||
|void|Die||
|void|Respawn||
|void|KnockoutBodyMesh||
|void|WakeUpBodyMesh||
|void|EquipWeapon<br/>(<br/>&emsp;[AWeaponBase](./WeaponBase.md)* WeaponBase<br/>)||
|[AWeaponBase](./WeaponBase.md)*|UnEquipWeapon||
|void|DropWeapon||
|void|PickUpWeapon||
|float|GetHealth||
|float|GetArmor||
|UCameraComponent*|GetCameraComponent||
|[AWeaponBase](./WeaponBase.md)*|GetPrimaryWeapon||
|USkeletalMeshComponent*|GetHandsMeshComponent||
|USkeletalMeshComponent*|GetBodyMeshComponent||
|float|GetAimPtich||
|float|GetAimYaw||
|TSubclassOf\<AHUD>|GetHudSubclass||
|[AGunShop](./GunShop.md)*|GetGunShop||
|void|SetPickableWeapon<br/>(<br/>&emsp;[APickUpWeapon](./PickUpWeapon.md)* Instance<br/>)||
|void|SetSpawnTransform<br/>(<br/>&emsp;FTransform Transform<br/>)||

</details>
