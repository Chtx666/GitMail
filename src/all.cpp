#include "all.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iterator>
#include <cstdlib>
using namespace std;

void Init() {
    isLogIn = false;
    DATA_REPO_PATH = std::filesystem::absolute(DATA_REPO_NAME).string();
}

void LogIn() {
    if (isLogIn) return;
    cout << "New account name without blank and enter: ";
    cin >> accountName;
    
    if (!(filesystem::exists(DATA_REPO_PATH) && filesystem::is_directory(DATA_REPO_PATH))) 
        system(("git clone https://github.com/Chtx666/" + string(DATA_REPO_NAME) + ".git").c_str());
    
    string r = RunCmd("git -C " + DATA_REPO_PATH + " ls-remote --heads origin refs/heads/" + accountName);
    
    if (r.empty()) {
        system(("git -C " + DATA_REPO_PATH + " switch -c " + accountName).c_str());
        cout << "Sign up successfully.\n";
        ofstream f(DATA_REPO_PATH + "/hello_world.txt");
        f << "Hello, " << accountName << "!\n";
        f.close();
        system(("git -C " + DATA_REPO_PATH + " add .").c_str());
        system(("git -C " + DATA_REPO_PATH + " commit -m \"Create account: " + accountName + "\"").c_str());
        system(("git -C " + DATA_REPO_PATH + " push -u origin " + accountName).c_str());
    } else {
        system(("git -C " + DATA_REPO_PATH + " switch " + accountName).c_str());
        cout << "Account exists. Log in automatically.\n";
    }
    
    cout << "Confirm your User ID: " << accountName << "\n";
    isLogIn = true;
}

void Process() {
    Receive();
    
    cout << "Do you wanna send or quit? (S/Q)\n";
    char reply;
    cin >> reply;
    switch (reply) {
    case 'S':
    case 's':
        Send();
        break;
    case 'Q':
    case 'q':
        cout << "See ya!\n";
        exit(0);
    default:
        cout << "Can't understand SIMIDA.\nTry again.\n";
    }
    
    Process();
}

void Receive() {
    system(("git -C " + DATA_REPO_PATH + " fetch origin " + accountName).c_str());
    string status = RunCmd("git -C " + DATA_REPO_PATH + " rev-list --count HEAD..origin/" + accountName);
    while (!status.empty() && (status.back() == '\n' || status.back() == '\r'))
        status.pop_back();
    cout << status + " new GMail.\n";
    if (status == "0") return;
    
    system(("git -C " + DATA_REPO_PATH + " pull origin " + string(accountName)).c_str());
    string added = RunCmd(("git -C " + DATA_REPO_PATH + " diff --name-only --diff-filter=A ORIG_HEAD HEAD").c_str());
    
    istringstream iss(added);
    string filename;
    while (getline(iss, filename)) {
        if (filename.empty()) continue;
        ifstream f(DATA_REPO_PATH + "/" + filename);
        string content((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
        cout << "[" << filename << "]\n";
        cout << "\t" + RunCmd(("git -C " + DATA_REPO_PATH + " log -1 --format=\"%ci\" -- " + string(filename)).c_str()) + "\n";
        cout << content << "\n";
    }
}

string RunCmd(const string& cmd) {
    string out;
    char buf[256];
    FILE* p = POPEN(cmd.c_str(), "r");
    if (!p) return "";
    while (fgets(buf, sizeof buf, p)) out += buf;
    PCLOSE(p);
    return out;
}

void Send() {
    cout << "To: ";
    string sendTo;
    cin >> sendTo;
    
    cout << "Sing-line Title input:\n";
    string title;
    cin >> title;
    
    cout << "Multi-line content input. End with a single line of \"" + END_MARK + "\":\n";
    string sendContent, line;
    cin.ignore();
    while (getline(cin, line)) {
        if (line == END_MARK) break;
        sendContent += line + "\n";
    }
    
    string r = RunCmd("git -C " + DATA_REPO_PATH + " ls-remote --heads origin refs/heads/" + sendTo);
    if (r.empty()) {
        cout << "User doesn't exist.\n";
        return;
    } 
    system(("git -C " + DATA_REPO_PATH + " fetch origin " + sendTo).c_str());
    system(("git -C " + DATA_REPO_PATH + " switch " + sendTo).c_str());
    system(("git -C " + DATA_REPO_PATH + " pull origin " + sendTo).c_str());

    ofstream f(DATA_REPO_PATH + "/" + title + ".txt");
    f << sendContent;
    f.close();

    system(("git -C " + DATA_REPO_PATH + " add .").c_str());
    system(("git -C " + DATA_REPO_PATH + " commit -m \"" + title + "\"").c_str());
    system(("git -C " + DATA_REPO_PATH + " push origin " + sendTo).c_str());
    system(("git -C " + DATA_REPO_PATH + " switch " + accountName).c_str());
}