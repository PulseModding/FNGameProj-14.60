# AsherUtils Plugin

A UE4 4.26.0 editor plugin that streamlines character data asset creation.

## Features

- **One-Click Character Creation**: Creates a complete set of character data assets with a single toolbar button click
- **Automatically Creates**:
  - Character Parts (Head, Body, Hat, Backpack, Face, Charm)
  - Hero Specialization
  - Hero Type
  - Athena Character Item Definition
- **Pre-configured**: All assets are properly linked and configured automatically

## Installation

1. Copy the `AsherUtils` folder to your project's `Plugins` directory
2. If the `Plugins` directory doesn't exist, create it in your project root
3. Restart Unreal Engine or regenerate project files
4. Enable the plugin in Edit → Plugins → Project → Editor

## Usage

1. In the Content Browser, navigate to the folder where you want to create your character assets
2. Click the **Create Character** button in the toolbar (located near the Play button)
3. The plugin will automatically create all necessary data assets in a structured folder hierarchy

## Created Asset Structure

```
YourSelectedFolder/
└── NewCharacter/
    ├── NewCharacter_Character (UAthenaCharacterItemDefinition)
    ├── NewCharacter_HeroType (UFortHeroType)
    ├── Parts/
    │   ├── NewCharacter_Head
    │   ├── NewCharacter_Body
    │   ├── NewCharacter_Hat
    │   ├── NewCharacter_Backpack
    │   ├── NewCharacter_Face
    │   └── NewCharacter_Charm
    └── Specializations/
        └── NewCharacter_Specialization
```

## Custom Icon

To use your own toolbar icon:

1. Replace `Resources/Icon40.png` with your 40x40 pixel icon
2. Restart the editor

## Requirements

- Unreal Engine 4.26.0 (source build)
- FortniteGame module (your project must have access to the Fortnite classes)

## Notes

- All created assets are automatically saved
- The Content Browser will sync to the main character asset after creation
- Character parts are pre-configured with appropriate types and gender settings
