#ifndef TIMER_HPP
#define TIMER_HPP

/**
* Timer that can be used as stopwatch.
*/
class Timer final
{
public:
    Timer();

    // Set duration and start measuring time.
    void start(double duration);

    // Check if the set time is up.
    bool finished() const;

private:
    double m_duration;
    double m_startTime;
};

#endif