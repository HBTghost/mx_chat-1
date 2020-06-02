#pragma once
#include <afxwin.h>
#include <WinSock2.h>
#include <list>
#include <vector>
#include <iostream>
using namespace std;
static class StringHelper
{
public:
	static WCHAR* wstringToWcharP(std::wstring str) {
		int n = str.size();
		WCHAR* rslt = new WCHAR[n];
		for (int i = 0; i < n; i++) {
			rslt[i] = str[i];
		}
		return rslt;
	}
	static WCHAR* wstringToWcharFixedP(std::wstring str, int len) {
		int n = str.size();
		WCHAR* rslt = new WCHAR[len];
		for (int i = 0; i < n; i++) {
			rslt[i] = str[i];
		}
		return rslt;
	}
};

