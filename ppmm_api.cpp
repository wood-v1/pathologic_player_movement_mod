#include "framework.h"
#include "PPMMApi.h"
#include "ppmm_api_internal.h"

#include <atomic>

namespace
{
std::atomic<bool> g_externalSprintBlocked{ false };
std::atomic<bool> g_sprintActive{ false };
std::atomic<bool> g_externalTirednessSuppressed{ false };

BOOL __stdcall SetExternalSprintBlocked(BOOL blocked)
{
    g_externalSprintBlocked.store(blocked != FALSE);
    return TRUE;
}

BOOL __stdcall IsSprintActive()
{
    return g_sprintActive.load() ? TRUE : FALSE;
}

BOOL __stdcall SetExternalTirednessSuppressed(BOOL suppressed)
{
    g_externalTirednessSuppressed.store(suppressed != FALSE);
    return TRUE;
}

const PpmmApiV1 g_api{
    sizeof(PpmmApiV1),
    PPMM_API_VERSION_1,
    &SetExternalSprintBlocked,
    &IsSprintActive,
    &SetExternalTirednessSuppressed
};
}

const PpmmApiV1* __stdcall PpmmGetApi(DWORD requestedVersion)
{
    return requestedVersion == PPMM_API_VERSION_1 ? &g_api : nullptr;
}

bool PpmmIsExternalSprintBlocked()
{
    return g_externalSprintBlocked.load();
}

bool PpmmIsExternalTirednessSuppressed()
{
    return g_externalTirednessSuppressed.load();
}

void PpmmSetSprintActive(bool active)
{
    g_sprintActive.store(active);
}
