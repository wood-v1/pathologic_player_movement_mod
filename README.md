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

## Tiredness Effect (v1.1 update)

- Optional tiredness effect that applies exhaustion during sprint
- The Exhaustion stat slowly grows during sprint
- The Exhaustion stat changes only while active movement

## How it works

- Injects into the game process
- Hooks movement-related code inside `Engine.dll`
- Uses code caves to modify values at runtime (see [Engine_dll_asm.txt](Engine_dll_asm.txt))
- Patches hardcoded constants
- Injects into the game console to call a script that increases exhaustion (see [ppmm_stats_effect.sc](ppmm_stats_effect.sc)) 

All characters share the same movement system, so changes apply globally.

## Controls

- **Hold Shift**  - Sprint
- **Caps Lock**  - Toggle sprint on/off

## Configuration

Settings are stored in `PPMM.ini`.

Example:

    [General]
    Debug=1
    TirednessEffect=1

    [Constants]
    MoveSpeedMultiplier=0.43
    JumpHeight=1.078
    JumpLandingGravity=-600
    TirednessDelta=30

## PPMM.ini Settings Guide

### [General]

**Debug**

* Description - 1 to enable debug information (only for testing purposes), 0 to disable  
* Type - boolean  
* Default value - 0  

**TirednessEffect**

* Description - Enables exhaustion gain while sprinting. When enabled, sprinting will gradually increase player exhaustion.  
* Type - boolean  
* Default value - 0  

---

### [Constants]

**MoveSpeedMultiplier**

* Description - Controls player movement speed. Lower values make the player run faster.  
* Type - float  
* Default value - 1.0  

**JumpHeight**

* Description - Controls the height of jumps. Higher values make the player jump higher.  
* Type - float  
* Default value - 1.0  

**JumpLandingGravity**

* Description - Controls how fast the player falls after a jump. Higher values make the player "lighter" and fall slower. Constant also affects fall damage and collisions with obstacles. Setting it to 0 will prevent the player from falling after a jump.  
* Type - integer  
* Default value - 2500  

**TirednessDelta**

* Description - Controls how quickly exhaustion increases during sprint. Higher values result in faster exhaustion buildup.  
* Type - integer  
* Default value - 30  