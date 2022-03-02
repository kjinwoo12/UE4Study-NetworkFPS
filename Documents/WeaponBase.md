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
3. Set `Muzzle` location. It's not necessary work. The `Muzzle` is just for effects.
4. Set Animations at `Animation - Hands` and `Animation - Body`. There are played when each function is run.
    - `Hands/Body Action Animation`
    - `Hands/Body Subaction Animation`
    - `Hands/Body Reload Animation`
5. Set Sounds at `Sound`. There are played when each function is run.
    - `Action Sound`
    - `Subaction Sound`
    - `Reload Sound`
6. Set `PickupableActor Blueprint`
    - You need to create `PickupableActor` based blueprint. Checkout [PickupableActor.md](./PickupableActor.md)
7. Set `HandsModelForBody Blueprint`
    - You need to create `HandsModelForBody` based blueprint. Checkout [HandsModelForBody.md](./HandsModelForBody.md)

# How_to_Guide
## Equip weapon
Same with [Hands.md - #Equipment](./Hands.md). Use `AFpsCharacter::Equip`

# Technical_reference

## Variables
<details open>
<summary></summary>

||Name|Description|
|-|-|-|
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
|UAnimInstance*|BodyAnimInstance||
|UAnimMontage*|HandsActionAnimation||
|UAnimMontage*|HandsSubactionAnimation||
|UAnimMontage*|HandsReloadAnimation||
|UAnimMontage*|BodyActionAnimation||
|UAnimMontage*|BodySubactionAnimation||
|UAnimMontage*|BodyReloadAnimation||
|USoundBase*|ActionSound||
|USoundBase*|SubactionSound||
|USoundBase*|ReloadSound||WeaponModelForBodySubclass||
|FTimerHandle|TimerHandle||
|void|(AWeaponBase::* FunctionAfterDelay)()||
|FTimerHandle|TimerHandleForExtraInput||
|void|(AWeaponBase::* FunctionAfterDelayForExtraInput)()||

</details>