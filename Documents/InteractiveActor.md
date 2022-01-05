# InteractiveActor
`InteractionActor` is the base class for interactable actor. Checkout `SetupPlayerInputComponent` at `FpsCharacter`. There are two binded actions is named `"Interaction"`.

# Index
- [Tutorial](#_Tutorial)
- [How to guide](#_How_to_Guide)
- [Technical reference](#_Technical_reference)

# Tutorial

# How_to_Guide


# Technical_reference
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
|virtual void|OnTargetedBy<br/>(<br/>&emsp;ACharacter*<br/>)|This is called when the `ACharacter` becomes available to interact with|
|virtual void|OnUntargetedBy<br/>(<br/>&emsp;ACharacter*<br/>)|This is called when interaction with the `ACharcter` is blocked|
|virtual void|OnInteractWith<br/>(<br/>&emsp;ACharacter*<br/>)|This is called when the Actor interact with `ACharacter`|
|virtual void|OnInteractionStop<br/>(<br/>&emsp;ACharacter*<br/>)|This is called when interaction is finished|