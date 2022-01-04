# PickUpWeapon
PickUpWeapon is the base class for pickable actor. It makes for WeaponBase actor can be picked by FPSCharacter. All of the actors

# Index
- [Tutorial](#_Tutorial)
- [How to guide](#_How_to_Guide)
- [Technical reference](#_Technical_reference)

# Tutorial
## Step 1. Create a new `PickUpWeapon` BP.
1. Create a new Blueprint inherits PickUpWeapon at `C++ Class/FPS`.
2. Set static mesh of `WeaponMesh`
3. Set size of PickUpRange. It will be trigger of if is player's character near by.
4. Set Drop Sound. Default value is `MyContent/Weapon/A_Drop`. It is a SoundCue. You have to set switch parameter name is `Power`. The range of `Power` is 0(Powerful) to 5(weakness).

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
## Variables
<details open>
<summary></summary>

||Name|Description|
|-|-|-|
|UStaticMeshComponent*|WeaponMesh||
|USphereComponent*|PickUpRange||
|TSubclassOf\<AWeaponBase>|WeaponBaseSubclass||
|AWeaponBase*|WeaponInstance||
|USoundCue*|DropSound||

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
|virtual void|BeginPlay||
|void|OnOverlapBegin<br/>(<br/>&emsp;<br/>&emsp;UPrimitiveComponent* OverlappedComponent,<br/>&emsp;AActor* OtherActor,<br/>&emsp;UPrimitiveComponent* OtherComponent,<br/>&emsp;int32 OtherBodyIndex,<br/>&emsp;bool bFromSweep,<br/>&emsp;const FHitResult& SweepResult<br/>)||
|void|OnOverlapEnd<br/>(<br/>&emsp;UPrimitiveComponent* OverlappedComponent,<br/>&emsp;AActor* OtherActor,<br/>&emsp;UPrimitiveComponent* OtherComponent,<br/>&emsp;int32 OtherBodyIndex<br/>)||
|void|OnWeaponMeshComponentHit<br/>(<br/>&emsp;UPrimitiveComponent* HitComponent,<br/>&emsp;AActor* OtherActor,<br/>&emsp;UPrimitiveComponent* OtherComponent,<br/>&emsp;FVector NormalImpulse,<br/>&emsp;const FHitResult& HitResult<br/>)||
|UStaticMeshComponent*|GetWeaponMesh||
|AWeaponBase*|GetWeaponInstance||
|TSubclassOf\<[AWeaponBase](./WeaponBase.md)>|GetWeaponBaseSubclass||
|void|SetWeaponInstance<br/>(<br/>&emsp;AWeaponBase* Instance<br/>)||

</details>