# WeaponModelForBody
It is used for `AFPSCharacter` body mesh and only other player can see.

# Index
- [Tutorial](#_Tutorial)
- [How to guide](#_How_to_Guide)
- [Technical reference](#_Technical_reference)

# Tutorial
## How to create a new WeaponModelForBody BP
1. Create a new Blueprint inherits `WeaponModelForBody` at `C++ class/FPS`.
2. Done!

# How_to_Guide


# Technical_reference
## Variables
<details open>
<summary></summary>

||Name|Description|
|-|-|-|
|USkeletalMeshComponent*|WeaponMesh||
|USceneComponent*|Muzzle||

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
|virtual_void|BeginPlay||
|virtual_void|Tick<br/>(<br/>&emsp;float DeltaTime<br/>)||