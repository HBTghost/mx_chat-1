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
	static bool wstringCompare(std::wstring str, const wchar_t* str_com) {
		return wcscmp(str.c_str(), (str_com)) == 0;
	}
	static bool wstringCompare(std::wstring str, std::wstring str_com) {
		return wcscmp(str.c_str(), (str_com).c_str()) == 0;
	}
	static wchar_t* multiByteToWideChar(char *str) {
		int wchars_num = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
		wchar_t* wstr = new wchar_t[wchars_num];
		MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr, wchars_num);
		return wstr;
	}

	static std::string ConvertWideToANSI(const std::wstring& wstr)
	{
		int count = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.length(), NULL, 0, NULL, NULL);
		std::string str(count, 0);
		WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
		return str;
	}

	static std::wstring ConvertAnsiToWide(const std::string& str)
	{
		int count = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), NULL, 0);
		std::wstring wstr(count, 0);
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), &wstr[0], count);
		return wstr;
	}

	static std::string ConvertWideToUtf8(const std::wstring& wstr)
	{
		int count = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.length(), NULL, 0, NULL, NULL);
		std::string str(count, 0);
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
		return str;
	}

	static std::wstring ConvertUtf8ToWide(const std::string& str)
	{
		int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
		std::wstring wstr(count, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);
		return wstr;
	}
};

