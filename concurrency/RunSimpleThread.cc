#include "SimpleThread.hh"

#include <iostream>
#include <utility>
#include <memory>

class SimpleRunnable : public Runnable
{
    public:
        SimpleRunnable(int id) : m_id{id} { }

        virtual void *run()
        {
            std::cout << "Thread " + std::to_string(m_id) + " is running\n";

            return reinterpret_cast<void*>(static_cast<uintptr_t>(m_id));
        }

    private:
        int m_id;
};

class SimpleThread : public Thread
{
    private:
        int m_id;
    public:
        SimpleThread(int id) : m_id{id}
        { }

    private:
        virtual void *run()
        {
            std::cout << "Thread " + std::to_string(m_id) + " is running\n";

            return reinterpret_cast<void*>(static_cast<uintptr_t>(m_id));
        }
};

int main()
{
    std::unique_ptr<Runnable> r{new SimpleRunnable(1)};
    std::unique_ptr<Thread> th1{new Thread(std::move(r))};
    th1->start();

    SimpleThread th2{2};
    th2.start();

    uintptr_t result1 = reinterpret_cast<uintptr_t>(th1->join());
    uintptr_t result2 = reinterpret_cast<uintptr_t>(th2.join());

    std::cout << "Thread 1 exited with " + std::to_string(result1) + '\n'
              << "Thread 2 exited with " + std::to_string(result2) + '\n';

    return 0;
}
