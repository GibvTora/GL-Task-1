#pragma once

#include <winsock2.h>
#include <Windows.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#pragma warning(disable: 4996)

// Функція ініціалізації бібліотеки Winsock
bool InitializeWinsock();

// Функція закриття бібліотеки Winsock
void CleanupWinsock();

// Функція для створення сокету
SOCKET CreateSocket();

// Функція для закриття сокету
void CloseSocket(SOCKET socket);

// Функція для бінду сокета до адреси та порту
bool BindSocket(SOCKET socket, const char* ipAddress, int port);

// Функція для прослуховування з'єднань
bool ListenSocket(SOCKET socket);

// Функція для прийому з'єднання
SOCKET AcceptSocket(SOCKET serverSocket, SOCKADDR_IN& clientAddress, int& clientAddressSize);
