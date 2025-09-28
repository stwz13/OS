#include <windows.h>
#include <stdio.h>
#include <string.h>

void print_colored_middle_text(const char* text, WORD color){
    CONSOLE_SCREEN_BUFFER_INFO console_screen_info;

    HANDLE out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(out_handle, &console_screen_info);

    int console_width = console_screen_info.srWindow.Right - console_screen_info.srWindow.Left + 1;
    int text_len = (int)strlen(text);
    int position_of_text = (console_width - text_len) / 2;

    COORD position;
    position.X = position_of_text;
    position.Y = console_screen_info.dwCursorPosition.Y;
    SetConsoleCursorPosition(out_handle, position);

    SetConsoleTextAttribute(out_handle, color);

   
    printf("%s\n", text);

    SetConsoleTextAttribute(out_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

}

int main(){
    HANDLE input_file_handle;

    const char* file_name = "input.txt";

    while (1){

        input_file_handle = CreateFileA(
            file_name,
            GENERIC_READ,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

        if (input_file_handle == INVALID_HANDLE_VALUE){

            DWORD error = GetLastError();

            if (error == ERROR_FILE_NOT_FOUND){
                print_colored_middle_text("File is not founded", FOREGROUND_RED | FOREGROUND_INTENSITY);
                return 1;
            }

            else if (error == ERROR_SHARING_VIOLATION){
                print_colored_middle_text("The file is locked by another process", FOREGROUND_BLUE);
                Sleep(2000);

            }
            else{
                print_colored_middle_text("An error occurred while opening the file", FOREGROUND_RED | FOREGROUND_INTENSITY);
                return 1;
            }
        }
        else{
            print_colored_middle_text("File opened successfully", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;

        }
    }

    DWORD file_size = GetFileSize(input_file_handle, NULL);

    if (file_size == INVALID_FILE_SIZE){

        print_colored_middle_text("Error reading file size", FOREGROUND_RED | FOREGROUND_INTENSITY);
        CloseHandle(input_file_handle);

        return 1;

    }

    char* buffer = (char *)malloc(file_size + 1);
    DWORD readed_bytes;

    if (ReadFile(input_file_handle, buffer, file_size, &readed_bytes, NULL)){
        buffer[readed_bytes] = '\0';
        print_colored_middle_text("Text in file: ", FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
        print_colored_middle_text(buffer, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
    }
    else{
        print_colored_middle_text("Error reading file", FOREGROUND_RED | FOREGROUND_INTENSITY);
    }
    
    free(buffer);
    CloseHandle(input_file_handle);
    return 0;

}
