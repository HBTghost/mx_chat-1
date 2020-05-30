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
	Account(wstring user, wstring password) {
		this->setUserName(user);
		this->setPassWord(password);
	}
	void setUserName(wstring username){
		_username = (username).c_str();
	}
	void setPassWord(wstring password) {
		_password = (password).c_str();
	}
	wstring& getUserName() {
		return _username;
	}
	wstring& getPassWord() {
		return _password;
	}
};

