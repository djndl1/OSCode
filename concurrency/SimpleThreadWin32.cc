#include <iostream>
#include <cstdio>
#include <cstdint>
#include <cinttypes>
#include <windows.h>
#include <process.h>

using std::cout;

static void print_error(const wchar_t *message, const char *filename, int linenum)
{

}

__stdcall unsigned run_simple_thread(void *args)
{
    //std::cout << "Thread " << reinterpret_cast<uintptr_t>(args) << " Running\n";

    //std::cout << "Thread " << (args) << " Running\n";
    uintptr_t id = reinterpret_cast<uintptr_t>(args);
    //::printf("Thread %" PRIuPTR " running\n", id);
    ::printf("Thread %d running\n", (int)id);

    return 0;
}

int main()
{
    const int n_threads = 10;

    HANDLE thread_handles[n_threads];

    for (int i = 1; i <= n_threads; i++) {
        uintptr_t id = i;
        uintptr_t thread_handle = ::_beginthreadex(nullptr, 0,
                                   run_simple_thread, reinterpret_cast<void*>(id),
                                   0, nullptr);
        if (thread_handle == 0) {
            ::print_error(L"_beginthreadex failed at", __FILE__, __LINE__);
        }
        thread_handles[i-1] = reinterpret_cast<HANDLE>(thread_handle);

    }
    DWORD rc = ::WaitForMultipleObjects(n_threads, thread_handles, TRUE, INFINITE);

    for (HANDLE handle : thread_handles) {
        CloseHandle(handle);
    }

    return 0;
}
