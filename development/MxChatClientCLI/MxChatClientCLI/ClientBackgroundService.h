#pragma once
#include <windows.h>
#include "ClientSocket.h"

extern ClientSocket gClientObj;
UINT  recMessageThread(LPVOID lParam);