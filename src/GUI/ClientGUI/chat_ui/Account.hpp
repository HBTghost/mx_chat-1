#pragma once 
#include <string> 
#include <vector> 
#include <fstream> 
#include <sstream>
#include "Tools.hpp"
#include "SHA256.h"

enum USER_FLAGS { NON_EXIST, WRONG_PASSWORD, RIGHT_PASSWORD };
enum FILE_FLAGS { USER_VALIDATION, USER_FRIENDS, USER_GROUPS, GROUP_MEMBERS };

class Account
{
private:
    std::wstring username;
    std::wstring hashedPassword;
    std::wstring createdDate;
    std::wstring lastActiveDate;

    Account(
        std::wstring username,
        std::wstring password,
        std::wstring createdDate,
        std::wstring lastActiveDate
    ) : username{ username }
        , hashedPassword{ SHA256()(password) }
        , createdDate{ createdDate }
        , lastActiveDate{ lastActiveDate } {}

public:
    ~Account() = default;
    Account(
        std::wstring username,
        std::wstring password
    ) : Account(username, password, Tools().now(), Tools().now()) {}
    Account(
        std::wstring username
    ) : Account(username, L"") {}
    Account(std::vector<std::wstring> info) : Account(info[0], info[1], info[2], info[3]) {
        this->hashedPassword = info[1];
    }
    Account() = default;

    Account* Clone() {
        return new Account(*this);
    }

    // username,password,03/11/2018 14:32:45,27/05/2020 20:13:29 
    Account* InitFromString(std::wstring line) {
        *this = Account(Tools().split(line));
        return this;
    }
    std::wstring ToString() {
        std::wstring res;
        res += username + L",";
        res += hashedPassword + L",";
        res += createdDate + L",";
        res += lastActiveDate;
        return res;
    }
    std::wstring GetUsername() {
        return username;
    }
    bool IsMatchUsername(Account account) {
        return this->username == account.username;
    }
    bool IsMatchUsername(std::wstring username) {
        return this->username == username;
    }
    bool IsMatchPassword(Account account) {
        return this->hashedPassword == account.hashedPassword;
    }
    bool IsMatchPassword(std::wstring password) {
        return this->hashedPassword == SHA256()(password);
    }
    void UpdatePasswordFrom(Account account) {
        this->hashedPassword = account.hashedPassword;
    }
    void UpdatePassword(std::wstring password) {
        this->hashedPassword = SHA256()(password);
    }
};

/////////////////////////////////////////////////////////////////////////////////

//chat
class Group {
public:
    std::wstring name;
    std::vector<std::wstring> members;

public:
    Group() = default; 
    Group(std::vector<std::wstring> info) {
        if (info.size() > 0) {
            name = info[0];
            for (size_t i = 1; i < info.size(); ++i) {
                members.push_back(info[i]);
            }
        }
    }
    Group(std::wstring line) : Group(Tools().split(line)) {}
    ~Group() = default;
    std::vector<std::wstring> split() {
        std::vector<std::wstring> res = members;
        res.insert(res.begin(), name);
        return res;
    }
};

/////////////////////////////////////////////////////////////////////////////////

class AccountManagement
{
private:
    std::wstring UserValidation{ L"./data/accounts/UserValidation.txt" }; //account
    std::wstring UserFriends{ L"./data/accounts/UserFriends.txt" }; //friend
    std::wstring UserGroups{ L"./data/accounts/UserGroups.txt" }; //list groups
    std::wstring GroupMembers{ L"./data/accounts/GroupMembers.txt" }; //member of group
    std::vector<std::wstring> fileList;
    std::vector<std::vector<std::vector<std::wstring>>> files;
    int FILE_FLAG;
public:

    AccountManagement() {
        fileList = std::vector<std::wstring>{ UserValidation, UserFriends, UserGroups, GroupMembers };
        for (size_t i = 0; i < fileList.size(); ++i) {
            std::vector<std::vector<std::wstring>> tmp;
            files.push_back(tmp);
            ReadFromFile(i);
        }
    }
    ~AccountManagement() = default;

    void WriteToFile(int FILE_FLAG) {
        std::wofstream fout(fileList[FILE_FLAG]);
        if (fout.is_open()) {
            for (auto x : files[FILE_FLAG]) {
                fout << Tools().merge(x) << L"\n";
            }
        }
        fout.close();
    }

    void ReadFromFile(int FILE_FLAG) {
        std::wifstream fin(fileList[FILE_FLAG]);
        std::wstring line;
        files[FILE_FLAG].clear();

        if (fin.is_open()) {
            files[FILE_FLAG].clear();
            while (fin.good()) {
                getline(fin, line);
                if (line == L"")
                    break;
                files[FILE_FLAG].push_back(Tools().split(line));
            }
        }
        else {
            WriteToFile(FILE_FLAG);
        }
        fin.close();
    }

    Account GetAccount(std::wstring username) {
        for (auto x : files[USER_VALIDATION]) {
            if (x[0] == username) {
                return Account(x);
            }
        }
        return Account();
    }

