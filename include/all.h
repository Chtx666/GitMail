# pragma once

#include <string>
using namespace std;

#ifdef _WIN32
    #define POPEN  _popen
    #define PCLOSE _pclose
#else
    #define POPEN  popen
    #define PCLOSE pclose
#endif

inline const char* dataRepoName = "GitMail-Server";
inline bool isLogIn;
inline string accountName;

void Init();
void LogIn();
void Process();
void Receive();
string RunCmd(const string& cmd);