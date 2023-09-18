#pragma once
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <TlHelp32.h>
#include <string>
#include <cstring>

void ConvertWCHARtoChar(const WCHAR* wideStr, char* charStr, int charStrSize);

void GetProcessList(WCHAR* msg);

void HandleClient(SOCKET clientSocket);