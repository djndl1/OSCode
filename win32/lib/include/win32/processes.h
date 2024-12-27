#pragma once

#include <windows.h>
#include <stdint.h>

#if defined (__cplusplus)
extern "C" {
#endif

uint32_t create_process(const wchar_t*, const wchar_t**, LPPROCESS_INFORMATION process);

#if defined (__cplusplus)
}
#endif
