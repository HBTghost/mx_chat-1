#include "stdafx.h"
#include "Account.h"

Account::Account(wstring user, wstring password) {
	this->setUserName(user);
	this->setPassWord(password);
}

void Account::setUserName(wstring username) {
	_username = (username).c_str();
}

void Account::setPassWord(wstring password) {
	_password = (password).c_str();
}

wstring& Account::getUserName() {
	return _username;
}

wstring& Account::getPassWord() {
	return _password;
}
