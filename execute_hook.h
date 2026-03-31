#pragma once
#include <cstdint>

void InitExecuteHook(DWORD engineBase);

void ExecCommand(const char* cmd);