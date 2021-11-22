# AFPSHUD
HUD with dynamic crosshair. It is used by [FpsCharacter](./FpsCharacter.md).

# Index
- [Tutorial](#_Tutorial)
- [How to guide](#_How_to_Guide)
- [Technical reference](#_Technical_reference)

# Tutorial
## Step 1. Create a new `FpsHud` BP
1. Create a new Blueprint inherits FPSHUD at `C++ class/FPS`.
2. Set `Corsshair texture`.
    - Crosshair is composed of 4 items. The texture is for those items. Please upload bottom texture only. FPSHUD makes them attaching your texture and rotate automatically.
    - example
        - Correct texture -> <img src="img/CrosshairPoint.png" width = "30px" style="vertical-align: middle">
        - Wrong texture -> <img src="img/CrosshairPoint_worng.png" width = "100px" style="vertical-align: middle">
3. Set `Default Widget`

# How_to_Guide

# Technical_reference
## Related Blueprints
- `MyContent/UI/FpsHud`
    - `BP_FpsHud` : Child HUD for FPSCharacter

## Variables
<details open>
<summary></summary>

||Name|Description|
|-|-|-|
|float|CrosshairDefaultOffset||
|float|CrosshairOffset||
|UTexture2D*|CrosshairTexture||
|TSubclassOf<UUserWidget>|DefaultWidgetClass||
|UUserWidget*|DefaultWidget||
|TSubclassOf<UUserWidget>|GunShopWidgetClass||
|UGunShopWidget*|GunShopWidget|Blueprint Widget Instance from `GunShopWidgetClass`|

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
|virtual void|DrawHUD||
|virtual void|BeginPlay||
|void|InitDefaultWidget||
|void|InitGunShopWidget||
|void|DrawCrosshair|Called every tick|
|void|OpenGunShop||
|void|CloseGunShop||
|bool|IsOpenGunShop||
|void|SetCrosshairCenterOffset<br/>(<br/>&emsp;float Value<br/>)||

</details>

