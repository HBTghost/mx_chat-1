#pragma once
#include <iostream>
#include <xstring>
using namespace std;
class Account
{
private:
	wstring _username;
	wstring _password;
public:
	Account();
	~Account();
	Account(wstring user, wstring password);
	void setUserName(wstring username);
	void setPassWord(wstring password);
	wstring& getUserName();
	wstring& getPassWord();
};

