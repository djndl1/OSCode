#ifndef WINHANDLE_H_
#define WINHANDLE_H_

#include <windows.h>

#include "winerror.h"

#include <wchar.h>
#include <stdbool.h>
#include "basis.h"

typedef struct winhandle {
    HANDLE handle;
} winhandle;

#define WINHANDLE(h) ((winhandle){ .handle = h })

#define invalid_winhandle  ((winhandle){ .handle = INVALID_HANDLE_VALUE })

static inline bool winhandle_invalid(const winhandle self)
{
    return self.handle == invalid_winhandle.handle;
}

static inline bool winhandle_close(winhandle *self)
{
    if (self == nullptr || winhandle_invalid(*self)) {
        return false;
    }
    bool status = CloseHandle(self->handle);
    if (status) {
        *self = invalid_winhandle;
    }

    return status;
}

#if _WIN32_WINNT >= _WIN32_WINNT_WIN10
static inline bool winhandle_same_kernel_object(const winhandle self, const winhandle second)
{
    return CompareObjectHandles(self.handle, second.handle);
}
#endif

typedef struct winhandle_info_result {
    winstatus status;
    bool inheritable;
    bool protected_from_close;
} winhandle_info_result;

winhandle_info_result winhandle_info(const winhandle handle);

winstatus winhandle_set_inheritable(const winhandle self, bool inheritable);

winstatus winhandle_set_protected_from_close(const winhandle self, bool inheritable);

#endif // WINHANDLE_H_
