#ifndef SIMPLETHREAD_H_
#define SIMPLETHREAD_H_

#include <memory>

class Runnable
{
    public:
        virtual void *run() = 0;
        virtual ~Runnable() = 0;
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

class Win32Thread
{
    public:
        Win32Thread(std::unique_ptr<Runnable>);

        Win32Thread();

        virtual ~Win32Thread();

        void start();

        void *join();

        const Win32Thread& operator=(const Win32Thread&) = delete;
    private:
        HANDLE m_thread_handle;

        std::unique_ptr<Runnable> m_runnable;

        unsigned m_thread_id;

        Win32Thread(const Win32Thread&);


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


#endif

#endif // SIMPLETHREAD_H_
