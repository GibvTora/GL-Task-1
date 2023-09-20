#pragma once

#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <TlHelp32.h>
#else
#include <unistd.h>
#include <dirent.h>
#include <cstring>
#include <cstdio>
#endif

#pragma warning(disable: 4996)

void ConvertWCHARtoChar(const wchar_t* wideStr, char* charStr, int charStrSize);

void GetProcessList(wchar_t* msg, int bufferSize);

void HandleClient(SOCKET clientSocket);
