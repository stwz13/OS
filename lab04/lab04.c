#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>>
#include <string.h>
#include <time.h>
#include <sys/ioctl.h>


#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

void print_colored_central(const char* text, const char* color){
    struct winsize w_size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w_size);
    int win_wedth = w_size.ws_col;
    int len_text = strlen(text);
    int position = (win_wedth - len_text) / 2;

    printf("%s%*s%s\n", color, position + len_text, text, RESET);
}

int main(){
    int file_open = open
}