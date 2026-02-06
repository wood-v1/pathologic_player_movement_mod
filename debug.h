#pragma once

#include <iostream>
#include <string>
#include <type_traits>
#include <iomanip>
#include "config.h"

void TryOpenDebugConsole();

template<typename T>
void DebugLog(T value) {
    if (g_debug) {
        if (std::is_same<T, bool>::value) {
            std::cout << std::boolalpha << value;
        }
        else {
            std::cout << value;
        }
    }
}

template<typename T, typename... Args>
void DebugLog(T first, Args... args) {
    DebugLog(first);
    DebugLog(args...);
}

template<typename... Args>
void DebugLogLn(Args... args) {
    DebugLog(args...);
    if (g_debug) std::cout;
}