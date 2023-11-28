#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <float.h>

#include "ReportError.h"


int ExceptFilter(LPEXCEPTION_POINTERS exception, int* eCategory)
{
    DWORD exceptCode = exception->ExceptionRecord->ExceptionCode;

    wprintf(L"Filter exception code: %x\n", exceptCode);

    if ((0x20000000 & exceptCode) != 0) {
        *eCategory = 10;
        return EXCEPTION_EXECUTE_HANDLER;
    }

    switch (exceptCode) {
        case EXCEPTION_ACCESS_VIOLATION:
            wprintf(L"Access Violation. Read/Write/Exec: %d. Address: %x\n",
                    exception->ExceptionRecord->ExceptionInformation[0],
                    exception->ExceptionRecord->ExceptionInformation[1]);
            *eCategory = 1;
            break;
        case EXCEPTION_INT_DIVIDE_BY_ZERO:
        case EXCEPTION_INT_OVERFLOW:
            *eCategory = 2;
            return EXCEPTION_EXECUTE_HANDLER;
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        case EXCEPTION_FLT_OVERFLOW:
            wprintf(L"Floating-point exception - large result\n");
            *eCategory = 3;
            _clearfp();
            return EXCEPTION_EXECUTE_HANDLER;

        default:
            *eCategory = 0;
            return EXCEPTION_CONTINUE_SEARCH;
    }
}

double x = 1.0;
double y = 0.0;

int wmain(int argc, wchar_t* argv[])
{
    bool done = false;

    unsigned int fpOld = _controlfp(0, 0);

    unsigned int fpNew = fpOld &
        ~(EM_OVERFLOW | EM_UNDERFLOW | EM_INEXACT
          | EM_ZERODIVIDE | EM_DENORMAL | EM_INVALID);
    _controlfp(fpNew, MCW_EM);

    while (!done) __try {
        wprintf(L"Enter exception type: ");
        wprintf(L"1: Mem, 2: Int, 3: Float, 4: User 5: __leave");

        int i = 0;
        wscanf(L"%d", &i);

        int eCategory = -1;
        __try {
            switch (i) {
                case 1:
                {
                    int* null = nullptr;
                    int ix = *null;
                    *null = 5;
                }
                break;
                case 2:
                {
                    int iy = 0;
                    int ix = 0;

                    ix = ix / iy;
                }
                break;
                case 3:
                {

                    x = x / y;
                    wprintf(L"x = %20e\n", x);
                }
                break;
                case 4:
                {
                    ReportException(L"User exception", 1);
                }
                break;
                case 5:
                    __leave;
                    break;
                default:
                    done = true;
            }
        } __except (ExceptFilter(GetExceptionInformation(), &eCategory)) {
            switch (eCategory) {
                case 0:
                    wprintf(L"Unknown exception\n");
                    break;
                case 1:
                    wprintf(L"Memory reference exception\n");
                    break;
                case 2:
                    wprintf(L"Integer exception\n");
                    break;
                case 3:
                    wprintf(L"Floating-point exception\n");
                    break;
                case 10:
                    wprintf(L"User exception\n");
                    break;
                default:
                    wprintf(L"Unknown exception\n");
            }
            wprintf(L"End of handler\n");
        }
    } __finally {
        wprintf(L"Abnormal termination?: %d\n", AbnormalTermination());
    }

    _controlfp(fpOld, 0xFFFFFFFF);

    return 0;
}
