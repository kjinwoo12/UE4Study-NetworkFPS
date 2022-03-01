# AWeaponBase
WeaponBase is the base class for a gun and knife.

# Index
- [Tutorial](#_Tutorial)
- [How to guide](#_How_to_Guide)
- [Technical reference](#_Technical_reference)

# Tutorial
## How to create a new weapon BP.
1. Create a new Blueprint inherits `WeaponBase` at `C++ class/FPS/Weapon`. And Follow the tutorial of [Hands.md](./Hands.md)
2. Set properties value.
    - Set `Action/Subaction Delay`. It makes extra time between each actions/subactions. If the value is 0, your weapon is going to be extremely fast but i don't want to recommend it.
    - Set `ActionLoopEnable` and `SubactionLoopEnable`. They allow to repeat `OnAction()` and `OnSubaction()` until player release key : `Mouse1` and `Mouse2`.
    - Set `ReloadDelay`. It means time to finish reload.
    - Set `Ammo` information.
        - `Magazine Size` : The size of one magazine
        - `Current Ammo` : Left ammo of magazine
        - `Sub Ammo` : Total ammo player has.
        - `Is Ammo Infinite` : The title says it all.
    - Set `Damage`.
    
3. Set skeletal mesh at `WeaponMesh`. 
4. Set `Muzzle` location. It's not necessary work. `Muzzle` is just for effects. You can handle it.
5. Set Animations at `Animation - Hands` and `Animation - Body`. There are played when each function is run.
    - `Hands/Body Action Animation`
    - `Hands/Body Subaction Animation`
    - `Hands/Body Reload Animation`
6. Set Sounds at `Sound`. There are played when each function is run.
    - `Action Sound`
    - `Subaction Sound`
    - `Reload Sound`
7. Set `PickUpWeapon Blueprint`
    - You need to create `PickUpWeapon` based blueprint. Checkout [PickUpWeapon.md](./PickUpWeapon.md)
8. Set `WeaponModelForBody Blueprint`
    - You need to create `WeaponModelForBody` based blueprint. Checkout [WeaponModelForBody.md](./WeaponModelForBody.md)

# How_to_Guide
## Equip weapon
1. Copy blueprint reference of your weapon.
2. Modify
    - original : Blueprint'/Game/MyContent/Blueprints/BP_WeaponBase_TestGun.BP_WeaponBase_TestGun'
    - Modified : `Class`'/Game/MyContent/Blueprints/BP_WeaponBase_TestGun.BP_WeaponBase_TestGun`_C`'
3. Use `EquipWeapon(const TCHAR*)` in `FPSCharacter`. The param is the reference what you modified.
```C++
//In AFPSCharacter class
AWeaponBase *WeaponBase = AWeaponBase::SpawnWeapon(GetWorld(), "Class'/Game/MyContent/Blueprints/BP_WeaponBase_TestGun.BP_WeaponBase_TestGun_C'");
EquipWeapon(WeaponBase)
```
The actor WeaponBase is spawned by SpawnWeapon at (0, 0, 0) in world location. and EquipWeapon  

# Technical_reference

## Variables
<details open>
<summary></summary>

||Name|Description|
|-|-|-|
|USkeletalMeshComponent*|WeaponMesh||
|FName|AttachingGripPointName||
|EWeaponType|WeaponType||
|float|ActionDelay||
|float|SubactionDelay||
|bool|ActionLoopEnable||
|bool|SubactionLoopEnable||
|float|ReloadDelay||
|int|MagazineSize||
|int|CurrentAmmo||
|int|SubAmmo||
|bool|IsAmmoInfinite||
|float|Accuracy||
|float|MovementStability||
|float|Damage||
|About|animation||
|UAnimInstance*|HandsAnimInstance||
|UAnimInstance*|BodyAnimInstance||
|UAnimMontage*|HandsActionAnimation||
|UAnimMontage*|HandsSubactionAnimation||
|UAnimMontage*|HandsReloadAnimation||
|UAnimMontage*|BodyActionAnimation||
|UAnimMontage*|BodySubactionAnimation||
|UAnimMontage*|BodyReloadAnimation||
|USoundBase*|ActionSound||
|USoundBase*|SubactionSound||
|USoundBase*|ReloadSound||
|TSubclassOf\<[APickUpWeapon](./PickUpWeapon.md)>|PickUpWeaponSubclass||
|TSubclassOf\<[AWeaponModelForBody](WeaponModelForBody.md)>|WeaponModelForBodySubclass||
|FTimerHandle|TimerHandle||
|void|(AWeaponBase::* FunctionAfterDelay)()||
|FTimerHandle|TimerHandleForExtraInput||
|void|(AWeaponBase::* FunctionAfterDelayForExtraInput)()||

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
|virtual void|BeginPlay||
|virtual void|Initialize<br/>(<br/>&emsp;AFpsCharacter* FPSCharacter<br/>)||
|virtual void|OnUnEquipped||
|void|StartAction||
|void|StopAction||
|void|StartSubaction||
|void|StopSubaction||
|void|StartReload||
|virtual void|OnAction||
|void|MulticastRPCOnActionFx||
|void|OnSubaction||
|void|OnReload||
|float|GetDelay||
|float|GetMovementStability||
|int|GetCurrentAmmo||
|int|GetSubAmmo||
|FName|GetAttachingGripPointName||
|void|SetHandsAnimInstance<br/>(<br/>&emsp;UAnimInstance* AnimInstance<br/>)||
|void|SetBodyAnimInstance<br/>(<br/>&emsp;UAnimInstance* AnimInstance<br/>)||
|[APickUpWeapon](./PickUpWeapon.md)*|SpawnPickUpWeaponActor||
|[AWeaponModelForBody](./WeaponModelForBody.md)*|SpawnWeaponModelForBodyActor||
|static [AWeaponBase](./WeaponBase.md)*|SpawnWeapon<br/>(<br/>&emsp;UWorld* World,<br/>&emsp;UClass* GeneratedBP<br/>)||

</details>