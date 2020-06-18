//https://www.scadacore.com/tools/programming-calculators/online-hex-converter/
#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <cstdint>
#include <random>
using namespace std;
static class StringHelper
{
public:
  

   
    static std::string random_string()
    {
        std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

        std::random_device rd;
        std::mt19937 generator(rd());

        std::shuffle(str.begin(), str.end(), generator);

        return str.substr(0, 32);    // assumes 32 < number of characters in str         
    }
    static std::vector<std::wstring> VectorStringToWideString(std::vector<std::string> src) {
        std::vector<std::wstring> rslt;
        for (std::string item : src) {
            rslt.push_back(s2ws(item));
        }
        return rslt;
    }
    static std::vector<std::string> VectorWideStringToString(std::vector<std::wstring> src){
        std::vector<std::string> rslt;
        for (std::wstring item : src) {
            rslt.push_back(utf8_encode(item));
        }
        return rslt;
    }
    static std::wstring s2ws(const std::string& s)
    {
        int len;
        int slength = (int)s.length() + 1;
        len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
        wchar_t* buf = new wchar_t[len];
        MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
        std::wstring r(buf);
        delete[] buf;
        return r;
    }

    // Convert an UTF8 string to a wide Unicode String
    static std::wstring utf8_decode(const std::string& str)
    {
        if (str.empty()) return std::wstring();
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
        std::wstring wstrTo(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
        return wstrTo;
    }
    // Convert a wide Unicode string to an UTF8 string
       static std::string utf8_encode(const std::wstring & wstr)
    {
        if (wstr.empty()) return std::string();
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
        std::string strTo(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
        return strTo;
    }



};