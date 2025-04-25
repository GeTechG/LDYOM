#ifndef PROCESS_UTILS_H
#define PROCESS_UTILS_H

#include <windows.h>

DWORD GetProcessIdFromModule(HMODULE hModule);
bool HasGameLaunched(HMODULE hModule, int maxRetries, int sleepDuration);
bool CheckAndPromptSilentPatch();

#endif // PROCESS_UTILS_H
