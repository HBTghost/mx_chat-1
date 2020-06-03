#pragma once
#include "stdafx.h"
#include <iostream>
#include "ClientBackgroundService.h"
#include <chrono>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <io.h>


using namespace std;
ClientBackgroundService client;

wstring username = L"admin";
wstring password = L"pass";


void printMenu() {
   wcout << "*******************************\n";
    wcout << " 1 - Đăng nhập.\n";
    wcout << " 2 - Danh sách online.\n";
    wcout << " 3 - Tạo cuộc hội thoại riêng.\n";
    wcout << " 4 - Help.\n";
    wcout << " 5 - Exit.\n";
    wcout << " Enter your choice and press return: ";
}

void dangnhap() {
  
    /*
    if (argc >= 2)
    {
        username = StringHelper::multiByteToWideChar(argv[1]);
        password = StringHelper::multiByteToWideChar(argv[2]);
        wcout << "Arguments : " << username << " - " << password << endl;
    }
    */
    wcout << "Username: "; wcin >> username;
    wcout << "Password: "; wcin >> password;

    client.TestLogin(username, password);

}
void get_list_online() {
    
}
void send_private_message() {
    wstring des_user = L"";

    wcout << "User to chat : "; wcin >> des_user;
    bool chatOn = true;
    wstring message = L""; 
    while (chatOn == true) {
        wcout << username << ">";
        //wcin.ignore();
        getline(wcin, message);
        client.TestPrivateMessage(des_user, message);

        if (StringHelper::wstringCompare(message, L"exit")) {
            chatOn = false; 
        }
    }
}
int main(int argc, char** argv)
{

    std::wcout << "Hello World Client!\n";
    client.InitClient();
    client.CreateWorkerThread();

    //
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);
    //SetConsoleTitleW(L"Việt Nam Vô Địch!");
    //HANDLE hdlConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //CONSOLE_FONT_INFOEX consoleFont;
    //ConsoleFont.cbSize = sizeof(consoleFont);
    //GetCurrentConsoleFontEx(hdlConsole, FALSE, &consoleFont);
    //memcpy(consoleFont.FaceName, L"Consolas", sizeof(consoleFont.FaceName));
    //SetCurrentConsoleFontEx(hdlConsole, FALSE, &consoleFont);

    int counter;
   
    int choice = 0;
    bool gameOn = true;
    while (gameOn != false) {
        wcout << "*******************************\n";
        wcout << " 1 - Đăng nhập.\n";
        wcout << " 2 - Danh sách online.\n";
        wcout << " 3 - Tạo cuộc hội thoại riêng.\n";
        wcout << " 4 - Help.\n";
        wcout << " 5 - Exit.\n";
        wcout << " Enter your choice and press return: ";
        wcin >> choice;

        switch (choice)
        {
        case 1:
            dangnhap();
            break;
        case 2:
            get_list_online();
            break;
        case 3:
            send_private_message();
        default:
            break;
        }
    }
    
    
	//client.TestLogin(L"mod" , L"pass");


	while (true);
	return 0;
}
