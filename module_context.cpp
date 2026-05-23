#include "module_context.h"

#include <cstdio>
#include <cstring>

namespace
{
char g_iniPath[MAX_PATH] = {};
char g_logPath[MAX_PATH] = {};

bool TrimLastPathComponent(char* path)
{
    char* slash = std::strrchr(path, '\\');
    if (!slash) {
        return false;
    }

    *slash = '\0';
    return true;
}
}

void InitializeModuleContext(HMODULE module)
{
    char modulePath[MAX_PATH] = {};
    if (!::GetModuleFileNameA(module, modulePath, MAX_PATH)) {
        return;
    }

    if (!TrimLastPathComponent(modulePath)) {
        return;
    }

    std::snprintf(g_iniPath, sizeof(g_iniPath), "%s\\PPMM.ini", modulePath);
    std::snprintf(g_logPath, sizeof(g_logPath), "%s\\PPMM.log", modulePath);
}

const char* GetIniPath()
{
    return g_iniPath;
}

const char* GetLogPath()
{
    return g_logPath;
}
