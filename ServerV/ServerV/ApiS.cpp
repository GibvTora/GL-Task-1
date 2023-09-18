#include "ApiS.h"

const int bufferSize = 4096;

void ConvertWCHARtoChar(const WCHAR* wideStr, char* charStr, int charStrSize) {
    WideCharToMultiByte(CP_ACP, 0, wideStr, -1, charStr, charStrSize, NULL, NULL);
}

void GetProcessList(WCHAR* msg) {
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Initialize the buffer to an empty string
    msg[0] = L'\0';

    // Get the first process
    if (Process32First(hProcessSnap, &pe32)) {
        // Add the first process to the list with PID
        swprintf(msg, bufferSize, L"%d: %s\n", pe32.th32ProcessID, pe32.szExeFile);

        // Get the next processes
        while (Process32Next(hProcessSnap, &pe32)) {
            // Append the next process to the list with PID
            wcscat_s(msg, bufferSize, L"\n");
            swprintf(msg + wcslen(msg), bufferSize - wcslen(msg), L"%d: %s", pe32.th32ProcessID, pe32.szExeFile);
        }
    }

    CloseHandle(hProcessSnap);
}

// Функція, яка виконує обробку клієнтського запиту
void HandleClient(SOCKET clientSocket) {
    while (true) {
        // Отправляем список процессов
        WCHAR processListData[bufferSize] = { 0 };
        GetProcessList(processListData);

        char processList[bufferSize];
        ConvertWCHARtoChar(processListData, processList, sizeof(processList));
        send(clientSocket, processList, sizeof(processList), NULL);

        // Ожидание запроса от клиента для завершения процесса
        int pid;
        char buffer[255];
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0'; // Добавляем завершающий нуль для создания строки

            // Прочитать строку и вывести её на сервере
            std::cout << "Client sent ID: " << buffer << std::endl;

            pid = std::stoi(buffer);

            // Закрываем процесс и отправляем статус закрытия
            bool isProcessTerminated = TerminateProcess(OpenProcess(PROCESS_TERMINATE, FALSE, pid), 0) != 0;
            send(clientSocket, reinterpret_cast<char*>(&isProcessTerminated), sizeof(isProcessTerminated), 0);

            if (isProcessTerminated) {
                std::cout << "Process " << pid << " closed successfully!\n";
            }
            else {
                std::cout << "Error: failed to terminate process " << pid << std::endl;
            }
        }
    }

    // Закрити з'єднання з клієнтом
    closesocket(clientSocket);
}