#pragma once

#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <TlHelp32.h>
#pragma warning(disable: 4996)
#else
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <unistd.h>      
#include <arpa/inet.h>   
#include <dirent.h>
#include <cstring>
#include <cstdio>

#define SOCKET int
#endif



void ConvertWCHARtoChar(const wchar_t* wideStr, char* charStr, int charStrSize);

void GetProcessList(wchar_t* msg, int bufferSize);

void HandleClient(SOCKET clientSocket);