#include "pch.h"
#include "config.h"
#include <WinBase.h>
#include <cstdlib>
#include "debug.h"

float g_speed            = 1.0f;
bool  g_debug            = false;
float g_jump_height      = 1.0f;
int   g_landing_gravity  = -2500;

bool  g_tiredness_effect = true;
int   g_tiredness_delta = 30;

void LoadConfig()
{
    // DEBUG--------------------------------------------------

    g_debug = GetPrivateProfileIntA(
        "General", "Debug", 1, ".\\PPMM.ini");

    TryOpenDebugConsole();
    DebugLog("Debug= ", g_debug, "\n");

    // SPEED -------------------------------------------------

    char charBuffer[32]{};
    GetPrivateProfileStringA(
        "Constants", "MoveSpeedMultiplier", "1",
        charBuffer, sizeof(charBuffer), ".\\PPMM.ini");

    g_speed = (float)atof(charBuffer);

    DebugLog("MoveSpeedMultiplierDt1= ", g_speed, "\n");

    // JUMP ---------------------------------------------------

    GetPrivateProfileStringA(
        "Constants", "JumpHeight", "1",
        charBuffer, sizeof(charBuffer), ".\\PPMM.ini");

    g_jump_height = (float)atof(charBuffer);

    DebugLog("JumpHeight= ", g_jump_height, "\n");

    // GRAVITY -------------------------------------------------

    g_landing_gravity = GetPrivateProfileIntA(
        "Constants", "JumpLandingGravity", -2500, ".\\PPMM.ini");

    DebugLog("JumpLandingGravity= ", g_landing_gravity, "\n");

    // TIREDNESS -------------------------------------------------

    g_tiredness_effect = GetPrivateProfileIntA(
        "General", "TirednessEffect", 1, ".\\PPMM.ini");

    DebugLog("TirednessEffect= ", g_tiredness_effect, "\n");

    // TIREDNESS DELTA -------------------------------------------

    g_tiredness_delta = GetPrivateProfileIntA(
        "Constants", "TirednessDelta", 30, ".\\PPMM.ini");

    DebugLog("TirednessDelta= ", g_tiredness_delta, "\n");
}