    int CheckAccount(Account account) {
        size_t size = files[USER_VALIDATION].size();
        for (auto x : files[USER_VALIDATION]) {
            Account tmp(x);
            if (account.IsMatchUsername(tmp)) {
                if (account.IsMatchPassword(tmp)) {
                    return RIGHT_PASSWORD;
                }
                else {
                    return WRONG_PASSWORD;
                }
            }  
        }
        return NON_EXIST;
    }

    int CheckAccount(Account account, size_t& pos) {
        size_t size = files[USER_VALIDATION].size();
        for (size_t i = 0; i < size; ++i) {
            Account tmp(files[USER_VALIDATION][i]);
            if (account.IsMatchUsername(tmp)) {
                pos = i;
                if (account.IsMatchPassword(tmp)) {
                    return RIGHT_PASSWORD;
                }
                else {
                    return WRONG_PASSWORD;
                }
            }  
        }
        return NON_EXIST;
    }

    int CheckAccountFriends(Account account, size_t& pos) {
        size_t tmp_pos;
        std::wstring username = account.GetUsername();
        if (AccountExisted(username)) {
            size_t size = files[USER_FRIENDS].size();
            for (size_t i = 0; i < size; ++i) {
                if (files[USER_FRIENDS][i][0] == username) {
                    pos = i;
                    return WRONG_PASSWORD;
                }
            }
        }

        return NON_EXIST;
    }

    int CheckAccountGroups(Account account, size_t& pos) {
        size_t tmp_pos;
        std::wstring username = account.GetUsername();
        if (AccountExisted(username)) {
            size_t size = files[USER_GROUPS].size();
            for (size_t i = 0; i < size; ++i) {
                if (files[USER_GROUPS][i][0] == username) {
                    pos = i;
                    return WRONG_PASSWORD;
                }
            }
        }

        return NON_EXIST;
    }

    int CheckGroupMembers(std::wstring group_name , size_t& pos) {
        size_t tmp_pos;
        size_t size = files[GROUP_MEMBERS].size();
        for (size_t i = 0; i < size; ++i) {
            std::vector<std::wstring> line = files[GROUP_MEMBERS][i];
            if (line[0] == group_name) {
                pos = i;
                return WRONG_PASSWORD; 
            }
        }

        return NON_EXIST;
    }

    bool AddAccount(Account account) {
        size_t pos;
        if (CheckAccount(account, pos) == NON_EXIST) {
            files[USER_VALIDATION].push_back(Tools().split(account.ToString()));
            WriteToFile(USER_VALIDATION);

            std::string user = Tools().WstringToString(account.GetUsername());
			std::string cmd = "cd data\\messages && mkdir " + user + " && cd " + user + " && mkdir friends && mkdir groups";
			system(cmd.c_str());
            return true;
        }
        return false;
    }

    bool AddFriend(Account account, std::wstring friend_username) {
        size_t pos;
        if (CheckAccountFriends(account, pos) != NON_EXIST) {
            std::vector<std::wstring>& friends = files[USER_FRIENDS][pos];
            for (auto x : friends) {
              if (x == friend_username) {
                return false;
              }
            }
            friends.push_back(friend_username);
        } else {
            std::vector<std::wstring> line { account.GetUsername(), friend_username };
            files[USER_FRIENDS].push_back(line);
        }
        WriteToFile(USER_FRIENDS);
        return true;
    }

    bool AddGroup(Account account, std::wstring group_name) {
        size_t pos;
        if (CheckAccountGroups(account, pos) != NON_EXIST) {
            std::vector<std::wstring>& groups = files[USER_GROUPS][pos];
            for (auto x : groups) {
                if (x == group_name) {
                    return false;
                }
            }
            groups.push_back(group_name);
        }
        else {
            std::vector<std::wstring> line{ account.GetUsername(), group_name };
            files[USER_GROUPS].push_back(line);
        }
        WriteToFile(USER_GROUPS);
        return true;
    }

    bool CreateGroup(Account account, std::wstring group_name, std::vector<std::wstring> members) {
        size_t pos;
        std::vector<std::wstring> groups;
        if (CheckGroupMembers(group_name, pos) != NON_EXIST) {
            return false;
        }
        else {
            members.insert(members.begin(), group_name);
            files[GROUP_MEMBERS].push_back(members);
        }
        WriteToFile(GROUP_MEMBERS);
        AddGroup(account, group_name);
        return true;
    }

    bool UpdateAccountGroup(Account account, std::wstring ex_group_name, std::wstring new_group_name) {
        size_t pos;
        if (CheckAccountGroups(account, pos) != NON_EXIST) {
            std::vector<std::wstring>& groups = files[USER_GROUPS][pos];
            for (size_t i = 0; i < groups.size(); ++i) {
                if (groups[i] == ex_group_name) {
                    groups[i] = new_group_name;
                    WriteToFile(USER_GROUPS);
                    return true;
                }
            }           
        }
        return false;
    }

