#include <windows.h>
#include <stdio.h>

#define SHM_NAME TEXT("MySharedMemory1")
#define EVENT_NAME TEXT("MySyncEvent1")

int main()
{
    printf("PROGRAM 1: Creating shared memory...\n");

    // 1. Создание разделяемой памяти (256 байт)
    HANDLE hMapFile = CreateFileMapping(
        (HANDLE)-1,
        NULL,
        PAGE_READWRITE,
        0,
        256,
        SHM_NAME
    );

    if (!hMapFile) {
        printf("Error CreateFileMapping: %d\n", GetLastError());
        return 1;
    }

    LPVOID pBuf = MapViewOfFile(
        hMapFile,
        FILE_MAP_ALL_ACCESS,
        0, 0,
        256
    );

    if (!pBuf) {
        printf("Error MapViewOfFile: %d\n", GetLastError());
        return 1;
    }

    printf("PROGRAM 1: Shared memory address = %p\n", pBuf);

    // 2. Создаём событие
    HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, EVENT_NAME);
    if (!hEvent) {
        printf("Error CreateEvent: %d\n", GetLastError());
        return 1;
    }

    printf("PROGRAM 1: Waiting 10 seconds before writing...\n");
    Sleep(10000);

    // 3. Записываем текст в shared memory
    char* message = "Hello from Program 1 after delay!";
    strcpy((char*)pBuf, message);

    printf("PROGRAM 1: Wrote text to shared memory: %s\n", message);

    // 4. Сигнализируем событие
    SetEvent(hEvent);
    printf("PROGRAM 1: Event signaled.\n");

    // ------- ДОПОЛНИТЕЛЬНОЕ ЗАДАНИЕ ---------
    printf("\nPROGRAM 1: Allocating 1000 bytes using VirtualAlloc...\n");

    BYTE* dyn = (BYTE*)VirtualAlloc(NULL, 1000, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!dyn) {
        printf("VirtualAlloc failed! Error: %d\n", GetLastError());
        return 1;
    }

    printf("Allocated block at address: %p\n", dyn);

    // Запись 15 символов с шагом 400 байт
    for (int i = 0; i < 15; i++) {
        size_t offset = i * 400;

        dyn[offset] = 'a' + i;

        printf("Writing '%c' at offset %lld, address = %p\n",
               'a' + i,
               (long long)offset,
               dyn + offset);
    }

    printf("PROGRAM 1 finished.\n");

    return 0;
}
