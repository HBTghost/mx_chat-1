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


};