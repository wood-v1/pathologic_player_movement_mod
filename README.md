# Pathologic Player Movement Mod

PPMM adds configurable sprint and movement tweaks to Pathologic Classic HD.

## Features

- Hold `Shift` to sprint
- Use `Caps Lock` to toggle sprint
- Configure movement speed, jump height, and landing gravity
- Optionally increase vanilla Exhaustion gradually while the player is sprinting and physically moving

## Configuration

Settings are stored in `PPMM.ini`:

```ini
[General]
Debug=0
TirednessEffect=1

[Constants]
MoveSpeedMultiplier=0.5
JumpHeight=1.05
JumpLandingGravity=-700
TirednessDelta=30
```

## PPMM.ini Settings Guide

### `[General]`

**`Debug`**

- Description: Enables the debug console for testing purposes. Set to `1` to enable or `0` to disable.
- Type: boolean
- Default value: `0`

**`TirednessEffect`**

- Description: Enables Exhaustion gain while sprinting. Set to `1` to enable or `0` to disable.
- Type: boolean
- Default value: `1`

### `[Constants]`

**`MoveSpeedMultiplier`**

- Description: Controls player movement speed. Lower values make the player run faster. In more detail, it is a multiplier of the friction factor, so the smaller this force, the faster the character moves.
- Type: float
- Default value: `1.0`

**`JumpHeight`**

- Description: Controls the height of jumps. Higher values make the player jump higher.
- Type: float
- Default value: `1.0`

**`JumpLandingGravity`**

- Description: Controls how fast the player falls after a jump. Higher values make the player lighter and fall slower. This constant also affects fall damage and collisions with obstacles. Setting it to `0` will prevent the player from falling after a jump.
- Type: integer
- Default value: `2500`

**`TirednessDelta`**

- Description: Controls how quickly Exhaustion increases during sprint. Higher values result in faster Exhaustion buildup.
- Type: integer
- Default value: `30`

## Public API

Consumers can include [`include/PPMMApi.h`](include/PPMMApi.h) and resolve `PpmmGetApi` from `PPMM.dll`.

Call `PpmmGetApi(PPMM_API_VERSION_1)` to request the current function table. The function returns `nullptr` when the requested version is unsupported. Consumers should validate both `version` and `size` before accessing table members.

### API v1

**`SetExternalSprintBlocked(BOOL blocked)`**

- Requests an external sprint block when called with `TRUE`.
- Clears the `Caps Lock` sprint toggle and restores regular movement from the PPMM main loop.
- Prevents a held `Shift` key from immediately restarting sprint until the key is released.
- Removes the external block when called with `FALSE`.
- Returns `TRUE` after storing the requested state.

**`IsSprintActive()`**

- Returns `TRUE` while PPMM currently considers sprint active.
- Returns `FALSE` while regular movement is active or an external sprint block is applied.

**`SetExternalTirednessSuppressed(BOOL suppressed)`**

- Temporarily suppresses PPMM Exhaustion updates when called with `TRUE`.
- Restores configured `TirednessEffect` behavior when called with `FALSE`.
- Returns `TRUE` after storing the requested state.

PPMM remains the owner of sprint input, the `Caps Lock` toggle, and movement hooks. API calls store state atomically; PPMM applies movement changes from its own main loop.

## Build

PPMM is a Win32/x86 CMake DLL and depends on [OynonTools](https://github.com/wood-v1/oynon_tools_api). By default the repository is expected at `../OynonTools`.

```powershell
cmake -S . -B build-win32 -A Win32
cmake --build build-win32 --config Release
```

The tiredness Lua script must be compiled with [pathologic_lua_compiler](https://github.com/wood-v1/pathologic_lua_compiler).

## Deploy

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\deploy.ps1 -GameRoot "D:\Games\Pathologic Classic HD"
```

The deploy script builds OynonTools and PPMM, compiles `ppmm_stats_effect.lua`, copies the required files, and verifies SHA-256 hashes.

## Launcher

```ini
LoadOrder=OynonTools.dll@suspended, PPMM.dll@engine
```

## Changelog

### v1.3

- Added public mod API
- Improved internal architecture
- Added deployment script

### v1.2

- Reworked runtime integration to use OynonTools

### v1.1

- Added the optional tiredness effect during sprint
- Added `TirednessEffect` in `[General]`
- Added `TirednessDelta` in `[Constants]`
