#include "stdafx.h"
#include <windows.h>
#include "ProcessUtils.h"
#include <tlhelp32.h>
#include <stdio.h>
#include <iostream>

int main(void)
{
	DWORD processPID = GetCurrentProcessId();

	// Get parent process handle
	DWORD parentProcessPid = getParentPID(processPID);
	HANDLE parentProcessHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, parentProcessPid);

	// Get self handle
	HANDLE selfProcessHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processPID);

	ProcessUtils procUtil = ProcessUtils();

	__try
	{
		char *avastSignatures[] = { "A.v.a.s.t", "s.n.x.h.k" };
		bool avastSandbox = procUtil.SearchForSignatureInProcessMemory(avastSignatures);

		bool comodoSandbox = procUtil.SearchForModuleInProcess(L"cmdvrt32.dll");
		bool qihoo360Sandbox = procUtil.SearchForModuleInProcess(L"SxIn.dll");
		bool sandboxieSandbox = procUtil.SearchForModuleInProcess(L"SbieDll.dll");
		bool shadeSandbox = procUtil.SearchFormModuleInProcessAndParentProcess(L"");

		if (comodoSandbox)
			_tprintf(TEXT("Comodo sandbox detected"));
		else if (qihoo360Sandbox)
			_tprintf(TEXT("Qihoo360 sandbox detected"));
		else if (sandboxieSandbox)
			_tprintf(TEXT("Sandboxie sandbox detected"));
		else if (avastSandbox)
			_tprintf(TEXT("Avast sandbox detected"));
		else
			_tprintf(TEXT("I'm running normally ;)"));
	}
	__finally
	{
		CloseHandle(parentProcessHandle);
		CloseHandle(selfProcessHandle);
	}

	getchar();
	return 0;
}

DWORD getParentPID(DWORD pid)
{
	HANDLE h = NULL;
	PROCESSENTRY32 pe = { 0 };
	DWORD ppid = 0;
	pe.dwSize = sizeof(PROCESSENTRY32);
	h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (Process32First(h, &pe))
	{
		do
		{
			if (pe.th32ProcessID == pid)
			{
				ppid = pe.th32ParentProcessID;
				break;
			}
		} while (Process32Next(h, &pe));
	}
	CloseHandle(h);
	return (ppid);
}