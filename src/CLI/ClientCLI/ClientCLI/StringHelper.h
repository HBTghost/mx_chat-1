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


};