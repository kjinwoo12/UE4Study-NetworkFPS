# AFPSHUD
HUD with dynamic crosshair. It is used by [FPSCharacter](./FPSCharacter.md).

# Tutorial
## How to use
1. Create a new Blueprint inherits FPSHUD at `C++ class/FPS`.
2. Set `Corsshair texture`.
    - Crosshair is composed of 4 items. The texture is for those items. Please upload bottom texture only. FPSHUD makes them attaching your texture and rotate automatically.
    - example
        - Correct texture -> <img src="img/CrosshairPoint.png" width = "30px" style="vertical-align: middle">
        - Wrong texture -> <img src="img/CrosshairPoint_worng.png" width = "30px" style="vertical-align: middle">