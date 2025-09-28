#include <windows.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINES 1024

int main() {
    INPUT_RECORD ir;
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD prevMode;

    // Включаем ввод мыши
    GetConsoleMode(hIn, &prevMode);
    SetConsoleMode(hIn, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    // Чтение файла
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        printf("Opening file is failed.\n");
        return 1;
    }

    char line[1024];
    int lineCount = 0;
    int lineLengths[MAX_LINES] = {0};  // длины каждой строки

    while (fgets(line, sizeof(line), file)) {
        int len = (int)strlen(line);
        if (line[len-1] == '\n') {
            line[len-1] = '\0';
            len--;
        }
        lineLengths[lineCount] = len;
        printf("%s\n", line);
        lineCount++;
    }
    fclose(file);

    // Получаем размер буфера консоли
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    int logLine = csbi.dwCursorPosition.Y + 1; // где выводить информацию о кликах

    DWORD count;
    char ch;
    while (1) {
        ReadConsoleInput(hIn, &ir, 1, &count);
        if (ir.EventType != MOUSE_EVENT) continue;

        MOUSE_EVENT_RECORD mer = ir.Event.MouseEvent;

        // Правый клик — выход
        if ((mer.dwButtonState & RIGHTMOST_BUTTON_PRESSED) && mer.dwEventFlags == 0)
            break;

        // Левый клик
        if ((mer.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)) {
            COORD pos = mer.dwMousePosition;

            // Проверяем, что клик внутри текста
            if (pos.Y < lineCount && pos.X < lineLengths[pos.Y]) {
                DWORD read;
                if (ReadConsoleOutputCharacter(hOut, &ch, 1, pos, &read) && ch != ' ' && ch != '\0') {
                    COORD writePos = {0, (SHORT)logLine};
                    SetConsoleCursorPosition(hOut, writePos);
                    printf("Symbol: %c  Row: %d  Column: %d\n", ch, pos.Y, pos.X);
                    logLine++;
                }
            }
        }
    }

    SetConsoleMode(hIn, prevMode);
    return 0;
}
