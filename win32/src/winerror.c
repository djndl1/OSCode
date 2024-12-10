#include "winerror.h"

winerror last_error()
{
    DWORD err = GetLastError();

    return (winerror) { .code = err };
}