    bool UpdateGroup(Account account, std::wstring ex_group_name, Group new_group) {
        size_t pos;
        std::vector<std::wstring> groups;
        if (CheckGroupMembers(ex_group_name, pos) == NON_EXIST) {
            return false;
        }
        files[GROUP_MEMBERS][pos] = new_group.split();
        WriteToFile(GROUP_MEMBERS);
        UpdateAccountGroup(account, ex_group_name, new_group.name);
        return true;
    }

    bool RemoveFriend(Account account, std::wstring friend_username) {
        size_t pos;
        if (CheckAccountFriends(account, pos) != NON_EXIST) {
            std::vector<std::wstring>& friends = files[USER_FRIENDS][pos];
            for (size_t i = 0; i < friends.size(); ++i) {
              if (friends[i] == friend_username) {
                friends.erase(friends.begin() + i);
                WriteToFile(USER_FRIENDS);
                return true;
              }
            }
        }
        return false;
    }

    bool RemoveFriends(Account account, std::vector<std::wstring> friends_username) {
        size_t pos;
        if (CheckAccountFriends(account, pos) != NON_EXIST) {
            std::vector<std::wstring>& friends = files[USER_FRIENDS][pos];
            for (size_t i = 0; i < friends_username.size(); ++i) {
                for (size_t j = 0; j < friends.size(); ++j) {
                    if (friends_username[i] == friends[j]) {
                        friends.erase(friends.begin() + j);
                        --j;
                    }
                }
            }
            WriteToFile(USER_FRIENDS);
        }
        return false;
    }

    bool RemoveAccount(Account account) {
        size_t pos;
        if (CheckAccount(account, pos) != NON_EXIST) {
            files[USER_VALIDATION].erase(files[USER_VALIDATION].begin() + pos);
            WriteToFile(USER_VALIDATION);
            return true;
        }
        return false;
    }

    bool AccountExisted(std::wstring username) {
        size_t pos;
        return CheckAccount(Account(username), pos) != NON_EXIST;
    }
    
    bool UpdateAccountPassword(Account account) {
        size_t pos;
        if (CheckAccount(account, pos) != NON_EXIST) {
            Account tmp(files[USER_VALIDATION][pos]);
            tmp.UpdatePasswordFrom(account);
            files[USER_VALIDATION][pos] = Tools().split(tmp.ToString());
            WriteToFile(USER_VALIDATION);
            return true;
        }
        return false;
    }

    void UpdateAccountFriends(Account account, std::vector<std::wstring> friends) {
        size_t pos;
        friends.insert(friends.begin(), account.GetUsername());
        if (CheckAccountFriends(account, pos) == NON_EXIST) {
            files[USER_FRIENDS].push_back(friends);
        }
        else {
            files[USER_FRIENDS][pos] = friends;
        }
        WriteToFile(USER_FRIENDS);
    }

    std::vector<std::wstring> GetFriends(Account account) {
        size_t pos;
        std::vector<std::wstring> res;
        if (CheckAccountFriends(account, pos) != NON_EXIST) {
            res = files[USER_FRIENDS][pos];
            res.erase(res.begin());
        }
        return res;
    }
    Group GetGroup(std::wstring group_name) {
        size_t pos;
        std::vector<std::wstring> info;
        if (CheckGroupMembers(group_name, pos) != NON_EXIST) {
            info = files[GROUP_MEMBERS][pos];
        }
        return Group(info);
    }
    std::vector<Group> GetGroups(Account account) {
        size_t pos;
        std::vector<std::wstring> line;
        std::vector<Group> res;
        if (CheckAccountGroups(account, pos) != NON_EXIST) {
            line = files[USER_GROUPS][pos];
            line.erase(line.begin());

            for (auto x : line) {
                res.push_back(GetGroup(x));
            }
        }
        return res;
    }

    void RemoveAccountFromGroups(Account account, std::vector<std::wstring> groups_name) {
        std::vector<std::vector<std::wstring>>& group_members = files[GROUP_MEMBERS];
        std::vector<std::vector<std::wstring>>& account_groups = files[USER_GROUPS];

        // Remove account from group members
        for (auto x : groups_name) {
            size_t pos;
            if (CheckGroupMembers(x, pos) != NON_EXIST) {
                std::vector<std::wstring>& group = group_members[pos];
                for (size_t i = 1; i < group.size(); ++i) {
                    if (group[i] == account.GetUsername()) {
                        group.erase(group.begin() + i);
                        break;
                    }
                }
            }
        }

        // Remove groups in account
        size_t pos;
        if (CheckAccountGroups(account, pos) != NON_EXIST) {
            std::vector<std::wstring>& group = account_groups[pos];
            for (auto x : groups_name) {
                for (size_t i = 1; i < group.size(); ++i) {
                    if (x == group[i]) {
                        group.erase(group.begin() + i);
                        break;
                    }
                }
            }
        }

        WriteToFile(GROUP_MEMBERS);
        WriteToFile(USER_GROUPS);
    }
};
