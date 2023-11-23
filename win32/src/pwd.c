#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "config.h"

#include <stdbool.h>
#include <stdlib.h>

// two more characters for CRLF
#define DIRNAME_LEN (MAX_PATH + 2)

VOID ReportError(LPCTSTR userMessage, DWORD exitCode, BOOL printErrorMessage);
bool print_strings(HANDLE hOut, ...);

int wmain(int argc, wchar_t* arv[])
{
    wchar_t buffer[DIRNAME_LEN] = { L'\0' };

    DWORD dirlen = GetCurrentDirectoryW(DIRNAME_LEN, buffer);

    if (dirlen == 0) {
        ReportError(L"Fail to get pathname", 1, TRUE);
    }

    if (dirlen > DIRNAME_LEN) {
        ReportError(L"path too long", 2, TRUE);
    }

    print_strings(GetStdHandle(STD_OUTPUT_HANDLE), buffer, L"\n", nullptr);

    return EXIT_SUCCESS;
}
