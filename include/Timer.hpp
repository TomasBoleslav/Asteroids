#ifndef TIMER_HPP
#define TIMER_HPP

class Timer
{
public:
    Timer();
    void start(double duration);
    bool finished();
private:
    double m_duration;
    double m_startTime;
};

#endif