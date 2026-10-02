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
        RunCmd("git clone https://github.com/Chtx666/" + string(DATA_REPO_NAME) + ".git");
    
    string r = RunCmd("git -C " + DATA_REPO_PATH + " ls-remote --heads origin refs/heads/" + accountName);
    
    if (r.empty()) {
        RunCmd("git -C " + DATA_REPO_PATH + " switch -c " + accountName);
        cout << "Sign up successfully.\n";
        ofstream f(DATA_REPO_PATH + "/hello_world.txt");
        f << "Hello, " << accountName << "!\n";
        f.close();
        RunCmd("git -C " + DATA_REPO_PATH + " add .");
        RunCmd("git -C " + DATA_REPO_PATH + " commit -m \"Create account: " + accountName + "\"");
        RunCmd("git -C " + DATA_REPO_PATH + " push -u origin " + accountName);
    } else {
        RunCmd("git -C " + DATA_REPO_PATH + " switch " + accountName);
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
    RunCmd("git -C " + DATA_REPO_PATH + " fetch origin");
    string status = RunCmd("git -C " + DATA_REPO_PATH + " rev-list --count HEAD..origin/" + accountName);
    while (!status.empty() && (status.back() == '\n' || status.back() == '\r'))
        status.pop_back();
    cout << status + " new GMail.\n";
    if (status == "0") return;
    
    RunCmd("git -C " + DATA_REPO_PATH + " pull origin " + string(accountName));
    string added = RunCmd("git -C " + DATA_REPO_PATH + " diff --name-only --diff-filter=A ORIG_HEAD HEAD");
    
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
    FILE* p = POPEN((cmd + " 2>&1").c_str(), "r");
    if (!p) return "";
    while (fgets(buf, sizeof buf, p)) out += buf;
    PCLOSE(p);
    ofstream log("log.txt", ios::app);
    log << out;
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
    RunCmd("git -C " + DATA_REPO_PATH + " fetch origin");
    RunCmd("git -C " + DATA_REPO_PATH + " switch " + sendTo);
    RunCmd("git -C " + DATA_REPO_PATH + " pull origin " + sendTo);

    ofstream f(DATA_REPO_PATH + "/" + title + ".txt");
    f << sendContent;
    f.close();

    RunCmd("git -C " + DATA_REPO_PATH + " add .");
    RunCmd("git -C " + DATA_REPO_PATH + " commit -m \"" + title + "\"");
    RunCmd("git -C " + DATA_REPO_PATH + " push origin " + sendTo);
    RunCmd("git -C " + DATA_REPO_PATH + " switch " + accountName);
}