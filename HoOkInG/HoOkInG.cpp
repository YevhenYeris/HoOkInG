#include <Windows.h>
#include <time.h>
#include<iostream>
#include <fstream>

using namespace std;

const char* path = "D:\\Shkola\\3rdCourse\\log.txt";

HHOOK hook;

MSLLHOOKSTRUCT* msLlHookStruct;

ofstream file;

int Record(WPARAM wParam)
{
	if (msLlHookStruct != NULL)
	{		
		printf_s("Mouse Coordinates: x = %i | y = %i \n", msLlHookStruct->pt.x, msLlHookStruct->pt.y);
	}
	else
	{
		return 0;
	}

	switch (wParam)
	{
		case WM_LBUTTONUP:
		{
			file << "LEFT CLICK UP AT (" << msLlHookStruct->pt.x << ", " << msLlHookStruct->pt.y << ")\n";
		}break;
		case WM_LBUTTONDOWN:
		{
			file << "LEFT CLICK DOWN AT (" << msLlHookStruct->pt.x << ", " << msLlHookStruct->pt.y << ")\n";
		}break;
	}

	file.flush();
	return 0;
}

LRESULT __stdcall mouseCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		msLlHookStruct = (MSLLHOOKSTRUCT*)lParam;
		Record(wParam);
	}
	return CallNextHookEx(hook, nCode, wParam, lParam);
}

int main()
{
	file.open(path);

	int pid = 91316;
	DWORD access = PROCESS_VM_READ |
		PROCESS_QUERY_INFORMATION |
		PROCESS_VM_WRITE |
		PROCESS_VM_OPERATION;
	HANDLE proc = OpenProcess(access, FALSE, pid);

	int addr = 0x007cf728;
	SIZE_T written;
	int value;
	ReadProcessMemory(proc, (LPCVOID)addr, &value, sizeof(value), &written);

	CloseHandle(proc);

	file << "MEMORY FROM PROCESS " << pid << ": " << addr << " - " << value << "\n";

	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 1);
	if (!(hook = SetWindowsHookEx(WH_MOUSE_LL, mouseCallback, NULL, 0))) {
		printf_s("Error: %x \n", GetLastError());
	}
	MSG message;
	while (true)
	{
		GetMessage(&message, NULL, 0, 0);
	}
}