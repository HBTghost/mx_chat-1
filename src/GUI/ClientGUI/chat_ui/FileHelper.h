#pragma once
#include <fileapi.h>
#include <handleapi.h>
#include <Windows.h>
static class FileHelper
{
public:
    static __int64 FileSize(const wchar_t* name)
    {
        HANDLE hFile = CreateFile(name, GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
            return -1; // error condition, could call GetLastError to find out more

        LARGE_INTEGER size;
        if (!GetFileSizeEx(hFile, &size))
        {
            CloseHandle(hFile);
            return -1; // error condition, could call GetLastError to find out more
        }

        CloseHandle(hFile);
        return size.QuadPart;
    }
};

