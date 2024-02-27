#include "SimpleThread.hh"

#include <iostream>
#include <string>
#include <utility>
#include <cstdint>

#ifdef WIN32_IMPL
#include <process.h>

Win32Thread::Win32Thread(std::unique_ptr<Runnable> runnable)
    : m_runnable{std::move(runnable)}
{
    if (!m_runnable) {
        print_error("WIn32Thread(std::unique_ptr<Runnable>) failed at ",
                    __FILE__, __LINE__);
    }

    uintptr_t handle = _beginthreadex(nullptr, 0, Win32Thread::start_thread_runnable,
                                      static_cast<void*>(this),
                                      CREATE_SUSPENDED, &m_thread_id);
    if (handle == 0) {
        print_error("_beginthreadex failed at ", __FILE__, __LINE__);
    }

    m_thread_handle = reinterpret_cast<HANDLE>(handle);
}

Win32Thread::Win32Thread()
    : m_runnable(nullptr)
{
    uintptr_t handle = _beginthreadex(nullptr, 0, Win32Thread::start_thread,
                                      static_cast<void*>(this),
                                      CREATE_SUSPENDED, &m_thread_id);
    if (handle == 0) {
        print_error("_beginthreadex failed at ", __FILE__, __LINE__);
    }

    m_thread_handle = reinterpret_cast<HANDLE>(handle);
}

__stdcall unsigned Win32Thread::start_thread_runnable(void *args)
{
    Win32Thread *this_thread = static_cast<Win32Thread*>(args);
    this_thread->result = this_thread->m_runnable->run();
    this_thread->set_completed();

    return static_cast<unsigned>(reinterpret_cast<uintptr_t>(this_thread->result));
}

__stdcall unsigned Win32Thread::start_thread(void *args)
{
    Win32Thread *this_thread = static_cast<Win32Thread*>(args);
    this_thread->result = this_thread->run();
    this_thread->set_completed();

    return static_cast<unsigned>(reinterpret_cast<uintptr_t>(this_thread->result));
}

void Win32Thread::start()
{
    DWORD rc = ::ResumeThread(m_thread_handle);
    if (rc == 0) {
        print_error("Resume Thread failed at ", __FILE__, __LINE__);
    }
}

Win32Thread::~Win32Thread()
{
    if (m_thread_id != ::GetCurrentThreadId()) {
        DWORD rc = ::CloseHandle(m_thread_handle);
        if (rc == 0) {
            print_error("CloseHandle failed at", __FILE__, __LINE__);
        }
    }
}

void *Win32Thread::join()
{
    if (m_thread_id != ::GetCurrentThreadId()) {
        DWORD rc = ::WaitForSingleObject(m_thread_handle, INFINITE);
        if (rc != WAIT_OBJECT_0) {
            print_error("WaitForSingleObject failed at ", __FILE__, __LINE__);
        }
        return result;
    }
    throw std::runtime_error("A thread may not join itself");
}

void Win32Thread::set_completed()
{
    // TODO
}

void Win32Thread::print_error(const std::string& msg,
                              const std::string& filename,
                              int linenum)
{
    DWORD err_code = ::GetLastError();

    wchar_t *errorMessage = nullptr;
    ::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                     nullptr, err_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                     (LPWSTR)&errorMessage, 0, nullptr);
    std::unique_ptr<wchar_t, LocalFreeDeleter<wchar_t>> err_msg_ptr(errorMessage);

    std::cerr << msg << " failed at line " << linenum << " in " << filename
               << " with error " << err_code << ":";
    std::wcerr << err_msg_ptr.get() << '\n';

    ::exit(err_code);
}

#else

#include <error.h>
#include <string.h>

PosixThread::PosixThread(std::unique_ptr<Runnable> runnable, bool detached)
    : m_detached{detached}, m_runnable{std::move(runnable)}
{
    if (!m_runnable) {
        print_error("PosixThread::PosixThread(std::unique_ptr<Runnable>, bool) failed at ",
                    EINVAL, __FILE__, __LINE__);
    }
}

PosixThread::PosixThread(bool detached)
    : m_detached{detached}, m_runnable{nullptr}
{ }

PosixThread::~PosixThread()
{ }

void *PosixThread::start_thread_runnable(void *args)
{
    PosixThread *this_thread = static_cast<PosixThread*>(args);
    if (!this_thread->m_runnable) {
        print_error("No runnable: ", ENOTSUP, __FILE__, __LINE__);
    }

    this_thread->result = this_thread->m_runnable->run();
    this_thread->set_completed();

    return this_thread->result;
}

void *PosixThread::start_thread(void *args)
{
    PosixThread *this_thread = static_cast<PosixThread*>(args);
    this_thread->result = this_thread->run();
    this_thread->set_completed();

    return this_thread->result;
}

void PosixThread::start()
{
    pthread_attr_t thread_attr;
    int attr_status = ::pthread_attr_init(&thread_attr);
    if (attr_status != 0) {
        print_error("pthread_attr_init() failed at ", attr_status, __FILE__, __LINE__);
    }

    if (m_detached) {
        int detach_status = ::pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
        if (detach_status != 0) {
            print_error("pthread_attr_setdetachstate() failed at ", detach_status, __FILE__, __LINE__);
        }
    }

    int thread_creation_status = ::pthread_create(&m_thread_handle, &thread_attr,
                                                  m_runnable ? start_thread_runnable : start_thread,
                                                  reinterpret_cast<void*>(this));
    if (thread_creation_status != 0) {
        print_error("pthread_create failed at ", thread_creation_status, __FILE__, __LINE__);
    }
    int attr_destroy_status = ::pthread_attr_destroy(&thread_attr);
    if (attr_destroy_status != 0) {
        print_error("pthread_attr_destroy failed at ", thread_creation_status, __FILE__, __LINE__);
    }
}

void *PosixThread::join()
{
    int status = ::pthread_join(m_thread_handle, nullptr);
    if (status == EDEADLK)
        throw std::runtime_error("A thread may not join itself");
    if (status != 0) {
        print_error("pthread_join failed at", status, __FILE__, __LINE__);
    }

    return result;
}

void PosixThread::set_completed()
{ }


void PosixThread::print_error(const char *message, int errcode, const char *filename, int linenum)
{
    std::cerr << message << " failed at line " << linenum << " in " << filename
               << " with error " << errcode  << ":" << ::strerror(errcode);

    ::exit(errcode);
}

#endif

Thread::Thread(std::unique_ptr<Runnable> runnable)
    : p_thread_impl{new ThreadImpl(std::move(runnable))}
{ }

Thread::Thread()
    : p_thread_impl{new ThreadImpl()}
{}


Thread::~Thread() = default;

void Thread::start()
{
    std::function<void*()> wrapper = std::bind(&Thread::run, this);

    p_thread_impl->setup_run_func(wrapper);
    p_thread_impl->start();
}

void *Thread::join()
{
    return p_thread_impl->join();
}
