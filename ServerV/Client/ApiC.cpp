#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>

void GetUserInput(SOCKET Connection) {
    while (true) {
        // Отримати вибір користувача та обробити його
        int choice;
        std::cout << "Menu:" << std::endl;
        std::cout << "1. Get process list" << std::endl;
        std::cout << "2. Close process" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Choose action: ";
        std::cin >> choice;

        if (choice == 1) {
            // Отримати список процесів і вивести його
            char process_list[4128];
            recv(Connection, process_list, sizeof(process_list), NULL);
            std::cout << process_list << std::endl;
        }
        else if (choice == 2) {
            // Закрити процес
            std::string request;
            std::cout << "Enter process ID to close: ";
            std::cin >> request;

            // Надіслати запит на сервер
            send(Connection, request.c_str(), request.length(), 0);

            // Отримати статус закриття
            bool process_status = false;
            recv(Connection, reinterpret_cast<char*>(&process_status), sizeof(process_status), 0);

            if (process_status) {
                std::cout << "Process closed successfully!" << std::endl;
            }
            else {
                std::cout << "Error, process closure failed!" << std::endl;
            }
        }
        else if (choice == 3) {
            // Вийти з програми
            exit(1);
        }
        else {
            std::cout << "Try again!" << std::endl;
        }
    }
}