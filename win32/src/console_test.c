#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdbool.h>
#include <stdlib.h>

#include "config.h"

int wmain(int argc, wchar_t* argv[])
{
    wchar_t buffer[10];

    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

    SetConsoleMode(hIn, ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT);

    DWORD chars_read = -1;
    ReadConsoleW(hIn, buffer, 10, &chars_read, nullptr);

    DWORD chars_written = -1;
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE),
                  buffer, chars_read, &chars_written, nullptr);

    return EXIT_SUCCESS;
}
