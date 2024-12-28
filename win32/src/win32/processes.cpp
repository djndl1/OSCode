#include "win32/processes.h"

#include <vector>
#include <cwchar>
#include <cstdint>
#include <string>
#include <numeric>

#include <windows.h>

using std::vector;
using std::wstring;
using std::accumulate;

uint32_t create_process(
    const wchar_t *application_name,
    const wchar_t **args,
    LPPROCESS_INFORMATION process)
{
    std::vector<wstring> arg_list = {};
    for (const wchar_t** cur_arg = args; cur_arg != nullptr && *cur_arg != nullptr; cur_arg++) {
        std::wstring arg_string{*cur_arg};
        arg_list.push_back(*cur_arg);
    }

    std::wstring args_string =  accumulate(arg_list.begin(), arg_list.end(), wstring{L" "});
    wchar_t *args_p = args_string.size() == 0 ? nullptr : &args_string[0];

    STARTUPINFOW startup_info = STARTUPINFOW { sizeof(STARTUPINFO), 0 };
    BOOL succeeded = CreateProcessW(
        application_name,
        args_p,
        nullptr,
        nullptr,
        FALSE,
        0,
        nullptr,
        nullptr,
        &startup_info,
        process);

    if (succeeded) {
        return 0;
    }
    else {
        return ::GetLastError();
    }
}
