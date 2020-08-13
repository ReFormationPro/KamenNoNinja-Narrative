# KamenNoNinja-Narrative
A narrative mode for one of the best games I have played on NES.

## What does it do?
This project aims to create dialogue screens for boss matches.
It uses external memory manipulation methods to read game status.
Supports reading boss state and spawning bosses.

## How to make it work?
Currently, it expects window name to be "FCEUX 2.2.3: Kamen No Ninja Original".
Base pointer is hardcoded according to my system. Later this will be changed.
You need to set this line according to your system and then it should work.
```
LPCVOID BASE = (LPCVOID) 0x007B1388;	//fceux.exe+3B1388
```
