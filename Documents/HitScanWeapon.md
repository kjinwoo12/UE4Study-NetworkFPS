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
## - Functions
|Return|Name|Description|
|---:|:---|:---|
|bool|LineTrace(FHitResult&)|Detect Pawn From "Camera.position" to "Camera.Direction * `reach`". 
## - Variables
|Type|Name|Description|
|---:|:---|:---|
|float|Reach|Range of possible to detect using `LineTrace(FHitResult&)`|