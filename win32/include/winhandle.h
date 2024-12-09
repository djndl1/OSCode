#ifndef WINHANDLE_H_
#define WINHANDLE_H_

#include <windows.h>

#include <wchar.h>
#include <stdbool.h>

typedef struct winhandle {
    HANDLE handle;
} winhandle;

#define WINHANDLE(h) ((winhandle){ .handle = h })

extern const winhandle invalid_winhandle;

typedef struct winerror {
    DWORD code;
} winerror;

#define WINERROR(e) ((winerror) { .code = e })

static inline bool is_handle_valid(winhandle h)
{
    return h.handle == invalid_winhandle.handle;
}

typedef struct winstatus {
    winerror error;
    bool succeeded;
} winstatus;

#define WINSTATUS(err, ok) ((winstatus) { .error = err , .succeeded = ok })
#define WIN_OK ((winstatus) { .succeeded = true })
#define WIN_ERR(e) WINSTATUS(e, false)


bool winhandle_close(winhandle handle);

winerror last_error();

#endif // WINHANDLE_H_
