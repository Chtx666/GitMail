#include "all.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iterator>
using namespace std;

void Init() {
    isLogIn = false;
}

void LogIn() {
    if (isLogIn) return;
    cout << "New account name without blank and enter: ";
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

void Process() {
    Receive();
}

void Receive() {
    system(("git -C " + string(dataRepoName) + " fetch origin " + accountName).c_str());
    string status = RunCmd("git -C " + string(dataRepoName) + " rev-list --count HEAD..origin/" + accountName);
    while (!status.empty() && (status.back() == '\n' || status.back() == '\r'))
        status.pop_back();
    cout << status + " new GMail.\n";
    if (status == "0") return;
    system(("git -C " + string(dataRepoName) + " pull origin " + string(accountName)).c_str());
    string added = RunCmd(("git -C " + string(dataRepoName) + " diff --name-only --diff-filter=A ORIG_HEAD HEAD").c_str());
    istringstream iss(added);
    string filename;
    while (getline(iss, filename)) {
        if (filename.empty()) continue;
        ifstream f(string(dataRepoName) + "/" + filename);
        string content((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
        cout << "[" << filename << "]\n";
        cout << "\t" + RunCmd(("git -C " + string(dataRepoName) + " log -1 --format=\"%ci\" -- " + string(filename)).c_str()) + "\n";
        cout << content << "\n";
    }
}

string RunCmd(const string& cmd) {
    std::string out;
    char buf[256];
    FILE* p = POPEN(cmd.c_str(), "r");
    if (!p) return "";
    while (fgets(buf, sizeof buf, p)) out += buf;
    PCLOSE(p);
    return out;
}