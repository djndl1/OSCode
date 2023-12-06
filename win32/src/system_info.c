#include <stdbool.h>
#include <stdint.h>
#include <windows.h>

#include <wchar.h>

int wmain(int argc, wchar_t *argv[])
{
    SYSTEM_INFO sysInfo = { 0 };
    GetSystemInfo(&sysInfo);

    wprintf(L"Processesor Architecture %d\n"
            L"PageSize %d\n"
            L"Address ranging from %xd to %xd\n"
            L"Number of processor %d\n"
            L"Processor type %d\n"
            L"Processor Revision %d\n"
            L"Allocation granularity %d\n",
            sysInfo.wProcessorArchitecture,
            sysInfo.dwPageSize,
            sysInfo.lpMinimumApplicationAddress, sysInfo.lpMaximumApplicationAddress,
            sysInfo.dwNumberOfProcessors,
            sysInfo.dwProcessorType,
            sysInfo.wProcessorRevision,
            sysInfo.dwAllocationGranularity);
}
