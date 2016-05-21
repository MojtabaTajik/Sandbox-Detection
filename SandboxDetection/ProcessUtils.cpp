#include "stdafx.h"
#include "ProcessUtils.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <psapi.h>
#include <iostream>

HANDLE processHandle;

ProcessUtils::ProcessUtils(DWORD processID)
{
	processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
	if (processHandle == nullptr)
		throw std::invalid_argument("Fill process id");
}

ProcessUtils::~ProcessUtils()
{
	CloseHandle(processHandle);
}

bool ProcessUtils::SearchForModuleInProcess(TCHAR moduleToFind[MAX_PATH])
{
	HMODULE moduleHandle[1024];
	DWORD cbNeeded;
	unsigned int i;

	if (EnumProcessModules(processHandle, moduleHandle, sizeof(moduleHandle), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR moduleName[MAX_PATH];

			if (GetModuleFileNameEx(processHandle, moduleHandle[i], moduleName, sizeof(moduleName) / sizeof(TCHAR))) {
				if (wcsstr(moduleName, moduleToFind) != nullptr)
					return true;
			}
		}
	}

	return false;;
}