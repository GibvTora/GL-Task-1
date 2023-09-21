#include "ApiS.h"
#include "NetworkingS.h"
#ifdef linux
#include "signal.h"
#include "cwchar"
#include <limits.h>
#endif

const int PROCESS_LIST_BUFFER_SIZE = 4000;

void ConvertWCHARtoChar(const wchar_t* wideStr, char* charStr, int charStrSize) {    //Конвертація wchar_t в char, так як
    wcstombs(charStr, wideStr, charStrSize);                                         //необхідні функції роботи з процесами працюють з wchar_t, а мережа з char
}

void GetProcessList(wchar_t* msg, int bufferSize) {    //Отримуємо список процессів
    
#ifdef _WIN32    //На віндовс отримуємо список процесів через snapshot, потім записуємо процес один за одним
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        // Unable to create a process snapshot
        return;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);    //Визначили розмір

    msg[0] = L'\0';

    if (Process32First(hProcessSnap, &pe32)) {
        //Записали перший процес
        swprintf(msg, bufferSize, L"%d: %s\n", pe32.th32ProcessID, pe32.szExeFile);

        while (Process32Next(hProcessSnap, &pe32)) {
            //Записали наступні
            wcscat_s(msg, bufferSize, L"\n");
            swprintf(msg + wcslen(msg), bufferSize - wcslen(msg), L"%d: %s", pe32.th32ProcessID, pe32.szExeFile);
        }
    }

    CloseHandle(hProcessSnap);
#else
    //Варіант для лінукс ми ітеруємо по директорії /proc і записуємо інформацію
    DIR* dp = opendir("/proc");
    if (!dp) {
        //Неможливо вікрити директорію
        return;
    }

    struct dirent* dirp;    //вказівник на структуру для представлення записів в каталозі
    while ((dirp = readdir(dp)) != NULL) {
        if (dirp->d_type == DT_DIR) {    
            //Будуємо шлях до cmdline
            char path[PATH_MAX];
            snprintf(path, sizeof(path), "/proc/%s/cmdline", dirp->d_name);
            
            FILE* fp = fopen(path, "r");    //відкриваємо
            if (fp) {                       //якщо відкрилось то пишемо в cmd
                char cmd[255];
                fgets(cmd, sizeof(cmd), fp);
                fclose(fp);
                if (strlen(cmd) > 0) {      //якщо не 0 то пишемо в msg
                    //Записуємо інформацію про процес у msg
                    swprintf(msg + wcslen(msg), bufferSize - wcslen(msg), L"%d: %s%s", atoi(dirp->d_name), cmd, wcslen(msg) > 0 ? L"\n" : L"");
                }
            }
        }
    }

    closedir(dp);
#endif
}

void HandleClient(SOCKET clientSocket) {
    while (true) {
        wchar_t processListData[PROCESS_LIST_BUFFER_SIZE] = { 0 };
        GetProcessList(processListData, sizeof(processListData) / sizeof(processListData[0]));    //отримуємо список процесів

        char processList[PROCESS_LIST_BUFFER_SIZE];
        ConvertWCHARtoChar(processListData, processList, sizeof(processList));    //конвертуємо в char для відправки

        send(clientSocket, processList, sizeof(processList), 0);    //відправили до клієнта

        char buffer[255];
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);    //отримуємо від клієнта ID процесу який треба закрити
        if (bytesRead > 0) {                                                  
            buffer[bytesRead] = '\0';                                         

            printf("Client sent ID: %s\n", buffer);                           

            int pid = atoi(buffer);                                           

#ifdef _WIN32    //закриваємо процес для WIN
            bool isProcessTerminated = TerminateProcess(OpenProcess(PROCESS_TERMINATE, FALSE, pid), 0) != 0;
#else            //закриваємо процес для Linux
            bool isProcessTerminated = kill(pid, SIGTERM) == 0;
#endif
                //Відправляємо результат закриття до клієнту
            send(clientSocket, reinterpret_cast<char*>(&isProcessTerminated), sizeof(isProcessTerminated), 0);

            if (isProcessTerminated) {
                printf("Process %d closed successfully!\n", pid);
            }
            else {
                printf("Error: failed to terminate process %d\n", pid);
            }
        }
    }

    CloseSocket(clientSocket);
}
