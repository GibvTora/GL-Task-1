#include "NetworkingS.h"
#include "ApiS.h"
#include <thread>

int main(int argc, char* argv[]) {
    // ����������� �������� WSA
    if (!InitializeWinsock()) {
        std::cout << "Error initializing Winsock" << std::endl;
        return 1;
    }

    // ��������� ������
    SOCKET serverSocket = CreateSocket();
    if (serverSocket == INVALID_SOCKET) {
        std::cout << "Error creating socket" << std::endl;
        CleanupWinsock();
        return 1;
    }

    // ����'���� ������ �� �����
    if (!BindSocket(serverSocket, "127.0.0.1", 1111)) {
        std::cout << "Error binding socket" << std::endl;
        CloseSocket(serverSocket);
        CleanupWinsock();
        return 1;
    }

    // ��������������� �'������
    if (!ListenSocket(serverSocket)) {
        std::cout << "Error listening on socket" << std::endl;
        CloseSocket(serverSocket);
        CleanupWinsock();
        return 1;
    }

    while (true) {
        std::cout << "Waiting for connection..." << std::endl;

        SOCKADDR_IN clientAddress;
        int clientAddressSize = sizeof(clientAddress);
        SOCKET clientSocket = AcceptSocket(serverSocket, clientAddress, clientAddressSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cout << "Error accepting client connection" << std::endl;
        }
        else {
            std::cout << "Client Connected!\n";

            // ��������� �������� ������ ��� ������� �볺�������� ������
            std::thread clientThread(HandleClient, clientSocket);
            clientThread.detach(); // ³����������� ������, ��� �� �������� ���������
            // ��� �'������� ���� ���������� ������� ��� ����������
        }
    }

    CloseSocket(serverSocket);
    CleanupWinsock();

    return 0;
}