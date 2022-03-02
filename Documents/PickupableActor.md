# PickupableWeapon
PickupableWeapon is the base class for pickable actor. It is  for WeaponBase actor can be picked by FpsCharacter.

# Index
- [Tutorial](#_Tutorial)
- [How to guide](#_How_to_Guide)
- [Technical reference](#_Technical_reference)

# Tutorial
## Step 1. Create a new `APickupableActor` BP.
1. Create a new Blueprint inherits PickUpWeapon at `C++ Class/FPS/Weapon`.
2. Set static mesh of `Mesh` component.
3. Set Drop Sound. Default value is `MyContent/Weapon/A_Drop`. It is a SoundCue. You have to set switch parameter name is `Power`. The range of `Power` is 0(Powerful) to 5(Weakness).

## Step 2. Set `PickupableActorSubclass` of your `AHands` BP
 Set your `PickupableWeapon` BP at `PickupableActorSubclass` in `WeaponBase` BP. It makes possible to drop to spawn the actor made using your `PickupableWeapon` BP. Check out the function `Hands::CreatePickupableActor`.

# How_to_Guide

# Technical_reference
## Variables
<details open>
<summary></summary>

||Name|Description|
|-|-|-|
|UStaticMeshComponent*|WeaponMesh||
|TSubclassOf\<AWeaponBase>|WeaponBaseSubclass||
|AWeaponBase*|WeaponInstance||
|USoundCue*|DropSound||

</details>