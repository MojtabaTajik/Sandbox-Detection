#include "stdafx.h"
#include "ProcessUtils.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <psapi.h>
#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

bool ProcessUtils::SearchForModuleInProcess(HANDLE processHandle, TCHAR moduleToFind[MAX_PATH])
{
	HMODULE moduleHandle[1024];
	DWORD cbNeeded;
	unsigned int i;

	if (! processHandle) return false;
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

bool ProcessUtils::SearchForSignatureInProcessMemory(HANDLE processHandle, char* signature[])
{
	if (!processHandle) return false;

		SYSTEM_INFO sys_info;
		GetSystemInfo(&sys_info);

		LPVOID proc_min_address = sys_info.lpMinimumApplicationAddress;
		LPVOID proc_max_address = sys_info.lpMaximumApplicationAddress;

		DWORD proc_min_address_d = (DWORD)proc_min_address;
		DWORD proc_max_address_d = (DWORD)proc_max_address;

		MEMORY_BASIC_INFORMATION memInfo;

		unsigned char *p = NULL;
		for (p = NULL;
			VirtualQueryEx(processHandle, p, &memInfo, sizeof(memInfo)) == sizeof(memInfo);
			p += memInfo.RegionSize)
		{
			std::vector<char> buffer;
			std::vector<char>::iterator pos;

			if (memInfo.State == MEM_COMMIT)// &&
				//(memInfo.Type == MEM_MAPPED || memInfo.Type == MEM_PRIVATE || memInfo.Type == LMEM_FIXED))
			{
				SIZE_T bytes_read;
				buffer.resize(memInfo.RegionSize);
				ReadProcessMemory(processHandle, p, &buffer[0], memInfo.RegionSize, &bytes_read);
				buffer.resize(bytes_read);
				if (bytes_read <= 0)
					continue;

				bool found = true;

				int arrayLen = (sizeof signature / sizeof signature[0]);
				for (int signCounter = 0; signCounter <= arrayLen; signCounter++)
				{
					for (int bufIndex = 0; bufIndex < buffer.size(); bufIndex++)
					{
						found = true;

						// Search for first character of signature in buffer
						if (buffer[bufIndex] != signature[signCounter][0] || buffer[bufIndex + 1] != signature[signCounter][2])
							continue;

						int tempBuffIndex = bufIndex;
						for (int signChecker = 0; signChecker < strlen(signature[signCounter]); signChecker++)
						{
							if (signature[signCounter][signChecker] == '.')
								continue;

							if (buffer[(tempBuffIndex)] != signature[signCounter][signChecker])
							{
								found = false;
								break;
							}

							tempBuffIndex++;
						}

						if (found)
							return true;
					}
				}
			}
		}
}