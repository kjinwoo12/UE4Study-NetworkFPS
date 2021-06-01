# AWeaponBase
WeaponBase is the base class for a gun and knife.

# Index
- [Tutorial](#_Tutorial)
- [How to guide](#_How_to_Guide)
- [Technical reference](#_Technical_reference)

# Tutorial
## How to create a new weapon BP.
1. Create a new Blueprint inherits `WeaponBase` at `C++ class/FPS`.
2. Set default variables.
    - Set `Attaching Grip Point Name`. It's for equipment system. Instances of `AWeaponBase` and `WeaponModelForBody` is attached to a socket with the same name as this.
    - Set `WeaponType`. It's for holding weapon motion. You can checkout what is the motion for `WeaponType` at `MyContent/Animation/ABP_ThirdPerson`
        - None <img src="./img/CharacterMotion_WeaponType_None.jpg"/>
        - Rifle <img src="./img/CharacterMotion_WeaponType_Rifle.jpg"/>
        - Knife <img src="./img/CharacterMotion_WeaponType_Knife.jpg"/>
    - Set `Reach`. It is range of your weapon.
    - Set `ActionLoopEnable` and `SubactionLoopEnable`. They allow to repeat `OnAction()` and `OnSubaction()` until player release key : `Mouse1` and `Mouse2`.
    - Set `Delay Action/Subaction`. It makes extra time between each actions/subactions. If the value is 0, your weapon is going to be extremely fast but i don't want to recommend it.
    - Set `Ammo` information.
        - `Magazine Size` : The size of one magazine
        - `Current Ammo` : Left ammo of magazine
        - `Sub Ammo` : Total ammo player has.
        - `Is Ammo Infinite` : The title says it all.
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
