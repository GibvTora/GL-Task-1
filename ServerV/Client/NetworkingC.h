#pragma once

#include <winsock2.h>
#include <iostream>
#include <tchar.h>
#include <string>
#include <cstring>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

void InitializeNetwork();
SOCKET ConnectToServer();
void CleanupNetwork(SOCKET conn);