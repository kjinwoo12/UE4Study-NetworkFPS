# GunShop
`AGunShop` is an invisible actor. The `AGunshop` is in charge of running important functions like RPC on GunShop Widget. Because widget or HUD doesn't exist at server so it can't call RPC functions.

# Index
- [Tutorial](#_Tutorial)
- [How to guide](#_How_to_Guide)
- [Technical reference](#_Technical_reference)

# Tutorial

# How_to_Guide
## How to add a new Weapon at GunShop Widget
1. Create a new WeaponBase. Check out [WeaponBase Tutorial](./WeaponBase.md#Tutorial).
2. Add a new data at `DT_WeaponList`.
3. Done! Check out the new product at GunShop!

# Technical_reference
## Related Blueprints
- `MyContent/UI/GunShop/`
    - `BP_WeaponListItemData` : Actual data for Listview at GunShopWidget.
    - `DT_WeaponList` : For list up products at GunShop
    - `SWeaponListItemData` : Basic structure using at `DT_WeaponList`
        - Name : is Displayed as text on GunShop.
        - Image : is Displayed as Texture2D on GunShop.
        - WeaponBase : Subclass to spawn weapon after buying.
    - `WBP_GunShopWidget` : GunShop UI.
    - `WBP_WeaponListItemWidget` : Element of Listview at GunShopWidget.

## Variables
<details open>
<summary></summary>

||Name|Description|
|-|-|-|

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
|void|ServerRpcBuyItem<br/>(<br/>&emsp;TSubclassOf\<[AWeaponBase](./WeaponBase.md)> WeaponBlueprint<br/>)||

</details>