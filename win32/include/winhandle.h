#ifndef WINHANDLE_H_
#define WINHANDLE_H_

#include <windows.h>

#include "winerror.h"

#include <wchar.h>
#include <stdbool.h>

typedef struct winhandle {
    HANDLE handle;
} winhandle;

#define WINHANDLE(h) ((winhandle){ .handle = h })

extern const winhandle invalid_winhandle;

static inline bool winhandle_invalid(winhandle self)
{
    return self.handle == invalid_winhandle.handle;
}

static inline bool winhandle_close(winhandle self)
{
    return CloseHandle(self.handle);
}

#if _WIN32_WINNT >= _WIN32_WINNT_WIN10
static inline bool winhandle_same_kernel_object(winhandle self, winhandle second)
{
    return CompareObjectHandles(self.handle, second.handle);
}
#endif

typedef struct handle_info_result {
    winstatus status;
    bool inheritable;
    bool protected_from_close;
} handle_info_result;

handle_info_result winhandle_info(winhandle handle);

winstatus winhandle_set_inheritable(winhandle self, bool inheritable);

winstatus winhandle_set_protected_from_close(winhandle self, bool inheritable);

#endif // WINHANDLE_H_
