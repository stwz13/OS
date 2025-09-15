#include <unistd.h>
#include <string.h>

int main(){
    char buffer[256];
    ssize_t bytes_read;
    int stdin_is_tty = isatty(STDIN_FILENO);
    int stdout_is_tty = isatty(STDOUT_FILENO);

    if (stdin_is_tty){
        const char* prompt_to_input_message = "Input your message: ";
        if (stdout_is_tty) {
            write(STDOUT_FILENO, prompt_to_input_message,
                 strlen(prompt_to_input_message));
        } else {
            write(STDERR_FILENO, prompt_to_input_message,
                 strlen(prompt_to_input_message));
        }
    }

    bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer)-1);
    buffer[bytes_read] = '\n';

    const char* output_message = "Your input: ";
    write(STDOUT_FILENO, output_message, strlen(output_message));
    write(STDOUT_FILENO, buffer, bytes_read);

    return 0;
}