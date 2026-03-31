#include "pch.h"
#include "hook.h"
#include <Windows.h>
#include "config.h"
#include <cstdio>
#include "debug.h"
#include <sstream>
#include <iostream>
#include <string>
#include "execute_hook.h"

DWORD hookDt1;
DWORD hookDt2;
DWORD hookDt1Ret;
DWORD hookDt2Ret;
DWORD hookJump;
DWORD hookJumpRet;

BYTE* caveDt1;
BYTE* caveDt2;
BYTE* caveJump;

float* speedCoef;
int* landingGravityValue;
float* jumpHeight;

DWORD WINAPI TirednessEffectStart(LPVOID lpParam) {
    std::string cmd = "setvar ppmm_tiredness_delta " + std::to_string(g_tiredness_delta);
    ExecCommand(cmd.c_str());
    Sleep(100);
    ExecCommand("effect player ppmm_stats_effect.bin");

    DebugLog("Sprint effect applied\n");
    return 0;
}

DWORD WINAPI TirednessEffectStop(LPVOID lpParam) {
    ExecCommand("trigger player ppmm_stats_effect_stop");

    DebugLog("Sprint effect removed\n");
    return 0;
}

DWORD WINAPI MainThread(LPVOID)
{
    LoadConfig();

    DebugLog("PPMM.dll injected successfully!\n");
    DebugLog("Config loaded\n");

    while (!GetModuleHandleA("Engine.dll")) {
        Sleep(100);
    }

    Sleep(1000);

    DebugLog("Engine.dll initialized\n");

    InitExecuteHook(GetEngineBase());

    DebugLog("Execute hook installed\n");

    InstallMovementHook(GetEngineBase());

    bool toggleSpeed = false;
    bool prevCapsState = false;

    bool isSprinting = false;
    bool wasSprinting = false;

    while (true)
    {
        // Detect Caps Lock toggle
        bool capsPressedNow = (GetAsyncKeyState(VK_CAPITAL) & 0x0001) != 0;
        if (capsPressedNow && !prevCapsState)
        {
            toggleSpeed = !toggleSpeed;
            DebugLog(toggleSpeed ? "Speed toggled ON\n" : "Speed toggled OFF\n");
        }
        prevCapsState = capsPressedNow;

        // Detect current Sprint state
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

        if (isSprinting)
        {
            *speedCoef = g_speed;
            *landingGravityValue = g_landing_gravity;
            *jumpHeight = g_jump_height;
        }
        else
        {
            *speedCoef = 1.0f;
            *landingGravityValue = -2500;
            *jumpHeight = 1.0f;
        }

        wasSprinting = isSprinting;

        Sleep(1);
    }

    return 0;
}

DWORD GetEngineBase()
{
    return (DWORD)GetModuleHandleA("Engine.dll");
}

void InstallMovementHook(DWORD base) 
{
    DebugLog("Installing movement hook...\n");

    ResolveAddresses(base);
    DebugLog("Base address resolved successfully\n");

    AllocateMemory();
    DebugLog("Memory allocated successfully\n");

    // Build custom assembly code (code caves)
    BuildCaveDt1();
    DebugLog("Cave built for DT1 successfully\n");

    BuildCaveDt2();
    DebugLog("Cave built for DT2 successfully\n");

    BuildCaveJump();
    DebugLog("Cave built for Jump successfully\n");

    // Install hooks (overwrite original instructions with JMP)
    HookDt(hookDt1, caveDt1);
    HookDt(hookDt2, caveDt2);

    DebugLog("Dt Hooks installed successfully\n");

    HookJump(hookJump, caveJump);

    DebugLog("Jump Hook installed successfully\n");

    // Patch gravity constant directly
    PatchLandingGravity();
    DebugLog("Landing Gravity patched successfully\n");
}

void PatchLandingGravity()
{
    DWORD base = GetEngineBase();
    BYTE* instr = (BYTE*)(base + 0x2047DF);

    DWORD oldProtect;
    VirtualProtect(instr + 1, 4, PAGE_EXECUTE_READWRITE, &oldProtect);

    *(float*)(instr + 1) = g_landing_gravity;

    VirtualProtect(instr + 1, 4, oldProtect, &oldProtect);
}

