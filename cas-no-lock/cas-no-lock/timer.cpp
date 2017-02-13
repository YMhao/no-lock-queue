#include "timer.h"
#include <stdio.h>

Timer::Timer()
{
    b1 = false;
    b2 = false;
}
void Timer::Start()
{
    gettimeofday(&t1,NULL);
    b1 = true;
    b2 = false;
}

void Timer::Stop()
{
    if (b1 == true)
    {
        gettimeofday(&t2,NULL);
        b2 = true;
    }
}

void Timer::Reset()
{
    b1 = false;
    b2 = false;
}

void Timer::Cost_time()
{
    if (b1 == false)
    {
        printf("计时出错，应该先执行Start()，然后执行Stop()，再来执行Cost_time()");
        return ;
    }
    else if (b2 == false)
    {
        printf("计时出错，应该执行完Stop()，再来执行Cost_time()");
        return ;
    }
    else
    {
        int usec,sec;
        bool borrow = false;
        if (t2.tv_usec > t1.tv_usec)
        {
            usec = t2.tv_usec - t1.tv_usec;
        }
        else
        {
            borrow = true;
            usec = t2.tv_usec+1000000 - t1.tv_usec;
        }

        if (borrow)
        {
            sec = t2.tv_sec-1 - t1.tv_sec;
        }
        else
        {
            sec = t2.tv_sec - t1.tv_sec;
        }
        printf("花费时间:%d秒 %d微秒\n",sec,usec);
    }
}
