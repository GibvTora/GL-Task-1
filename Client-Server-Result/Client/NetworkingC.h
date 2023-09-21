#pragma once

#include <iostream>
#include <string>
#include <thread>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
typedef int SOCKET;
#endif

void InitializeNetwork();
SOCKET ConnectToServer();
void CleanupNetwork(SOCKET conn);