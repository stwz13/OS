#include <windows.h>
#include <stdio.h>

int main(){
    char buffer [1024];
    DWORD bytes_written;

    HANDLE handle_file = CreateFile("testfile.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle_file == INVALID_HANDLE_VALUE){
        printf("Creating file error");
        return 1;
    }

    printf("Enter text\n");
    while (fgets(buffer, sizeof(buffer), stdin)){
        if (!WriteFile(handle_file, buffer, (DWORD)strlen(buffer), &bytes_written, NULL)){
            printf("Writing in file error");
            CloseHandle(handle_file);
            return 1;
        }
        
    }
    
    CloseHandle(handle_file);
    return 0;

}
