#include <iostream>
#include <semaphore.h>
#include <pthread.h>

namespace SemModule
{
const int defaultvalue = 1;
    class Sem
    {
    public:
        Sem(unsigned  int sem_value = defaultvalue)
        {
            sem_init(&_sem, 0, sem_value);
        }

        void P()
        {
            int n = sem_wait(&_sem);
        }
        void V()
        {
            int n = sem_post(&_sem);
        }
        
        ~Sem()
        {
            sem_destroy(&_sem);
        }

    private:
        sem_t _sem;
    };

}