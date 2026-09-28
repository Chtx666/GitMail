#include "all.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
using namespace std;

void Init() {
    isLogIn = false;
}

void LogIn() {
    if (isLogIn) return;
    cout << "New account name without blank and enter: ";
    string accountName;
    cin >> accountName;
    if (!(filesystem::exists(dataRepoName) && filesystem::is_directory(dataRepoName))) 
        system(("git clone https://github.com/Chtx666/" + string(dataRepoName) + ".git").c_str());
    if (system(("git -C " + string(dataRepoName) + " switch -c " + accountName).c_str()) != 0) {
        cout << "Account exists. Log in automatically.\n";
        isLogIn = true;
        return;
    }
    cout << "Sign up successfully.\n";
    ofstream f(string(dataRepoName) + "/hello_world.txt");
    f << "Hello, " << accountName << "!\n";
    f.close();
    system(("git -C " + string(dataRepoName) + " add .").c_str());
    system(("git -C " + string(dataRepoName) + " commit -m \"Create account: " + accountName + "\"").c_str());
    system(("git -C " + string(dataRepoName) + " push -u origin " + accountName).c_str());
    cout << "Confirm your User ID: " << accountName << "\n";
    isLogIn = true;
}

