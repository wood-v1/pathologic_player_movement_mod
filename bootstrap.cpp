#include "framework.h"
#include "bootstrap.h"
#include "config.h"
#include "debug.h"
#include "OynonToolsApi.h"

#include <string>

DWORD WINAPI TirednessEffectStart(LPVOID)
{
    std::string cmd = "setvar ppmm_tiredness_delta " + std::to_string(g_tiredness_delta);
    OynonExecCommand(cmd.c_str());
    Sleep(100);
    OynonExecCommand("effect player ppmm_stats_effect.bin");

    DebugLog("Sprint effect applied\n");
    return 0;
}

DWORD WINAPI TirednessEffectStop(LPVOID)
{
    OynonExecCommand("trigger player ppmm_stats_effect_stop");

    DebugLog("Sprint effect removed\n");
    return 0;
}

DWORD WINAPI MainThread(LPVOID)
{
    LoadConfig();

    DebugLog("PPMM.dll injected successfully!\n");
    DebugLog("Config loaded\n");

    const DWORD flags =
        OYNON_HOOK_CONSOLE_EXECUTE |
        OYNON_HOOK_MOVEMENT_FRICTION |
        OYNON_HOOK_MOVEMENT_VERTICAL;
    if (!OynonInitializeHooksWhenReady(flags)) {
        DebugLog("OynonTools initialization failed for shared hooks\n");
    }
    else {
        DebugLog("Shared hooks installed\n");
    }

    bool toggleSpeed = false;
    bool prevCapsState = false;

    bool isSprinting = false;
    bool wasSprinting = false;

    while (true)
    {
        const bool capsPressedNow = (GetAsyncKeyState(VK_CAPITAL) & 0x0001) != 0;
        if (capsPressedNow && !prevCapsState)
        {
            toggleSpeed = !toggleSpeed;
            DebugLog(toggleSpeed ? "Speed toggled ON\n" : "Speed toggled OFF\n");
        }
        prevCapsState = capsPressedNow;

        isSprinting = ((GetAsyncKeyState(VK_LSHIFT) & 0x8000) || toggleSpeed);

        if (g_tiredness_effect) {
            if (isSprinting && !wasSprinting)
            {
                DebugLog("Sprint START\n");
                CreateThread(0, 0, TirednessEffectStart, 0, 0, 0);
            }

            if (!isSprinting && wasSprinting)
            {
                DebugLog("Sprint END\n");
                CreateThread(0, 0, TirednessEffectStop, 0, 0, 0);
            }
        }

        if (isSprinting && !wasSprinting)
        {
            OynonSetMovementFrictionMultiplier(g_speed);
            OynonSetMovementJumpHeightMultiplier(g_jump_height);
            OynonSetMovementLandingGravity(g_landing_gravity);
        }
        else if (!isSprinting && wasSprinting)
        {
            OynonSetMovementFrictionMultiplier(1.0f);
            OynonSetMovementJumpHeightMultiplier(1.0f);
            OynonSetMovementLandingGravity(-2500);
        }

        wasSprinting = isSprinting;

        Sleep(1);
    }

    return 0;
}
