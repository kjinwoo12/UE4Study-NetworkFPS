# HitScanWeapon
AHitScanWeapon is based on AWeaponBase. There is the function `LineTrace()`. It is run with When `OnAction` or `OnSubaction` function is run by Player. And it gives damage to `AFPSCharacter` if it is detected by `LineTrace()`

## Inheritance Hierarchy
- [AWeaponBase](./WeaponBase.md)

# Index
- [Tutorial](#_Tutorial)
- [How to guide](#_How_to_Guide)
- [Technical reference](#_Technical_reference)

# Tutorial
## How to create a new weapon.
Same with [AWeaponBase.md](./WeaponBase.md).

# How_to_Guide
Same with [AWeaponBase.md](./WeaponBase.md).

# Technical_reference
## Variables
<details open>
<summary></summary>

||Name|Description|
|-|-|-|
|float|Reach||
|FCollisionQueryParams|LineTraceCollisionQueryParams||

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
|virtual void|Initialize<br/>(<br/>&emsp;[AFpsCharacter](./FpsCharacter.md)* FPSCharacter<br/>)||
|virtual void|OnUnEquipped||
|virtual void|OnAction||
|bool|LineTrace<br/>(<br/>&emsp;FHitResult& HitResult<br/>)||

</details>