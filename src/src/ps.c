//    __ _/| _/. _  ._/__ /
// _\/_// /_///_// / /_|/
//            _/
// sof digital 2021
// written by michael rinderle <michael@sofdigital.net>

// mit license
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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