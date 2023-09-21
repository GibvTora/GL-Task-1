#pragma once

#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define closesocket close
#endif


bool InitializeNetworkLibrary();

void CleanupNetworkLibrary();

SOCKET CreateSocket();

void CloseSocket(SOCKET socket);

bool BindSocket(SOCKET socket, const char* ipAddress, int port);

bool ListenSocket(SOCKET socket);

SOCKET AcceptSocket(SOCKET serverSocket, sockaddr_in& clientAddress, int& clientAddressSize);
