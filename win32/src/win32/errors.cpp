#include "win32/errors.h"
#include <cwchar>

#include <windows.h>
#include <stdlib.h>
#include <exception>
#include <utility>
#include <cstdint>
#include <string>

#include "modernlib/dyn_cwstr.h"
#include "modernlib/allocator.h"
#include "cwinapi/winerror.h"

std::wstring windows::get_error_message(uint32_t error)
{
    dyn_cwstr_result result = winerror_get_message(WINERROR(error),
                                                   std_allocator);
    if (result.error) {
        throw windows::message_format_error(error, L"Cannot find message for error " + std::to_wstring(error));
    }
    const wchar_t *raw = dyn_cwstr_release(&result.str);
    return std::wstring{std::move(raw)};
}
