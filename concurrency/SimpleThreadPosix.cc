#include <iostream>
#include <cstdio>
#include <cstdint>
#include <cinttypes>
#include <memory>

#include <pthread.h>
#include <cstring>

using std::cout;
using std::cerr;
using std::string;

static void print_error(const char *message, int errcode, const char *filename, int linenum)
{
    std::cerr << message << " failed at line " << linenum << " in " << filename
               << " with error " << errcode  << ":" << ::strerror(errcode);

    ::exit(errcode);
}

void *run_simple_thread(void *args)
{
    uintptr_t id = reinterpret_cast<uintptr_t>(args);
    std::string msg = "Thread " + std::to_string(id) + " Running\n";
    std::cout << msg;

    return args;
}

int main()
{
    const int n_threads = 10;

    pthread_t thread_handles[n_threads];

    for (int i = 1; i <= n_threads; i++) {
        uintptr_t id = i;
        pthread_t thread_handle;
        int status = ::pthread_create(&thread_handle, nullptr,
                                      run_simple_thread, reinterpret_cast<void*>(id));
        if (status != 0) {
            ::print_error("_beginthreadex failed at", status, __FILE__, __LINE__);
        }
        thread_handles[i-1] = thread_handle;
    }

    for (pthread_t handle : thread_handles) {
        void *retval = nullptr;
        if (int status = 0; (status = ::pthread_join(handle, &retval)) != 0) {
            print_error("WaitForMultipleObjects failed at", status, __FILE__, __LINE__);
        }
        std::cout << "Thread " + std::to_string(reinterpret_cast<uintptr_t>(retval)) + " Exited.\n";
    }
    // pthread_exit(0); // the main thread exits but the process continues until the final thread exits

    return 0;
}
