# AFPSCharacter
It is the base of playable character.

# Index
- [Tutorial](#_Tutorial)
- [How to guide](#_How_to_Guide)
- [Explanation](#_Explanation)
- [Technical reference](#_Technical_reference)

# Tutorial
## Step 1. Create a new FPSCharacter BP.
1. Create a new Blueprint inherits FPSCharacter at `C++ Class/FPS`.
2. Set Skeletal Meshs
    - HandsMeshComponent : Show at first-person camera.
    - Mesh : Show this to other players.
3. Set Height and Radius of CapsuleComponent

## Step 2. Set a new FPSCharacter in your project
1. Create a new Blueprint inherits `AFPSCharacter`.
2. Open the Project Settings ( Edit -> Project settings )
3. Enter Project -> Map & Mode
4. Change Default Pawn Class to Blueprint AFPSCharacter.

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

If you need other actions or want to change actions, You can also change actions at `AFPSCharacter::SetupPlayerInputComponent`.

# How_to_Guide
## Equip_weapon
Read first [WeaponBase.md](./WeaponBase.md) tutorial. 

AWeaponBase class is equipable actor. Use `AFPSCharacter::EquipWeapon`.

## Drop_and_pick_up_weapon
Read first [PickUpWeapon.md](./PickUpWeapon.md) tutorial. 

Use `AFPSCharacter::DropWeapon` and `AFPSCharacter::PickUpWeapon`.

# Explanation
AFPSCharacter is general playable character class. 

# Technical_reference
