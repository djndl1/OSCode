#include <iostream>
#include <cstdio>
#include <cstdint>
#include <cinttypes>
#include <windows.h>
#include <process.h>
#include <memory>

using std::cout;
using std::wcerr;
using std::wstring;

template <typename T>
struct LocalFreeDeleter
{
    public:
        void operator()(T *obj) const
        {
            ::LocalFree(obj);
        }
};

static void print_error(const wchar_t *message, const char *filename, int linenum)
{
    DWORD err_code = ::GetLastError();

    wchar_t *errorMessage = nullptr;
    ::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                     nullptr, err_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                     (LPWSTR)&errorMessage, 0, nullptr);
    std::unique_ptr<wchar_t, LocalFreeDeleter<wchar_t>> err_msg_ptr(errorMessage);

    std::wcerr << message << L" failed at line " << linenum << L" in " << filename
               << L" with error " << err_code << L":" << err_msg_ptr.get();

    ::exit(err_code);
}

__stdcall unsigned run_simple_thread(void *args)
{
    uintptr_t id = reinterpret_cast<uintptr_t>(args);
    std::string msg = "Thread " + std::to_string(id) + " Running\n";
    std::cout << msg;

    return static_cast<unsigned>(id);
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
    if (rc < WAIT_OBJECT_0 || rc > (WAIT_OBJECT_0 + n_threads)) {
        print_error(L"WaitForMultipleObjects failed at", __FILE__, __LINE__);
    }

    for (HANDLE handle : thread_handles) {
        DWORD exit_code;
        if (!GetExitCodeThread(handle, &exit_code)) {
            print_error(L"GetExitCodeThread failed at", __FILE__, __LINE__);
        }
        std::cout << "Thread " + std::to_string(exit_code) + " Exited.\n";
        CloseHandle(handle);
    }

    return 0;
}
