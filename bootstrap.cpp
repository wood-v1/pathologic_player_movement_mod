#include "framework.h"
#include "bootstrap.h"
#include "config.h"
#include "debug.h"
#include "ppmm_api_internal.h"
#include "tiredness_effect.h"
#include "OynonToolsApi.h"

namespace
{
void ResetMovementToDefault()
{
    OynonSetMovementFrictionMultiplier(1.0f);
    OynonSetMovementJumpHeightMultiplier(1.0f);
    OynonSetMovementLandingGravity(-2500);
}
}

DWORD WINAPI MainThread(LPVOID)
{
    LoadConfig();
    TirednessEffectInitialize();

    DebugLog("PPMM.dll injected successfully!\n");
    DebugLog("Config loaded\n");

    DWORD flags =
        OYNON_HOOK_MOVEMENT_FRICTION |
        OYNON_HOOK_MOVEMENT_VERTICAL;
    if (g_tiredness_effect) {
        flags |= OYNON_HOOK_CONSOLE_EXECUTE;
    }
    if (!OynonInitializeHooksWhenReady(flags)) {
        DebugLog("OynonTools initialization failed for shared hooks\n");
    }
    else {
        DebugLog("Shared hooks installed\n");
    }

    bool toggleSpeed = false;
    bool prevCapsState = false;
    bool wasSprinting = false;
    bool blockSprintUntilShiftRelease = false;

    while (true)
    {
        const bool shiftHeldRaw = (GetAsyncKeyState(VK_LSHIFT) & 0x8000) != 0;
        const bool externalBlocked = PpmmIsExternalSprintBlocked();
        if (externalBlocked) {
            toggleSpeed = false;
            blockSprintUntilShiftRelease =
                blockSprintUntilShiftRelease || shiftHeldRaw;
        }

        const bool capsPressedNow = (GetAsyncKeyState(VK_CAPITAL) & 0x0001) != 0;
        if (capsPressedNow && !prevCapsState &&
            !externalBlocked && !blockSprintUntilShiftRelease) {
            toggleSpeed = !toggleSpeed;
            DebugLog(toggleSpeed ? "Speed toggled ON\n" : "Speed toggled OFF\n");
        }
        prevCapsState = capsPressedNow;

        if (blockSprintUntilShiftRelease && !shiftHeldRaw) {
            blockSprintUntilShiftRelease = false;
            DebugLog("Sprint input unlocked after external block\n");
        }

        const bool sprintActive =
            !externalBlocked &&
            !blockSprintUntilShiftRelease &&
            (shiftHeldRaw || toggleSpeed);

        TirednessEffectUpdate(sprintActive);

        if (sprintActive && !wasSprinting) {
            OynonSetMovementFrictionMultiplier(g_speed);
            OynonSetMovementJumpHeightMultiplier(g_jump_height);
            OynonSetMovementLandingGravity(g_landing_gravity);
        }
        else if (!sprintActive && wasSprinting) {
            ResetMovementToDefault();
        }

        wasSprinting = sprintActive;
        PpmmSetSprintActive(sprintActive);
        Sleep(1);
    }

    return 0;
}
