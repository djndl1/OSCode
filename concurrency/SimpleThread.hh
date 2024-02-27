#ifndef SIMPLETHREAD_H_
#define SIMPLETHREAD_H_

#include <memory>
#include <functional>
#include <stdexcept>

class ThreadBase
{
    public:
        virtual void start() = 0;

        virtual void *join() = 0;


        virtual ~ThreadBase() { };

    private:
        virtual void *run() { return nullptr; }
};

class Runnable
{
    public:
        virtual void *run() = 0;
        virtual ~Runnable() { };
};

#if defined(_WIN32) || defined(_WIN64) || defined(__MINGW32__) || defined(_MSC_VER)

#define WIN32_IMPL
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

template <typename T>
struct LocalFreeDeleter
{
    public:
        void operator()(T *obj) const
        {
            ::LocalFree(obj);
        }
};

class Win32Thread : public ThreadBase
{
    public:
        Win32Thread(std::unique_ptr<Runnable>);

        Win32Thread();

        virtual ~Win32Thread();

        virtual void start();

        virtual void *join();

        const Win32Thread& operator=(const Win32Thread&) = delete;

        Win32Thread(const Win32Thread&) = delete;
    private:
        HANDLE m_thread_handle;

        std::unique_ptr<Runnable> m_runnable;

        unsigned m_thread_id;

        void set_completed();

        void *result;

        /**
         *  to be derived
         */
        virtual void *run() { return nullptr; }

        __stdcall static unsigned start_thread_runnable(void*);

        __stdcall static unsigned start_thread(void*);

        void print_error(const std::string& msg, const std::string& filename, int linenum);
};

#else

#include <pthread.h>

class PosixThread : public ThreadBase
{
    public:
        PosixThread(std::unique_ptr<Runnable> runnable, bool detached = false);

        PosixThread(bool detached = false);

        virtual ~PosixThread();

        virtual void start();

        virtual void *join();

        const PosixThread& operator=(const PosixThread&) = delete;

        PosixThread(const PosixThread&) = delete;
    private:
        pthread_t m_thread_handle;

        bool m_detached;

        std::unique_ptr<Runnable> m_runnable;

        void set_completed();

        void *result;

        /**
         *  to be derived
         */
        virtual void *run() { return nullptr; }

        static void *start_thread_runnable(void*);

        static void *start_thread(void*);

        static void print_error(const char *message, int errcode, const char *filename, int linenum);
};

#endif

class ThreadImpl : public
#ifdef WIN32_IMPL
    Win32Thread
#else
    PosixThread
#endif
{
    public:
        ThreadImpl(std::unique_ptr<Runnable> runnable) :
#ifdef WIN32_IMPL
    Win32Thread(std::move(runnable))
#else
    PosixThread(std::move(runnable))
#endif
        { }

        ThreadImpl() { }

        void setup_run_func(std::function<void*()> func)
        {
            run_impl = func;
        }
    private:
        std::function<void*()> run_impl;

        virtual void *run()
        {
            if (run_impl) {
                return run_impl();
            } else {
                throw std::runtime_error("Null run_impl");
            }
        }
};

class Thread : public ThreadBase
{
    public:
        Thread(std::unique_ptr<Runnable> runnable);

        Thread();

        virtual void start();

        virtual void *join();

        virtual ~Thread();
    private:
        const std::unique_ptr<ThreadImpl> p_thread_impl;

        virtual void *run() { return nullptr; }
};

#endif // SIMPLETHREAD_H_
