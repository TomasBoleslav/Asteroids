#ifndef TIMER_HPP
#define TIMER_HPP

class Timer
{
public:
    void start(double duration);
    bool finished();
private:
    double m_startTime;
    double m_duration;
};

#endif