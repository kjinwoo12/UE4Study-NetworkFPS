# PickUpWeapon
PickUpWeapon is the base class of pickable actor. It makes WeaponBase actor can be picked by FPSCharacter. All of the actors

# Index
- [Tutorial](#_Tutorial)
- [How to guide](#_How_to_Guide)
- [Technical reference](#_Technical_reference)

# Tutorial
## Step 1. Create a new `PickUpWeapon` BP.
1. Create a new Blueprint inherits PickUpWeapon at `C++ Class/FPS`.
2. Set static mesh of `WeaponMesh`
3. Set size of PickUpRange. It will be trigger of if is player's character near by. 

## Step 2. Set `PickUpWeaponBlueprint` of your `WeaponBase` BP to `PickUpWeapon` BP what you had make at step 1
Set your `PickUpWeapon` BP at `PickUpWeaponBlueprint` in `WeaponBase` BP. It makes possible to spawn the actor made using your `PickUPWeapon` BP. Check out the function `WeaponBase::SpawnPickUpWeaponActor`. It is a preparing for [How to drop weapon](##_Drop_weapon)

## Step 3. Use AWeaponBase::SpawnPickUpWeaponActor and initialize variables.
1. Create a WeaponBase Actor. Check out [WeaponBase.md](./WeaponBase.md) tutorial.
2. There is a function of the Actor, `SpawnPickUpWeaponActor`
    ```C++
    AWeaponBase *Weapon = /*Create or Get WeaponBaseActor*/;
    APickUpWeapon *PickUpWeapon = Weapon->SpawnPickUpWeaponActor();
    ```
3. Set Weapon instance for if character pick up weapon.
    ```C++
    PickUpWeapon->SetWeaponInstance(Weapon);
    ```

# How_to_Guide

# Technical_reference