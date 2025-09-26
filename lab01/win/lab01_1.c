#include <windows.h>
#include <stdio.h>
int main(){
    DWORD bytes_read, bytes_written;
    CHAR buffer[256];
    CHAR handles_info[64];

    HANDLE handle_input = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE handle_output = GetStdHandle(STD_OUTPUT_HANDLE);

    if (handle_input == INVALID_HANDLE_VALUE || 
        handle_output == INVALID_HANDLE_VALUE) return 1;

    wsprintf(handles_info, "STDIN handle = %p\nSTDOUT handle = %p\n", handle_input, handle_output);
    WriteFile(handle_output, handles_info, lstrlen(handles_info), &bytes_written, NULL);

    const char* promt_to_input = "Input text\n";
    WriteFile(handle_output, promt_to_input, lstrlen(promt_to_input), &bytes_written, NULL);

    BOOL rc = ReadFile(handle_input, buffer, sizeof(buffer), &bytes_read, NULL);

    if (!rc){
        const char* failed_message = "Failed input";
        WriteFile(handle_output, failed_message, lstrlen(failed_message), &bytes_written, NULL);
        return 1;
    }

    const char* output_message = "Your inputed text:";
    WriteFile(handle_output, output_message, lstrlen(output_message), &bytes_written, NULL);

    WriteFile(handle_output, buffer, bytes_read, &bytes_written, NULL);

    return 0;

}
