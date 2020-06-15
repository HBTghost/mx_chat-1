#pragma once
#include <iostream>
#include <string>
using namespace std;
class Account
{
public:
	string username, password;
	string sha_user;

	Account() {}
	Account(string user, string pass)
	{
		this->username = user;
		this->password = pass;
	}
	~Account(){}

};

