#include <windows.h>
#include <iostream>
#include "memory.h"
#include <cstdint>

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
	PLAYER_HP_ADDR = (LPCVOID) ((intptr_t)baseAddr+PL_HP_OFF);
	EN_HP_ADDR = (LPCVOID) ((intptr_t)baseAddr+EN_HP_OFF);
	EN_HP1_ADDR = (LPCVOID) ((intptr_t)baseAddr+EN_HP1_OFF);
	EN_HP2_ADDR = (LPCVOID) ((intptr_t)baseAddr+EN_HP2_OFF);
	TANGLED_HAIR_ADDR = (LPCVOID) ((intptr_t)baseAddr+TANGLED_HAIR_OFF);
	MOB_TYPE_ADDR = (LPCVOID) ((intptr_t)baseAddr+MOB_TYPE_OFF);
}
int get_enemy_state(enum MOB_TYPE type) {
	return read_byte((LPCVOID) ((intptr_t)baseAddr + (int)ENEMY_FLAGS_BASE_OFF + (int)type));
}
void set_enemy_state(enum MOB_TYPE type, int val) {
	write_byte((LPCVOID) ((intptr_t)baseAddr + (int)ENEMY_FLAGS_BASE_OFF + (int)type), val);
}
bool open_process() {
	int dwProcessId = 0;	
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
//Return as "int", not "BYTE" because "BYTE" is "unsigned char" and thus causes confusion with cout << read_byte(...);
int read_byte(LPCVOID addr) {
	int ret = 0;
	ReadProcessMemory(hProcess, addr, (LPVOID) &ret, 1, NULL);
	return ret;
}
enum MOB_TYPE get_mob_type() {
	int type = 0;
	ReadProcessMemory(hProcess, MOB_TYPE_ADDR, (LPVOID) &type, 1, NULL);
	return MOB_TYPE(type);
}
void set_mob_type(enum MOB_TYPE type) {
	write_byte(MOB_TYPE_ADDR, (int) type);
}
void write_byte(LPCVOID addr, int val) {
	LPVOID adr = (LPVOID) addr;
	WriteProcessMemory(hProcess, adr, (LPCVOID)&val, 1, NULL);
}

void debug() {
	cout << "BASE ADDR " << baseAddr << endl;
	cout << "PLAYER_HP_ADDR " << PLAYER_HP_ADDR << endl;
	cout << "PLAYER HP " << read_byte(PLAYER_HP_ADDR) << endl;
	cout << "BALDIE STATE " << get_enemy_state(BALDIE) << " addr "<< (LPCVOID) ((intptr_t)baseAddr + (int)ENEMY_FLAGS_BASE_OFF + (int)BALDIE) << endl;
	write_byte(PLAYER_HP_ADDR, 56);
}
