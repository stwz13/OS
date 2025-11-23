#include <windows.h>
#include <stdio.h>

#define SHM_NAME TEXT("MySharedMemory1")
#define EVENT_NAME TEXT("MySyncEvent1")

int main()
{
    printf("PROGRAM 2: Opening shared memory...\n");

    HANDLE hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,
        FALSE,
        SHM_NAME
    );

    if (!hMapFile) {
        printf("Error OpenFileMapping: %d\n", GetLastError());
        return 1;
    }

    LPVOID pBuf = MapViewOfFile(
        hMapFile,
        FILE_MAP_ALL_ACCESS,
        0, 0,
        256
    );

    if (!pBuf) {
        printf("Error MapViewOfFile\n");
        return 1;
    }

    printf("PROGRAM 2: Shared memory address = %p\n", pBuf);

    printf("PROGRAM 2: Initial content: \"%s\"\n", (char*)pBuf);

    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, EVENT_NAME);
    if (!hEvent) {
        printf("Error OpenEvent: %d\n", GetLastError());
        return 1;
    }

    printf("PROGRAM 2: Waiting for event...\n");


    WaitForSingleObject(hEvent, INFINITE);

    printf("PROGRAM 2: Event received. New memory content: \"%s\"\n", (char*)pBuf);

    return 0;
}
