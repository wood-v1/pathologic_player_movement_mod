#pragma once

#include <sstream>

void TryOpenDebugConsole();
void DebugLogText(const char* text);

template<typename... Args>
void DebugLog(Args... args) {
    std::ostringstream stream;
    stream << std::boolalpha;
    (stream << ... << args);
    DebugLogText(stream.str().c_str());
}
