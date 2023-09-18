#pragma once

#include <winsock2.h>
#include <Windows.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#pragma warning(disable: 4996)

// ������� ����������� �������� Winsock
bool InitializeWinsock();

// ������� �������� �������� Winsock
void CleanupWinsock();

// ������� ��� ��������� ������
SOCKET CreateSocket();

// ������� ��� �������� ������
void CloseSocket(SOCKET socket);

// ������� ��� ���� ������ �� ������ �� �����
bool BindSocket(SOCKET socket, const char* ipAddress, int port);

// ������� ��� ��������������� �'������
bool ListenSocket(SOCKET socket);

// ������� ��� ������� �'�������
SOCKET AcceptSocket(SOCKET serverSocket, SOCKADDR_IN& clientAddress, int& clientAddressSize);
