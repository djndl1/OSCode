// use C++ just for some C99 features

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>

#include "ReportError.h"

void str_toupper_inplace(char* str, size_t len)
{
    if (str == nullptr || len == 0) return;

    for (size_t idx = 0; idx < len; idx++) {
        if (str[idx] < 0 || str[idx] > 0xff) continue;
        if (isalpha(str[idx])) {
            str[idx] = toupper(str[idx]);
        }
    }
}

void file_toupper(wchar_t const* in_filename, wchar_t const* out_filename)
{
    HANDLE hIn = INVALID_HANDLE_VALUE;
    HANDLE hOut = INVALID_HANDLE_VALUE;
    char* buffer = nullptr;
    __try {
        hIn = CreateFileW(in_filename, GENERIC_READ,
                          0, nullptr, OPEN_EXISTING, 0, nullptr);
        if (hIn == INVALID_HANDLE_VALUE) {
            ReportException(in_filename, 1);
        }

        LARGE_INTEGER file_size;
        file_size.QuadPart = -1;
        if (!GetFileSizeEx(hIn, &file_size) || file_size.HighPart > 0) {
            ReportException(L"This file is too large.", 1);
        }

        hOut = CreateFileW(out_filename,
                           GENERIC_READ | GENERIC_WRITE, 0, nullptr,
                           CREATE_NEW, 0, nullptr);
        if (hOut == INVALID_HANDLE_VALUE) {
            ReportException(out_filename, 1);
        }

        size_t buflen = file_size.LowPart / 10;
        buffer = (char*) malloc(buflen);
        if (buffer == nullptr) {
            ReportException(L"Memory allocation error", 1);
        }

        DWORD data_read_len = -1;
        DWORD data_written_len = -1;
        while (ReadFile(hIn, buffer, buflen, &data_read_len, nullptr)
               && (data_read_len != 0)) {
            str_toupper_inplace(buffer, data_read_len);

            if (!WriteFile(hOut, buffer, data_read_len,
                           &data_written_len, nullptr)
                || (data_written_len != data_read_len)) {
                ReportException(L"Write file error", 1);
            }
        }
    } __finally {
        if (buffer != nullptr) {
            free(buffer);
            buffer = nullptr;
        }
        if (hOut != INVALID_HANDLE_VALUE) {
            CloseHandle(hOut);
            hOut = INVALID_HANDLE_VALUE;
        }
        if (hIn != INVALID_HANDLE_VALUE) {
            CloseHandle(hIn);
            hIn = INVALID_HANDLE_VALUE;
        }
    }
}

int wmain(int argc, wchar_t* argv[])
{
    for (int f_count = 1; f_count < argc; f_count++) {

        wchar_t const* in_filename = argv[f_count];
        wchar_t out_filename[MAX_PATH - 4] = L"";
        __try {
            if (wcslen(in_filename) > MAX_PATH - 10) {
                ReportException(L"The file name is too long.", 1);
            }
            //TODO handle path prefix
            swprintf(out_filename, MAX_PATH - 4, L"UC_%s", argv[f_count]);

            file_toupper(in_filename, out_filename);
        } __except (EXCEPTION_EXECUTE_HANDLER) {
            wprintf(L"Error processing file %s\n", argv[f_count]);

            DeleteFile(out_filename);
        }
    }

    wprintf(L"All files converted, except as noted above\n");

    return 0;
}
