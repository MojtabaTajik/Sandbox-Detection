#pragma once
#include <windows.h>

class ProcessUtils
{
public:
	bool SearchForModuleInProcess(HANDLE processHandle, TCHAR moduleToFind[MAX_PATH]);
	bool SearchForSignatureInProcessMemory(HANDLE processHandle, char* signature[]);
};