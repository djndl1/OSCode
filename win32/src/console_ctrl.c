#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <signal.h>

static volatile sig_atomic_t exit_flag = 0;

static BOOL WINAPI Handler(DWORD cntrlEvent)
{
    exit_flag = 1;

    switch (cntrlEvent) {
        case CTRL_C_EVENT:
            wprintf(L"Ctrl-C Leaving in <= 5 seconds\n");
            Sleep(4000);
            wprintf(L"Leaving handler in 1 second or less\n");
            return TRUE;
        case CTRL_CLOSE_EVENT:
            wprintf(L"Close event. Leaving in <= 5 seconds\n");
            Sleep(4000);
            wprintf(L"Leaving handler in 1 second or less\n");
            return TRUE;
        default:
            wprintf(L"Event %d. Leaving in <= 5 seconds\n", cntrlEvent);
            Sleep(4000);
            wprintf(L"Leaving handler in 1 second or less\n");
            return TRUE;
    }
}

int wmain(int argc, wchar_t* argv[])
{
    SetConsoleCtrlHandler(Handler, TRUE);

    while (!exit_flag) {
        Sleep(5000);
        Beep(1000, 250);
    }

    wprintf(L"Stopping the program as requested\n");
    return EXIT_SUCCESS;
}
