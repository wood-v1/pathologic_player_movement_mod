#include "framework.h"
#include "tiredness_effect.h"
#include "config.h"
#include "debug.h"
#include "ppmm_api_internal.h"
#include "OynonToolsApi.h"

#include <string>

namespace
{
bool g_tirednessActive = false;

void StartTirednessEffect()
{
    const std::string deltaCommand =
        "setvar ppmm_tiredness_delta " + std::to_string(g_tiredness_delta);
    OynonExecCommand(deltaCommand.c_str());
    OynonExecCommand("effect player ppmm_stats_effect.bin");
    DebugLog("Tiredness effect applied\n");
}

void StopTirednessEffect()
{
    OynonExecCommand("trigger player ppmm_stats_effect_stop");
    DebugLog("Tiredness effect removed\n");
}
}

void TirednessEffectInitialize()
{
    g_tirednessActive = false;
}

void TirednessEffectUpdate(bool sprintRequested)
{
    const bool shouldRun =
        g_tiredness_effect &&
        !PpmmIsExternalTirednessSuppressed() &&
        sprintRequested;

    if (shouldRun && !g_tirednessActive) {
        StartTirednessEffect();
        g_tirednessActive = true;
    }
    else if (!shouldRun && g_tirednessActive) {
        StopTirednessEffect();
        g_tirednessActive = false;
    }
}
