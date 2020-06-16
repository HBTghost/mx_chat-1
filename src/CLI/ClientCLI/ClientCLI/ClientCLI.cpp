// ClientCLI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include <iostream>
#include "ClientBackgroundService.h"
using namespace std;
ClientBackgroundService client;

string username = "";
string password = "";
void printMenu() {
	wcout << "*******************************\n";
	wcout << L" 1 - Login .\n";
	wcout << L" 2 - List Online \n";
	wcout << L" 3 - New private conversation.\n";
	wcout << L" 4 - Transfer File.\n";
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
    cout << "Username: "; cin >> username;
    cout << "Password: "; cin >> password;

    client.Login(username, password);

}
void get_list_online() {}
void send_private_message() {
    cout << "=== create message ===" << endl;
    string user_chat = "";
    cout << "Input user you want to chat:"; cin >> user_chat;
    client.CreatePrivateConversation(user_chat);

}

int main(int argc, char** argv)
{

    std::wcout << "Hello World Client!\n";

    client.InitClient();
    client.CreateWorkerThread();
  
    int counter;

    int choice = 0;
    bool gameOn = true;
    while (gameOn != false) {
        printMenu();
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
    return 0;
}

/*
int main(int argc, char** argv)
{
	std::cout << "Hello World Client!\n";
	client.InitClient();
	client.CreateWorkerThread();
	string user = "";
	string pass = "";
	cout << "Username: "; cin >> user;
	cout << "Password: "; cin >> pass;
	client.Login(user, pass);
	cout << "=== create message ===" << endl;
	string user_chat = "";
	cout << "Input user you want to chat:"; cin >> user_chat;
	client.CreatePrivateConversation(user_chat);
	
	while (true);
	return 0;
}
*/
