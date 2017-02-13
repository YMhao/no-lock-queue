#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>
class Timer
{
public:
    Timer();
    // 开始计时时间
    void Start();
    // 终止计时时间
    void Stop();
    // 重新设定
    void Reset();
    // 耗时时间
    void Cost_time();
private:
    struct timeval t1;
    struct timeval t2;
    bool b1,b2;
};
#endif
