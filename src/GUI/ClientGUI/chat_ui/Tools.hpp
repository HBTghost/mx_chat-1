#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include "resource.h"
#include <ctime> 
#include <sstream>
#include "Mmsystem.h"

namespace fs = std::filesystem;
using namespace std;

class Tools
{
public:
    Tools() = default;
    ~Tools() = default;

    bool is_ascii(std::string c) {
        for (size_t i = 0; i < c.length(); i++) {
            if (c[i] < 0) return false;
        }
        return true;
    }

    void Fillter(
        vector<wstring> src,
        vector<wstring> des,
        vector<wstring>* val_add,
        vector<wstring>* val_remove
    ) {
        vector<wstring> commons;
        for (size_t i = 0; i < src.size(); ++i) {
            for (size_t j = 0; j < des.size(); ++j) {
                if (src[i] == des[j]) {
                    commons.push_back(src[i]);
                    src.erase(src.begin() + i);
                    des.erase(des.begin() + j);
                    --i;
                    --j;
                    break;
                }
            }
        }
        *val_add = des;
        *val_remove = src;
    }

    void PlayGotMessSound() {
        PlaySound(TEXT("./res/uh-oh.wav"), NULL, SND_SYNC);
    }

    int countChInString(CString ch, CString str) {
        int p, s = 0;
        p = str.Find(ch);
        while (p != -1) {
            ++s;
            p = str.Find(ch, p + ch.GetLength());
        }
        return s;
    }

    bool isIn(CString _item, std::vector<std::string> list) {
        std::string item = Tools().WstringToString(std::wstring(_item));
        for (auto x : list) {
            if (x == item) {
                return true;
            }
        }
        return false;
    }

    std::vector<std::wstring> split(std::wstring line, wchar_t ch = L',') {
        std::wstring temp;
        std::vector<std::wstring> parts;
        std::wstringstream wss(line);
        while (getline(wss, temp, ch)) {
            parts.push_back(temp);
        }
        return parts;
    }
    std::wstring merge(std::vector<std::wstring> parts, std::wstring ch = L",") {
        std::wstring res;
        for (auto x : parts) {
            res += x + ch;
        }
        res.pop_back();
        return res;
    }

    std::wstring TmToString(tm* ltm) {
        int hour, min, sec, day, month, year;

        hour = ltm->tm_hour + 1;
        min = ltm->tm_min + 1;
        sec = ltm->tm_sec + 1;
        day = ltm->tm_mday;
        month = ltm->tm_mon + 1;
        year = ltm->tm_year + 1900;

        std::wstring res;
        res += std::to_wstring(hour) + L":";
        res += std::to_wstring(min) + L":";
        res += std::to_wstring(sec) + L" ";
        res += std::to_wstring(day) + L"/";
        res += std::to_wstring(month) + L"/";
        res += std::to_wstring(year);

        return res;
    }

    std::wstring now() {
        time_t time_now = time(0);
        tm* ltm = new tm;
        localtime_s(ltm, &time_now);
        return TmToString(ltm);
    }

    bool is_exist(const fs::path& p, fs::file_status s = fs::file_status{})
    {
        return fs::status_known(s) ? fs::exists(s) : fs::exists(p);
    }
    void genNeededFolder()
    {
        if (!is_exist("data")) {
            fs::create_directory("data");
        }
        if (!is_exist("data/accounts")) {
            fs::create_directory("data/accounts");
        }
        if (!is_exist("data/messages")) {
            fs::create_directory("data/messages");
        }
    }
    std::string WstringToString(std::wstring wstr) {
        std::string res;
        for (auto x : wstr) {
            res += x;
        }
        return res;
    }
    std::wstring StringToWstring(std::string str) {
        std::wstring res;
        for (auto x : str) {
            res += x;
        }
        return res;
    }

    std::vector<CString> ReadFile(std::wstring path)
    {
        CStdioFile readFile;
        CFileException fileException;
        CString strFilePath = path.c_str();
        CString strLine;
        std::vector<CString> messages;

        if (readFile.Open(strFilePath, CFile::modeRead | CFile::typeUnicode, &fileException))
        {
            while (readFile.ReadString(strLine)) {
                messages.push_back(strLine);
            }
            readFile.Close();
        }

        return messages;
    }

    void WriteToFile(std::wstring path, std::vector<CString> messages)
    {
        CStdioFile writeToFile;
        CFileException fileException;
        CString strFilePath = path.c_str();

        if (writeToFile.Open(strFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeUnicode), &fileException)
        {
            for (auto x : messages) {
                writeToFile.WriteString(x + _T("\n"));
            }
            writeToFile.Close();
        }
    }
};