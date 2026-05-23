#include "framework.h"
#include "config.h"
#include "debug.h"
#include "module_context.h"

#include <cstdlib>

float g_speed = 1.0f;
float g_jump_height = 1.0f;
int g_landing_gravity = -2500;

bool g_tiredness_effect = true;
int g_tiredness_delta = 30;

void LoadConfig()
{
    TryOpenDebugConsole();

    char charBuffer[32]{};
    GetPrivateProfileStringA(
        "Constants", "MoveSpeedMultiplier", "1",
        charBuffer, sizeof(charBuffer), GetIniPath());

    g_speed = (float)atof(charBuffer);
    DebugLog("MoveSpeedMultiplierDt1= ", g_speed, "\n");

    GetPrivateProfileStringA(
        "Constants", "JumpHeight", "1",
        charBuffer, sizeof(charBuffer), GetIniPath());

    g_jump_height = (float)atof(charBuffer);
    DebugLog("JumpHeight= ", g_jump_height, "\n");

    g_landing_gravity = GetPrivateProfileIntA(
        "Constants", "JumpLandingGravity", -2500, GetIniPath());
    DebugLog("JumpLandingGravity= ", g_landing_gravity, "\n");

    g_tiredness_effect = GetPrivateProfileIntA(
        "General", "TirednessEffect", 1, GetIniPath());
    DebugLog("TirednessEffect= ", g_tiredness_effect, "\n");

    g_tiredness_delta = GetPrivateProfileIntA(
        "Constants", "TirednessDelta", 30, GetIniPath());
    DebugLog("TirednessDelta= ", g_tiredness_delta, "\n");
}
