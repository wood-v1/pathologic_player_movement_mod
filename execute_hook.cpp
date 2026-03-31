#include "pch.h"
#include <Windows.h>
#include "debug.h"

void* g_pEngine = nullptr;

DWORD ExecuteAddr;
DWORD ExecuteRet;
BYTE* ExecuteCave;

typedef void(__thiscall* Execute_t)(void* pEngine, const char* cmd);
Execute_t Execute = nullptr;

void BuildExecuteCave()
{
    ExecuteCave = (BYTE*)VirtualAlloc(nullptr, 128, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    int i = 0;

    // mov [g_pEngine], ecx
    ExecuteCave[i++] = 0x89;
    ExecuteCave[i++] = 0x0D;
    *(DWORD*)(ExecuteCave + i) = (DWORD)&g_pEngine;
    i += 4;

    // original bytes
    ExecuteCave[i++] = 0x8B;
    ExecuteCave[i++] = 0xD9;

    ExecuteCave[i++] = 0x8A;
    ExecuteCave[i++] = 0x06;

    ExecuteCave[i++] = 0x3C;
    ExecuteCave[i++] = 0x20;

    // jmp back
    ExecuteCave[i++] = 0xE9;
    *(DWORD*)(ExecuteCave + i) = ExecuteRet - (DWORD)(ExecuteCave + i) - 4;
}

void SetupExecuteHook(DWORD base)
{
    ExecuteAddr = base + 0x3CA7E;
    ExecuteRet = ExecuteAddr + 6;
}

void HookExecute()
{
    DWORD oldProtect;
    VirtualProtect((LPVOID)ExecuteAddr, 6, PAGE_EXECUTE_READWRITE, &oldProtect);

    for (int i = 0; i < 6; i++) {
        *(BYTE*)(ExecuteAddr + i) = 0x90;
    }

    *(BYTE*)ExecuteAddr = 0xE9;
    *(DWORD*)(ExecuteAddr + 1) = (DWORD)ExecuteCave - ExecuteAddr - 5;

    VirtualProtect((LPVOID)ExecuteAddr, 6, oldProtect, &oldProtect);
}

void InitExecuteHook(DWORD engineBase)
{
    DebugLog("Installing execute hook...\n");

    Execute = (Execute_t)(engineBase + 0x3CA60);

    SetupExecuteHook(engineBase);
    BuildExecuteCave();
    HookExecute();

    DebugLog("Execute hook installed\n");
}

void ExecCommand(const char* cmd)
{
    if (g_pEngine && Execute) {
        Execute(g_pEngine, cmd);
    }
}