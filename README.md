# Pathologic Player Movement Mod

## Overview

This mod adds configurable sprint and movement tweaks to Pathologic Classic HD.

It works by applying runtime patches inside `Engine.dll` and adjusting movement-related values.

## Features

- Configurable sprint (Shift / Caps Lock toggle)
- Movement speed multiplier
- Jump height multiplier
- Landing gravity adjustment
- Centralized debug console via `GameModLauncher.ini`

### Tiredness Effect

- Optional tiredness effect that applies exhaustion during sprint
- The Exhaustion stat slowly grows during sprint
- The Exhaustion stat changes only while active movement

## Build

This project is built as a Win32/x86 DLL with CMake and MSVC.

It depends on [OynonTools](https://github.com/wood-v1/oynon_tools_api), which provides the external API and libraries used by the mod to interact with the game at runtime. By default, `CMakeLists.txt` looks for that repository at `../OynonTools`, but you can also set `OYNONTOOLS_ROOT` manually when configuring the project.

Typical flow:

1. Clone this repository.
2. Clone `oynon_tools_api` next to it, or point `OYNONTOOLS_ROOT` to your local copy.
3. Configure CMake for Win32/x86.
4. Build the `PPMM` target.

The Lua sources used by the tiredness effect live in [`scripts/`](scripts/). They are intended to be compiled with [pathologic_lua_compiler](https://github.com/wood-v1/pathologic_lua_compiler), which is a small compiler/builder for Pathologic Lua scripts. In this repository, those scripts provide the timed loop and the exhaustion update logic used by the runtime patch.

## How it works

- Applies runtime patches to the game process
- Hooks movement-related code inside `Engine.dll`
- Uses code caves to modify values at runtime (see [Engine_dll_asm.txt](Engine_dll_asm.txt))
- Patches hardcoded constants
- Uses the game console bridge to start the tiredness script (see [scripts/ppmm_stats_effect.lua](scripts/ppmm_stats_effect.lua))

All characters share the same movement system, so changes apply globally.

## Controls

- **Hold Shift**  - Sprint
- **Caps Lock**  - Toggle sprint on/off

## Configuration

Settings are stored in `PPMM.ini`.

Example:

    [General]
    TirednessEffect=1

    [Constants]
    MoveSpeedMultiplier=0.43
    JumpHeight=1.078
    JumpLandingGravity=-600
    TirednessDelta=30

## Changelog

### v1.2

- Internal architecture was reworked

### v1.1

- Added the optional tiredness effect during sprint
- Added `TirednessEffect` in `[General]`
- Added `TirednessDelta` in `[Constants]`

## PPMM.ini Settings Guide

### [General]

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
