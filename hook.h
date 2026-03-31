#pragma once


#include <sstream>
#include <iostream>
#include <string>


DWORD WINAPI MainThread(LPVOID);

void TryOpenDebugConsole();

DWORD GetEngineBase();

void InstallMovementHook(DWORD base);

void ResolveAddresses(DWORD base);

void AllocateMemory();

void BuildCaveDt1();

void BuildCaveDt2();

void PatchLandingGravity();

void BuildCaveJump();

void HookDt(DWORD addr, BYTE* cave);

void HookJump(DWORD addr, BYTE* cave);