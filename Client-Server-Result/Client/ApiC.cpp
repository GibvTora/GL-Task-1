#include <iostream>
#include <string>
#include "NetworkingC.h"

const int PROCESS_LIST_BUFFER_SIZE = 4128; 

void GetUserInput(SOCKET Connection) {
    while (true) {    //Просте меню
        int choice;
        std::cout << "Menu:" << std::endl;
        std::cout << "1. Get process list" << std::endl;
        std::cout << "2. Close process" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Choose action: ";
        std::cin >> choice;

        if (choice == 1) {    //Отримати список процесів від серверу та вивести в консоль
            char process_list[PROCESS_LIST_BUFFER_SIZE];
            recv(Connection, process_list, sizeof(process_list), 0);
            std::cout << process_list << std::endl;
        }
        else if (choice == 2) {    //Відправити запит на закриття певного процесу
            std::string request;
            std::cout << "Enter process ID to close: ";
            std::cin >> request;

            send(Connection, request.c_str(), request.length(), 0);

            bool process_status = false;    //Отримали статус закриття процесу
            recv(Connection, reinterpret_cast<char*>(&process_status), sizeof(process_status), 0);

            if (process_status) {
                std::cout << "Process closed successfully!" << std::endl;
            }
            else {
                std::cout << "Error, process closure failed!" << std::endl;
            }
        }
        else if (choice == 3) {
            exit(1);
        }
        else {
            std::cout << "Try again!" << std::endl;
        }
    }
}
