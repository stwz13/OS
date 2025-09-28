#include <stdio.h>
#include <stdlib.h>

int main(){
    FILE *file = fopen("testfile.txt", "w");
    char buffer[1024];


    if (!file){
        printf("%s\n", "Creating file error");
        return 1;
    }

    printf("Enter text\n");
    while (fgets(buffer, sizeof(buffer), stdin)){
        fputs(buffer, file);
    }

    fclose(file);

    return 0;

}
