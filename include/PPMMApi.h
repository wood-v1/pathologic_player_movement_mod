#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

constexpr DWORD PPMM_API_VERSION_1 = 1;

struct PpmmApiV1
{
    DWORD size;
    DWORD version;
    BOOL (__stdcall* SetExternalSprintBlocked)(BOOL blocked);
    BOOL (__stdcall* IsSprintActive)();
    BOOL (__stdcall* SetExternalTirednessSuppressed)(BOOL suppressed);
};

#ifdef PPMM_EXPORTS
#define PPMM_API extern "C" __declspec(dllexport)
#else
#define PPMM_API extern "C" __declspec(dllimport)
#endif

PPMM_API const PpmmApiV1* __stdcall PpmmGetApi(DWORD requestedVersion);
