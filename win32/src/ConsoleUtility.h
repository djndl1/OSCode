#ifndef CONSOLEUTILITY_H_
#define CONSOLEUTILITY_H_

#include <stdbool.h>
#include <windows.h>
#include <wchar.h>

bool print_strings(HANDLE hOut, ...);

bool print_string(HANDLE hOut, wchar_t const* msg);

bool console_prompt(wchar_t const* prompt_msg, wchar_t* response, DWORD buf_len, bool echo);

#endif // CONSOLEUTILITY_H_
