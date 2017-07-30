
#ifndef TIMER_H
#define TIMER_H

class Timer
{
  private:
    unsigned int _ms;
    unsigned long _lastExecution = 0;

  public:
    Timer(unsigned int ms) : _ms(ms)
    {
        _lastExecution = 0;
    }

    bool ready()
    {
        auto now = millis();

        if (now - _lastExecution >= _ms)
        {
            _lastExecution = now;

            return true;
        }

        return false;
    }

    void reset()
    {
        _lastExecution = millis();
    }
};

#endif