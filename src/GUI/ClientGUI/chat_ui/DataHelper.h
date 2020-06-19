#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <cstdint>
using namespace std;
static class DataHelper {
public:
	// <summary>
	// Chuyển đổi Uint32 sang 1 mảng 4 bytes unsinged char* little endian
	// </summary>
	// <param name="des">Con trỏ tới mảng cần lưu (4 bytes)</param>
	// <param name="src">Số dạng uint32</param>
	static void ConvertUint32Bit(unsigned char* des, uint32_t src) {
		des[0] = static_cast<unsigned char>(src & 0x000000FF);
		des[1] = static_cast<unsigned char>((src & 0x0000FF00) >> 8);
		des[2] = static_cast<unsigned char>((src & 0x00FF0000) >> 16);
		des[3] = static_cast<unsigned char>((src & 0xFF000000) >> 24);
	}

	// <summary>
	// Chuyển đổi int32 sang 1 mảng 4 bytes char* little endian
	// </summary>
	// <param name="des">Con trỏ tới mảng cần lưu (4 bytes)</param>
	// <param name="src">Số dạng int32</param>
	static void ConvertInt32Bit(char* des, int32_t src) {
		des[0] = static_cast<char>(src & 0x000000FF);
		des[1] = static_cast<char>((src & 0x0000FF00) >> 8);
		des[2] = static_cast<char>((src & 0x00FF0000) >> 16);
		des[3] = static_cast<char>((src & 0xFF000000) >> 24);
	}


	// <summary>
	// Chuyển đổi mảng 4 byte Little Endian sang uint32_t
	// </summary>
	// <param name="src">Con trỏ mảng src chứa số dạng 4 byte</param>
	// <param name="start_pos">Vị trí byte đầu tiên của số từ mảng src</param>
	// <return></return>
	static uint32_t ConvertBitUint32(unsigned char* src, int start_pos = 0) {
		unsigned char temp[4];
		memcpy(temp, src, sizeof(uint32_t));
		uint32_t i32 = temp[0] | (temp[1] << 8) | (temp[2] << 16) | (temp[3] << 24);
		return i32;
	}

	// <summary>
	// Chuyển đổi mảng 4 byte Little Endian sang int32_t
	// </summary>
	// <param name="src">Con trỏ mảng src chứa số dạng 4 byte</param>
	// <param name="start_pos">Vị trí byte đầu tiên của số từ mảng src</param>
	static int32_t ConvertBitInt32(char* src, int start_pos = 0) {
		char temp[4];
		memcpy(temp, src, sizeof(int32_t));
		int32_t i32 = temp[0] | (temp[1] << 8) | (temp[2] << 16) | (temp[3] << 24);
		return i32;
	}

	// <summary>
	// Lấy string từ mảng với ký tự đánh dấu kết thúc là end_character
	// </summary>
	// <param name="src">Mảng cần trích xuất</param>
	// <param name="len">Kích thước của mảng</param>
	// <param name="start_pos">Vị trí bắt đầu trích chuỗi</param>
	// <param name="end_character">Ký tự đánh dấu kết thúc</param>
	// <returns>Chuỗi string </returns>
	static string ConvertBitString(char* src, int len, int start_pos = 0, char end_character = '\0') {
		string rslt = "";
		char t = src[start_pos];
		int i = start_pos;


		while (t != end_character && i < len) {
			rslt += t;
			t = src[++i];
		}
		return rslt;

	}






};
