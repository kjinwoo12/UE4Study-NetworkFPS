# GunShop
GunShop is a widget using at FPSHUD. Player can open the widget press `B` button on keyboard.

# Index
- [Technical reference](#_Technical_reference)
- [Tutorial](#_Tutorial)
- [How to guide](#_How_to_Guide)

# Technical_reference
- Type : Blueprint Widget
- Path : `MyContent/UI/GunShop/`
    - `BP_WeaponListItemData` : Actual data for Listview at GunShopWidget.
    - `DT_WeaponList` : For list up products at GunShop
    - `SWeaponListItemData` : Basic structure using at `DT_WeaponList`
        - Name : is Displayed as text on GunShop.
        - Image : is Displayed as Texture2D on GunShop.
        - WeaponBase : Subclass to spawn weapon after buying.
    - `WBP_GunShopWidget` : GunShop UI.
    - `WBP_WeaponListItemWidget` : Element of Listview at GunShopWidget.

# Tutorial

# How_to_Guide
## How to add a new Weapon at GunShop
1. Create a new WeaponBase. Check out [WeaponBase Tutorial](./WeaponBase.md#Tutorial).
2. Add a new data at `DT_WeaponList`.
3. Done! Check out the new product at GunShop!