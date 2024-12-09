#include "winhandle.h"

const winhandle invalid_winhandle = (winhandle){ .handle = INVALID_HANDLE_VALUE };

bool winhandle_close(winhandle handle)
{
    return CloseHandle(handle.handle);
}


winerror last_error()
{
    DWORD err = GetLastError();

    return (winerror) { .code = err };
}
