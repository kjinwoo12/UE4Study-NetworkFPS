# AWeaponBase
Base class for 1st Camera hands.

# Index
- [Tutorial](#_Tutorial)
- [How to guide](#_How_to_Guide)
- [Technical reference](#_Technical_reference)

# Tutorial
## How to create a new hands BP.
1. Create a new Blueprint inherits `AHands` at `C++ class/FPS/Weapon`
2. Set default variables.
    - Set `Attaching Grip Point Name`. It's for equipment system. Instances of `WeaponModelForBody` are attached to a socket of with the same name as this.
    - Set `HandsIndex`. It is for switch weapon number key. It start from 0.
3. Set skeletal mesh at `HandsMesh`.
4. Set `APickupableActor Blueprint`
    - You need to create `APickupableActor` based blueprint. Checkout [PickupableActor.md](./PickupableActor.md)
5. Set `HandsModelForBody Blueprint`
    - You need to create `HandsModelForBody` based blueprint. Checkout [HandsModelForBody.md](./WeaponModelForBody.md)

# How_to_Guide
## Equipment
```C++
//In AFpsCharacter class
AHands *Hands = GetWorld()->SpawnActor<AWeaponBase>(HandsSubclass, FVector(0, 0, 0), FRotator::ZeroRotator);
Equip(Hands);
```

# Technical_reference

## Variables
<details open>
<summary></summary>

||Name|Description|
|-|-|-|
|const FVector|DefaultLocationOfHandsMeshComponent||
|USkeletalMeshComponent*|HandsMesh||
|FName|AttachingGripPointName|It is for `HandsModelForBody`|
|int|HandsIndex|Index start at 0. But number key start at 1.|
|TSubclassOf<APickupableActor>|PickupableActorSubclass|Using at spawn actor to drop the hands.|
|TSubclassOf<AHandsModelForBody>|ModelForBodySubclass|Attached at character on 3rd-camera|

</details>

</details>

## Functions
<details open>
<summary></summary>

||Name|Description|
|-|-|-|
|virtual void|BeginPlay||
|virtual void|Initialize<br/>(<br/>&emsp;AFpsCharacter* FPSCharacter<br/>)||
|virtual void|Tick<br/>(<br/>&emsp;float DeltaTime<br/>)||
|virtual void|OnUnEquipped||
|void|StartAction||
|void|StopAction||
|void|StartSubaction||
|void|StopSubaction||
|void|StartReload||
|void|ClientRpcOnUnEquippeed||
|void|GetHandsIndex||
|FName|GetAttachingGripPointName||

</details>