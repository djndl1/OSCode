#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "config.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>

VOID ReportError(LPCTSTR userMessage, DWORD exitCode, BOOL printErrorMessage);

bool print_strings(HANDLE hOut, ...)
{
    va_list msg_list;
    va_start(msg_list, hOut);

    bool retval = true;
    wchar_t *msg = nullptr;
    while ((msg = va_arg(msg_list, wchar_t*)) != nullptr) {
        size_t len = wcslen(msg);

        DWORD written_chars = -1;
        DWORD written_bytes = -1;
        if (!WriteConsoleW(hOut, msg, len, &written_chars, nullptr)
            && !WriteFile(hOut, msg, len * sizeof(wchar_t), &written_bytes, nullptr)) {
            retval = false;
            goto cleanup;
        }
    }

cleanup:
    va_end(msg_list);

    return retval;
}

bool print_string(HANDLE hOut, wchar_t const* msg)
{
    return print_strings(hOut, msg, nullptr);
}

bool console_prompt(wchar_t const* prompt_msg, wchar_t* response, DWORD max_char, bool echo)
{
    bool retval = true;
    HANDLE hIn = CreateFileW(L"CONIN$", GENERIC_READ | GENERIC_WRITE, 0,
                             nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (hIn == INVALID_HANDLE_VALUE) {
        retval = false;
        response = nullptr;
        goto ret;
    }

    HANDLE hOut = CreateFileW(L"CONOUT$", GENERIC_READ | GENERIC_WRITE, 0,
                              nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hOut == INVALID_HANDLE_VALUE) {
        retval = false;
        response = nullptr;
        goto close_in;
    }

    DWORD chars_read = -1;
    WINBOOL succeeded =
        SetConsoleMode(hIn, ENABLE_LINE_INPUT | (echo ? ENABLE_ECHO_INPUT : 0) | ENABLE_PROCESSED_INPUT)
        && SetConsoleMode(hOut, ENABLE_LINE_INPUT | ENABLE_PROCESSED_OUTPUT)
        && ReadConsoleW(hIn, response, max_char - 2, &chars_read, nullptr); // 2 characters reserved for \r\l

    if (succeeded) {
        response[chars_read - 2] = L'\0'; // the final two characters are \r\l, discard them.
    } else {
        ReportError(L"console_prompt failure.", 0, TRUE);
    }

close_out:
    if (hOut != INVALID_HANDLE_VALUE) {
        CloseHandle(hOut);
    }
close_in:
    if (hIn != INVALID_HANDLE_VALUE) {
        CloseHandle(hIn);
    }
ret:
    return retval;
}
