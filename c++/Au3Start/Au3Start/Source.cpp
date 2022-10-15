
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string>

using namespace std;

string ExePath() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    string::size_type pos = string(buffer).find_last_of("\\/");
    if (pos == string::npos) {
        return "";
    }else {
        return string(buffer).substr(0, pos);
    }
}

int StringToWString(std::wstring& ws, const std::string& s)
{
    std::wstring wsTmp(s.begin(), s.end());

    ws = wsTmp;

    return 0;
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
   
    string exePath = ExePath();
    wstring wexePath(exePath.begin(), exePath.end());

    wstring application;
    application.append(wexePath).append(L"\\bin\\avm");
    wstring arg;
    arg.append(L" \"").append(wexePath).append(L"\\bin\\libUI.dat\" ");
    string environent;
    environent.append("PATH=").append(exePath).append("\\bin;").append(std::getenv("PATH"));
    CreateProcess(application.c_str(), &arg[0], NULL, NULL, 0,
        CREATE_NO_WINDOW, (LPVOID) environent.c_str(), NULL, &si, &pi);
    return 0;
}

