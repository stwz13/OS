#include <windows.h>
#include <stdio.h>
#include <string.h>

#define MAX_COUNT_LINES 1024

int main(){
    HANDLE input_handle = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE output_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD current_console_mode;
    INPUT_RECORD input_record;

    GetConsoleMode(input_handle, &current_console_mode);
    SetConsoleMode(input_handle, ~ENABLE_QUICK_EDIT_MODE & (ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS));

    FILE *file = fopen("input.txt", "r");
    if (!file){
        printf("Opening file is failed\n");
        return 1;
    }

    char line_buffer[1024];
    int line_count = 0;
    int line_lengths[MAX_COUNT_LINES] = {0};

    while (fgets(line_buffer, sizeof(line_buffer), file)){
        int curr_len = (int)strlen(line_buffer);
        if (line_buffer[curr_len - 1] == '\n'){
            line_buffer[curr_len-1] = '\0';
            curr_len--;

        } 

        line_lengths[line_count] = curr_len;
        printf("%s\n", line_buffer);
        line_count++;

    }

    fclose(file);

    CONSOLE_SCREEN_BUFFER_INFO console_inf;
    GetConsoleScreenBufferInfo(output_handle, &console_inf);
    int message_line = console_inf.dwCursorPosition.Y + 1;
    DWORD count;
    
    while (1){
        if (ReadConsoleInput(input_handle, &input_record, 1, &count)){
        if (input_record.EventType != MOUSE_EVENT) continue;
        
        MOUSE_EVENT_RECORD mouse_event = input_record.Event.MouseEvent;

        if (mouse_event.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED){
            char symbol;
            COORD position = mouse_event.dwMousePosition;

            if (position.Y < line_count && position.X < line_lengths[position.Y]){
                DWORD read;
                if (ReadConsoleOutputCharacter(output_handle, &symbol, 1, position, &read) 
                        && symbol != ' '){

                if (islower(symbol)) symbol = toupper(symbol);
                else if (isupper(symbol)) symbol = tolower(symbol);

                SetConsoleTextAttribute(output_handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
                SetConsoleCursorPosition(output_handle, position);
                DWORD written;
                WriteConsole(output_handle, &symbol, 1, &written, NULL);

                SetConsoleTextAttribute(output_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }   
        }

        }

        if (mouse_event.dwButtonState == RIGHTMOST_BUTTON_PRESSED) break;
    }
}

    SetConsoleMode(input_handle, current_console_mode);
    return 0;

}
