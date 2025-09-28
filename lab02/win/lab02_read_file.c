#include <windows.h>
#include <stdio.h>

int main(){
    HANDLE handle_file1, handle_file2, handle_file3;

    char buffer1[7], buffer2[7], buffer3[7];

    handle_file1 = CreateFile("testfile.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (handle_file1 == INVALID_HANDLE_VALUE){
        printf("Opening file error");
        return 1;
    }

    if (!DuplicateHandle(GetCurrentProcess(), handle_file1, GetCurrentProcess(), &handle_file2, 0, FALSE, DUPLICATE_SAME_ACCESS)){
        printf("Duplicate error");
        return 1;
    }

    handle_file3 = CreateFile("testfile.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle_file3 == INVALID_HANDLE_VALUE){
        printf("Opening file error");
        return 1;
    }

    printf("hFile1 = %p\nhFile2 = %p\nhFile3 = %p\n", handle_file1, handle_file2, handle_file3);

    DWORD bytes_read;
    ReadFile(handle_file1, buffer1, 7, &bytes_read, NULL);
    ReadFile(handle_file2, buffer2, 7, &bytes_read, NULL);
    ReadFile(handle_file3, buffer3, 7, &bytes_read, NULL);

    printf("handle_file1: %s\n", buffer1);
    printf("handle_file2: %s\n", buffer2);
    printf("handle_file3: %s\n", buffer3);

    CloseHandle(handle_file1);
    CloseHandle(handle_file2);
    CloseHandle(handle_file3);

    return 0;

}
