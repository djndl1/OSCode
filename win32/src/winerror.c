#include "cwinapi/winerror.h"
#include "modernlib/dyn_cwstr.h"

winerror last_error()
{
    DWORD err = GetLastError();

    return (winerror) { .code = err };
}
