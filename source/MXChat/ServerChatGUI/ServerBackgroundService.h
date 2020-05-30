#pragma once
#include <windows.h>

extern ServerSocket gServerObj;

UINT  listenServerThread(LPVOID lParam);
UINT  recServerThread(LPVOID lParam);