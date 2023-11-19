#define WINVER 0x0501
#define _WIN32_WINNT 0x0501

#include "config.h"
#include <unistd.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <windows.h>

#define BUFSIZE 4096


static uint8_t buffer[BUFSIZE];

VOID ReportError(LPCTSTR userMessage, DWORD exitCode, BOOL printErrorMessage);

static void cat_file(HANDLE in_file, HANDLE out_file)
{
    DWORD bytes_read = 0;
    DWORD bytes_written = 0;
    while (ReadFile(in_file, buffer, BUFSIZE, &bytes_read, nullptr)
           && (bytes_read != 0)
           && WriteFile(out_file, buffer, bytes_read, &bytes_written, nullptr));
}

wchar_t *char_str_to_wchar(const char *cstr)
{
    if (cstr == NULL) return NULL;

    size_t len = strlen(cstr);
    if (len == 0) return L"";

    wchar_t *wstr = malloc(sizeof(wchar_t) * (len + 1));
    if (wstr == NULL) {
        return NULL;
    }

    size_t converted_wchar_count = mbstowcs(wstr, cstr, len + 1);

    return wstr;
}

int main(int argc, char* argv[])
{
    HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

    bool dash_s = false; // whether not to show error

    int c = getopt(argc, argv, "s");
    if (c == 's') {
        dash_s = true;
    }

    if (optind == argc) {
        cat_file(hstdin, hstdout);

        return EXIT_SUCCESS;
    }
    for (int farg = optind; farg < argc; farg++) {
        wchar_t *in_file_name = char_str_to_wchar(argv[farg]);
        HANDLE in_file = CreateFile(in_file_name, GENERIC_READ, 0,
                                    nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (in_file == INVALID_HANDLE_VALUE) {
            if (!dash_s) {
                ReportError(L"Error: file does not exist", 0, true);
            }
        } else {
                cat_file(in_file, hstdout);

                if (GetLastError() != 0 && !dash_s) {
                    ReportError(L"Cat Error.", 0, true);

                    goto close_in;
                }
        }

close_in:
        if (in_file != INVALID_HANDLE_VALUE) {
            CloseHandle(in_file);
        }
    }

    return EXIT_SUCCESS;
}
