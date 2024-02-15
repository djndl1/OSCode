#include "SimpleThread.hh"

#include <iostream>
#include <atomic>
#include <string>

std::atomic_int shared = 0;

class CommunicatingThread : public Thread
{
    public:
        CommunicatingThread(int id) : m_id{id} { }
        virtual void* run()
        {
            std::cout << " Thread " + std::to_string(m_id) + " is running\n";
            for (int i = 0; i < 100'000'000; i++) {
                shared++;
            }

            return 0;
        }
    private:
        int m_id;
};

int main()
{
    CommunicatingThread thr1{1};
    CommunicatingThread thr2{2};
    thr1.start();
    thr2.start();
    thr1.join();
    thr2.join();

    std::cout << " Shared = " << shared  << std::endl;

    return 0;
}
