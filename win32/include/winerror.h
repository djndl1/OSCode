#ifndef WINERROR_H_
#define WINERROR_H_

#include <windows.h>
#include <stdint.h>

#include <stdbool.h>

typedef struct winerror {
    uint32_t code;
} winerror;

#define WINERROR(e) ((winerror) { .code = e })


typedef struct winstatus {
    winerror error;
    bool succeeded;
} winstatus;

#define WINSTATUS(err, ok) ((winstatus) { .error = err , .succeeded = ok })
#define WIN_OK ((winstatus) { .succeeded = true })
#define WIN_ERR(e) WINSTATUS(e, false)
#define WIN_LASTERR WIN_ERR(last_error())

winerror last_error();

#endif // WINERROR_H_
