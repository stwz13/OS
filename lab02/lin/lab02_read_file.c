#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    char h_buffer1[7], h_buffer2[7], h_buffer3[7];

    int desc1 = open("testfile.txt", O_RDONLY);
    if (desc1 < 0){
        printf("Reading file error");
        return 1;
    }

    int desc2 = dup(desc1);

    if (desc2 < 0){
        printf("Reading file error");
        return 1;
    }

    int desc3 = open("testfile.txt", O_RDONLY);
    if (desc3 < 0){
        printf("Reading file error");
        return 1;
    }

    printf("File descriptor values:\nfd1 = %d\nfd2 = %d\nfd3 = %d\n\n", desc1, desc2, desc3);

    int r1 = read(desc1, h_buffer1, 7);
    int r2 = read(desc2, h_buffer2, 7);
    int r3 = read(desc3, h_buffer3, 7);

    printf("С fd1: %s\n", h_buffer1);
    printf("С fd2: %s\n", h_buffer2);
    printf("С fd3: %s\n", h_buffer3);

    close(desc1);
    close(desc2);
    close(desc3);

    return 0;
}