#include "pch.h"

void
stealth_console_mode()
{
	HWND stealth_hwnd;
	AllocConsole();
	stealth_hwnd = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(stealth_hwnd,0);
}

void
enable_debug_privilege()
{
	HANDLE hToken;
    LUID luid;
    TOKEN_PRIVILEGES tkp;

    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

    LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = luid;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL);

    CloseHandle(hToken); 
}

void
process_scanner(char* procs[], int proc_count)
{
			
	PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
			for(uint i = 0; i < proc_count; i++)
			{
				if(StrStrA(entry.szExeFile, procs[i]) != NULL)
				{
					HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
	                CloseHandle(hProcess);
					TerminateProcess(hProcess, 1);
				}
			}
        }
    }

    CloseHandle(snapshot);
}

int
main()
{
	
	const char* procs[5] = {
		"YourPhone",
		"Adobe",
		"Microsoft Edge Update",
		"Microsoft Office Click-To-Run",
		"Notification Manager For Adobe Creative Cloud",
	};

	const int proc_count = sizeof(procs) / sizeof(procs[0]);
	
	stealth_console_mode();
    enable_debug_privilege();
	
	while(1)
	{
		process_scanner(procs, proc_count);
		Sleep(15000);
	}
}