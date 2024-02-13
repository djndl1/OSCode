#include "SimpleThread.hh"


#ifdef WIN32_IMPL
#include <iostream>
#include <string>

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

    }
}

void *Win32Thread::join()
{
    // TODO
    return result;
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

#endif