void ResolveAddresses(DWORD base)
{
    hookDt1 = base + 0x103B70;
    hookDt2 = base + 0x103B96;
    hookJump = base + 0x1042EE;

    hookDt1Ret = base + 0x103B76;
    hookDt2Ret = base + 0x103B9C;
    hookJumpRet = base + 0x1042F6;
}

void AllocateMemory()
{
    caveDt1 = (BYTE*)VirtualAlloc(
        nullptr, 256,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE);

    caveDt2 = (BYTE*)VirtualAlloc(
        nullptr, 256,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE);

    caveJump = (BYTE*)VirtualAlloc(
        nullptr,
        256,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE
    );

    speedCoef = (float*)VirtualAlloc(
        nullptr, sizeof(float),
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE);

    landingGravityValue = (int*)VirtualAlloc(
        nullptr, sizeof(int),
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE
    );

    jumpHeight = (float*)VirtualAlloc(
        nullptr, sizeof(float),
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE
    );
    
    *landingGravityValue = g_landing_gravity;
    *speedCoef = g_speed;
    *jumpHeight = g_jump_height;
}

void BuildCaveDt1()
{
    BYTE code[] = {
        0xF3,0x0F,0x10,0x44,0x24,0x50,      // movss xmm0,[esp+50]
        0xF3,0x0F,0x59,0x05,0,0,0,0,        // mulss xmm0,[speedcoef1]
        0xF3,0x0F,0x11,0x44,0x24,0x50,      // movss [esp+50],xmm0
        0xE9,0,0,0,0                        // jmp ret
    };

    // Inject pointer to variable
    *(DWORD*)(code + 10) = (DWORD)speedCoef;

    // Calculate relative jump back
    DWORD rel32 = hookDt1Ret - ((DWORD)caveDt1 + sizeof(code));
    *(DWORD*)(code + 21) = rel32;

    memcpy(caveDt1, code, sizeof(code));
}

void BuildCaveDt2()
{
    BYTE code[] = {
        0xF3,0x0F,0x10,0x44,0x24,0x58,      // movss xmm0,[esp+58]
        0xF3,0x0F,0x59,0x05,0,0,0,0,        // mulss xmm0,[speedcoef2]
        0xF3,0x0F,0x11,0x44,0x24,0x58,      // movss [esp+58],xmm0
        0xE9,0,0,0,0                        // jmp ret
    };

    // Inject pointer to variable
    *(DWORD*)(code + 10) = (DWORD)speedCoef;

    // Calculate relative jump back
    DWORD rel32 = hookDt2Ret - ((DWORD)caveDt2 + sizeof(code));
    *(DWORD*)(code + 21) = rel32;

    memcpy(caveDt2, code, sizeof(code));
}

void HookDt(DWORD addr, BYTE* cave)
{
    DWORD old;
    VirtualProtect((LPVOID)addr, 6, PAGE_EXECUTE_READWRITE, &old);

    *(BYTE*)addr = 0xE9;
    *(DWORD*)(addr + 1) = (DWORD)cave - addr - 5;
    *(BYTE*)(addr + 5) = 0x90;

    VirtualProtect((LPVOID)addr, 6, old, &old);
}

void BuildCaveJump()
{
    BYTE code[] = {
        0xF3,0x0F,0x10,0x1D, 0,0,0,0,       // movss xmm3,[orig]
        0xF3,0x0F,0x59,0x1D, 0,0,0,0,       // mulss xmm3,[jumpMul]
        0xE9,0,0,0,0                        // jmp ret
    };

    *(DWORD*)(code + 4) = GetEngineBase() + 0x1FC368;
    *(DWORD*)(code + 12) = (DWORD)jumpHeight;

    DWORD rel = hookJumpRet - ((DWORD)caveJump + sizeof(code));
    *(DWORD*)(code + 17) = rel;

    memcpy(caveJump, code, sizeof(code));
}

void HookJump(DWORD addr, BYTE* cave)
{
    DWORD old;
    VirtualProtect((LPVOID)addr, 8, PAGE_EXECUTE_READWRITE, &old);

    *(BYTE*)addr = 0xE9;
    *(DWORD*)(addr + 1) = (DWORD)cave - addr - 5;

    *(BYTE*)(addr + 5) = 0x90;
    *(BYTE*)(addr + 6) = 0x90;
    *(BYTE*)(addr + 7) = 0x90;

    VirtualProtect((LPVOID)addr, 8, old, &old);
}
