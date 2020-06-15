#pragma once
#include <windows.h>
#include "ServerSocket.h"

extern ServerSocket gServerObj;

UINT  listenServerThread(LPVOID lParam);
UINT  recServerThread(LPVOID lParam);