#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 12
#define READERS 3
#define WRITERS 3
#define MAX_LINES 22

char buffer[BUFFER_SIZE + 1];

HANDLE sem_write;
HANDLE sem_read;

CRITICAL_SECTION cs_console;

const char *cities[WRITERS] = {
    "Novosibirsk",
    "Semipalatink",
    "Ekaretinburg"
};

int reader_lines[READERS] = {0};

void rand_sleep(int max_ms)
{
    Sleep(rand() % max_ms);
}


DWORD WINAPI writer_thread(LPVOID param)
{
    int id = (int)(intptr_t)param;
    const char *text = cities[id];
    int len = strlen(text);
    int half = len / 2;

    while (1)
    {
        WaitForSingleObject(sem_write, INFINITE);

        memset(buffer, 0, BUFFER_SIZE + 1);


        memcpy(buffer, text, half);


        ReleaseSemaphore(sem_read, READERS, NULL);

        rand_sleep(1000);


        memcpy(buffer + half, text + half, len - half);

        ReleaseSemaphore(sem_read, READERS, NULL);

        ReleaseSemaphore(sem_write, 1, NULL);

        Sleep(2000 + rand() % 1000);
    }
}


DWORD WINAPI reader_thread(LPVOID param)
{
    int id = (int)(intptr_t)param;
    int column = id * 20;

    while (reader_lines[id] < MAX_LINES)
    {
        rand_sleep(300);

        WaitForSingleObject(sem_read, INFINITE);

        char local[BUFFER_SIZE + 1];
        strncpy(local, buffer, BUFFER_SIZE);
        local[BUFFER_SIZE] = 0;

        EnterCriticalSection(&cs_console);
        COORD pos = { (SHORT)column, (SHORT)reader_lines[id] };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
        printf("%-12s", local);
        LeaveCriticalSection(&cs_console);

        reader_lines[id]++;
    }
    return 0;
}


int main()
{
    srand((unsigned)time(NULL));

    InitializeCriticalSection(&cs_console);

    sem_write = CreateSemaphore(NULL, 1, 1, NULL);
    sem_read  = CreateSemaphore(NULL, 0, 100, NULL);

    HANDLE writers[WRITERS];
    HANDLE readers[READERS];

    for (int i = 0; i < WRITERS; i++)
        writers[i] = CreateThread(NULL, 0, writer_thread,
                                  (LPVOID)(intptr_t)i, 0, NULL);

    for (int i = 0; i < READERS; i++)
        readers[i] = CreateThread(NULL, 0, reader_thread,
                                  (LPVOID)(intptr_t)i, 0, NULL);

    WaitForMultipleObjects(READERS, readers, TRUE, INFINITE);
    return 0;
}
