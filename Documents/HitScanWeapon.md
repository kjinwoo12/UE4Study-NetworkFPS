# HitScanWeapon
AHitScanWeapon is based on AWeaponBase. It is added `LineTrace` function. When `Action` or `Subaction` function is run then the `LineTrace` function is also run. And add damage if `AFPSCharacter` is detected.

## Inheritance Hierarchy
- [AWeaponBase](./WeaponBase.md)

# Index
- [Tutorial](#_Tutorial)
- [How to guide](#_How_to_Guide)
- [Technical reference](#_Technical_reference)

# Tutorial
## How to create a new weapon.
Same with [AWeaponBase.md](./WeaponBase.md). But there is another step for create `BulletTracer`. Follow the steps below after finish tutorial of `AWeaponBase`.

1. Find `MyContent/Weapons/Effects/BP_BulletTracer`.
2. Set `Gameplay - Bullet Tracer Blueprint` of your new weapon blueprint as `BP_BulletTracer`. You can set it other actor for using as BulletTracer.
3. Set attributes of `MuzzleFlashLight` and `MuzzleFlashTextureEffect`. You can see the effect when your weapon is on fire if there `Visible` option is true.

# How_to_Guide
Same with [AWeaponBase.md](./WeaponBase.md).

# Technical_reference
Same with [AWeaponBase.md](./WeaponBase.md).