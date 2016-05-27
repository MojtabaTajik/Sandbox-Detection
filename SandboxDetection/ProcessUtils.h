#pragma once
#include <windows.h>

class ProcessUtils
{
public:
	ProcessUtils(DWORD processID);
	~ProcessUtils();

	bool SearchForModuleInProcess(TCHAR moduleToFind[MAX_PATH]);
	bool ProcessUtils::SearchForSignatureInMemory(char* signature[]);
};