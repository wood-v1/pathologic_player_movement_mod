#include "pch.h"
#include "debug.h"
#include <consoleapi.h>
#include <consoleapi2.h>
#include "config.h"

void TryOpenDebugConsole()
{
    if (!g_debug) {
        return;
    }

    if (!AllocConsole()) {
        return;
    }

    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);
    freopen_s(&f, "CONIN$", "r", stdin);

    SetConsoleTitleA("PPMM Debug Console");
}