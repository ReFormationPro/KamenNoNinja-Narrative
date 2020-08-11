#include <windows.h>
#include <iostream>
#define WINDOW_NAME "FCEUX 2.2.3: Kamen No Ninja Original"

using namespace std;

LPCVOID BASE = (LPCVOID) 0x007B1388;
const int PL_HP_OFF = 0x86;
const int EN_HP_OFF = 0x8B;
const int EN_HP1_OFF = 0x34D;
const int EN_HP2_OFF = 0xB2;
const int TANGLED_HAIR_OFF = 0x3A5;

HANDLE hProcess;

int baseAddr;
// They are not addresses, they are values

int PLAYER_HP_ADDR;
int EN_HP_ADDR;
int EN_HP1_ADDR;
int EN_HP2_ADDR;
int TANGLED_HAIR_ADDR;

void load_addresses() {
	ReadProcessMemory(hProcess, (LPCVOID) BASE, (LPVOID) &baseAddr, 4, NULL);
	ReadProcessMemory(hProcess, (LPCVOID) baseAddr+PL_HP_OFF, (LPVOID) &PLAYER_HP_ADDR, 4, NULL);
	ReadProcessMemory(hProcess, (LPCVOID) baseAddr+EN_HP_OFF, (LPVOID) &EN_HP_ADDR, 4, NULL);
	ReadProcessMemory(hProcess, (LPCVOID) baseAddr+EN_HP1_OFF, (LPVOID) &EN_HP1_ADDR, 4, NULL);
	ReadProcessMemory(hProcess, (LPCVOID) baseAddr+EN_HP2_OFF, (LPVOID) &EN_HP2_ADDR, 4, NULL);
	ReadProcessMemory(hProcess, (LPCVOID) baseAddr+TANGLED_HAIR_OFF, (LPVOID) &TANGLED_HAIR_ADDR, 4, NULL);
}

bool open_process() {
	int dwProcessId;	
    HWND GameWindowHandle = FindWindow(NULL, WINDOW_NAME);
    GetWindowThreadProcessId(GameWindowHandle, (LPDWORD) &dwProcessId);
    std::cout << "WindowThreadProcessId: " << dwProcessId << std::endl;
 	hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, false, dwProcessId);
 	return hProcess != NULL;
}

int read_byte(LPCVOID addr) {
	int ret;
	ReadProcessMemory(hProcess, addr, (LPVOID) &ret, 1, NULL);
	return ret;
}

void debug() {
	cout << "PLAYER_HP_ADDR " << PLAYER_HP_ADDR << endl;
	cout << "PLAYER HP " << read_byte((LPCVOID) baseAddr+PL_HP_OFF) << endl;
}
