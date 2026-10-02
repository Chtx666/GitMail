# pragma once

#include <string>

#ifdef _WIN32
    #define POPEN  _popen
    #define PCLOSE _pclose
#else
    #define POPEN  popen
    #define PCLOSE pclose
#endif

inline const char* DATA_REPO_NAME = "GitMail-Server";
inline std::string DATA_REPO_PATH;
inline const std::string END_MARK = "<<END>>";
inline bool isLogIn;
inline std::string accountName;

void Init();
void LogIn();
void Process();
void Receive();
std::string RunCmd(const std::string& cmd);
void Send();