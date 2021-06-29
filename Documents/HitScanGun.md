# HitScanGun
AHitScanGun is based on AHitScanWeapon. It is added Muzzle and BulletTracer from AHitScanWeapon.

## Inheritance Hierarchy
- [AWeaponBase](./WeaponBase.md)
    - [AHitScanWeapon](./HitScanWeapon.md)

# Index
- [Tutorial](#_Tutorial)
- [How to guide](#_How_to_Guide)
- [Technical reference](#_Technical_reference)

# Tutorial
## How to create a new weapon.
Same with [AHitScanWeapon.md](./WeaponBase.md). But there is another step for create `BulletTracer` and `Muzzle` effects. Follow the steps below after finish tutorial of `AWeaponBase`.

1. Find `MyContent/Weapons/Effects/BP_BulletTracer`.
2. Set the position of `Muzzle` component. `BulletTracer` will be spawned right here.
3. Set `Gameplay - Bullet Tracer Blueprint` of your new weapon blueprint as `BP_BulletTracer`. You can set it other actor for using as bullet-tracer.
4. Set attributes of `MuzzleFlashLight` and `MuzzleFlashTextureEffect`. You can see the effect when your weapon is on fire if there `Visible` option is true.

# How_to_Guide
Same with [AHitScanWeapon.md](./WeaponBase.md).

# Technical_reference
## BP_BulletTracer
It is just an actor based on `AActor`. Base directory is `MyContent/Weapon/Effects`. I recommend that don't make it for other usage etc projectile weapon.