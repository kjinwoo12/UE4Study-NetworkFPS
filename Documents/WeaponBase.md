# AWeaponBase
WeaponBase is the base class for a gun and knife.

# Index
- [Tutorial](#_Tutorial)
- [How to guide](#_How_to_Guide)
- [Technical reference](#_Technical_reference)

# Tutorial
## How to create a new weapon.
1. Create a new Blueprint inherits WeaponBase at `C++ class/FPS`.
2. Set default variables.
    - Set `Reach`. It is range of your weapon.
    - Set `ActionLoopEnable` and `SubactionLoopEnable`. They allow to repeat `OnAction()` and `OnSubaction()` until player release key : Mouse1 and Mouse2.
    - Set Delay. It makes extra time between each actions. If the value is 0, your weapon is going to be extremely fast.
    - Set Ammo information.
3. Set skeletal mesh at `WeaponMesh`.
4. Set `Muzzle` location. It's not necessary work. `Muzzle` is just for effects. You can handle it.
5. Set Animations and Sounds at Gameplay tab.

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
