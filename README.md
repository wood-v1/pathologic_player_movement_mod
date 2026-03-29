# Pathologic Player Movement Mod (PPMM)

## Overview

This mod adds configurable sprint and movement tweaks to Pathologic Classic HD.

It works by patching the game's assembly at runtime (inside `Engine.dll`) and modifying movement-related values.

## Features

- Configurable sprint (Shift / Caps Lock toggle)
- Movement speed multiplier
- Jump height multiplier
- Landing gravity adjustment
- Optional debug console

## How it works

- Injects into the game process
- Hooks movement-related code inside `Engine.dll`
- Uses code caves to modify values at runtime (see Engine_dll_asm.txt)
- Patches hardcoded constants

All characters share the same movement system, so changes apply globally.

## Controls

- **Hold Shift**  - Sprint
- **Caps Lock**  - Toggle sprint on/off

## Configuration

Settings are stored in `PPMM.ini`.

Example:

```ini
[General]
Debug=1

[Constants]
MoveSpeedMultiplier=0.43
JumpHeight=1.078
JumpLandingGravity=-600
```

## PPMM.ini Settings Guide

### [General]

**Debug**

* Description - 1 To Enable debug information (only for testing purposes). 0 to Disable debug information
* Type - boolean
* Default value - false

### [Constants]

**MoveSpeedMultiplier**

* Description - Controls player movement speed. Lower values make the player run faster.﻿
* Type - float
* Default value - 1.0

**JumpHeight**

* Description - Controls the height of jumps. Higher values make the player jump higher.﻿
* Type - float
* Default value - 1.0

**JumpLandingGravity**

* Description - Controls how fast the player falls after a jump. Higher values make the player "lighter" and fall slower. Constant also affects fall damage and collisions with obstacles. Setting it to 0 will prevent the player from falling after a jump.
* Type - integer
* Default value - 2500
