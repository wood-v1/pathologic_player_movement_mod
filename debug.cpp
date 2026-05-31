#include "framework.h"
#include "debug.h"
#include "config.h"
#include "module_context.h"
#include "OynonToolsApi.h"

namespace
{
constexpr const char* PPMM_DEBUG_CHANNEL = "PPMM";
}

void TryOpenDebugConsole()
{
    OynonDebugConfigureChannel(
        PPMM_DEBUG_CHANNEL,
        g_debug ? TRUE : FALSE,
        g_debug ? GetLogPath() : "",
        "");
    OynonDebugOpenConsole();
}

void DebugLogText(const char* text)
{
    OynonDebugLog(PPMM_DEBUG_CHANNEL, text);
}
