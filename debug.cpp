#include "framework.h"
#include "debug.h"
#include "OynonToolsApi.h"

namespace
{
constexpr const char* PPMM_DEBUG_CHANNEL = "PPMM";
}

void TryOpenDebugConsole()
{
    OynonDebugConfigureLauncherChannel(PPMM_DEBUG_CHANNEL, FALSE);
    OynonDebugOpenConsole();
}

void DebugLogText(const char* text)
{
    OynonDebugLog(PPMM_DEBUG_CHANNEL, text);
}
