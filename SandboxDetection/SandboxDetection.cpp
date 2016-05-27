#include "stdafx.h"
#include <windows.h>
#include "ProcessUtils.h"
#include <stdio.h>
#include <iostream>

int main(void)
{
	ProcessUtils procUtil = ProcessUtils(GetCurrentProcessId());

	char *avastSignatures[] = { "A.v.a.s.t", "s.n.x.h.k" };
	bool avastSandbox = procUtil.SearchForSignatureInMemory(avastSignatures);

	bool comodoSandbox = procUtil.SearchForModuleInProcess(L"cmdvrt32.dll");
	bool qihoo360Sandbox = procUtil.SearchForModuleInProcess(L"SxIn.dll");
	bool sandboxieSandbox = procUtil.SearchForModuleInProcess(L"SbieDll.dll");

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

	getchar();
	return 0;
}