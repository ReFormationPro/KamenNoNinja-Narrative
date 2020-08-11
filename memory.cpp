#include <windows.h>
#include <iostream>
#include "memory.h"

#define WINDOW_NAME "FCEUX 2.2.3: Kamen No Ninja Original"

using namespace std;

LPCVOID BASE = (LPCVOID) 0x007B1388;	//fceux.exe+3B1388
const int PL_HP_OFF = 0x86;
const int EN_HP_OFF = 0x8B;
const int EN_HP1_OFF = 0x34D;
const int EN_HP2_OFF = 0xB2;
const int TANGLED_HAIR_OFF = 0x3A5;
const int MOB_TYPE_OFF = 0x373;	//The eagle, the toad, the snakes are not kinds of mob.
const int ENEMY_FLAGS_BASE_OFF = 0x3C6;

HANDLE hProcess;

LPVOID baseAddr;

LPCVOID PLAYER_HP_ADDR;
LPCVOID EN_HP_ADDR;
LPCVOID EN_HP1_ADDR;
LPCVOID EN_HP2_ADDR;
LPCVOID TANGLED_HAIR_ADDR;
LPCVOID MOB_TYPE_ADDR;

void load_addresses() {
	ReadProcessMemory(hProcess, (LPCVOID) BASE, (LPVOID) &baseAddr, 4, NULL);
	PLAYER_HP_ADDR = (LPCVOID) (baseAddr+PL_HP_OFF);
	EN_HP_ADDR = (LPCVOID) (baseAddr+EN_HP_OFF);
	EN_HP1_ADDR = (LPCVOID) (baseAddr+EN_HP1_OFF);
	EN_HP2_ADDR = (LPCVOID) (baseAddr+EN_HP2_OFF);
	TANGLED_HAIR_ADDR = (LPCVOID) (baseAddr+TANGLED_HAIR_OFF);
	MOB_TYPE_ADDR = (LPCVOID) (baseAddr+MOB_TYPE_OFF);
}
int get_enemy_state(enum MOB_TYPE type) {
	return read_byte((LPCVOID) (baseAddr + (int)ENEMY_FLAGS_BASE_OFF + (int)type));
}
void set_enemy_state(enum MOB_TYPE type, int val) {
	write_byte((LPCVOID) (baseAddr + (int)ENEMY_FLAGS_BASE_OFF + (int)type), val);
}
bool open_process() {
	int dwProcessId;	
    HWND GameWindowHandle = FindWindow(NULL, WINDOW_NAME);
    GetWindowThreadProcessId(GameWindowHandle, (LPDWORD) &dwProcessId);
    std::cout << "WindowThreadProcessId: " << dwProcessId << std::endl;
 	hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, false, dwProcessId);
 	return hProcess != NULL;
}
bool close_process() {
	int ret = CloseHandle(hProcess);
	hProcess = NULL;
	return ret != 0;
}
int read_byte(LPCVOID addr) {
	int ret;
	ReadProcessMemory(hProcess, addr, (LPVOID) &ret, 1, NULL);
	return ret;
}
bool write_byte(LPCVOID addr, int val) {
	int ret;
	LPVOID adr = (LPVOID) addr;
	WriteProcessMemory(hProcess, adr, (LPCVOID)&val, 1, NULL);
	return ret;
}

void debug() {
	cout << "PLAYER_HP_ADDR " << PLAYER_HP_ADDR << endl;
	cout << "PLAYER HP " << read_byte(PLAYER_HP_ADDR) << endl;
	cout << "BALDIE STATE " << get_enemy_state(BALDIE) << endl;
	write_byte(PLAYER_HP_ADDR, 56);
}

/* This needs to be injected into the application because we cannot add vectored exception handler to another process */
LPVOID _addr;
DWORD dwProtection;
void test_data_breakpoint() {
	set_on_access_breakpoint((LPVOID) (baseAddr + (int)ENEMY_FLAGS_BASE_OFF + (int)BALDIE));
	cout << dwProtection << " " << _addr << endl;
}
LONG WINAPI hExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo)
{
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == STATUS_ACCESS_VIOLATION)
    {
        /*DWORD dwESI = ExceptionInfo->ContextRecord->Esi;
        DWORD dwSize = *(DWORD*)(dwESI + 0x14);
        PVOID pVAC3 = *(PVOID*)(dwESI + 0x18);*/
        VirtualProtectEx(hProcess, (PVOID)_addr, 0x1, dwProtection, NULL);
        cout << "Exception Handled Successfully!";
    }
    cout << "What did you do?" << endl;
    return EXCEPTION_CONTINUE_SEARCH;
}
void set_on_access_breakpoint(LPVOID addr) {
	_addr = addr;
    VirtualProtectEx(hProcess, (PVOID)addr, 0x1, PAGE_EXECUTE, &dwProtection);
    //*(BYTE*)(addr) = 0xCC;
    //VirtualProtect((PVOID)addr, 0x1, dwProtection, NULL);
    AddVectoredExceptionHandler(1, hExceptionFilter);
}

