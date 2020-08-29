#ifndef MUTEX_H_
#define MUTEX_H_

class Mutex
{
public:
  virtual ~Mutex();
  virtual void acquire(void) = 0;
  virtual void release(void) = 0;

  class ScopedLock
  {
    Mutex& m;
  public:
    explicit
    ScopedLock(Mutex& _m)
    : m(_m)
    {
      m.acquire();
    }

    ~ScopedLock()
    {
      m.release();
    }
  };
};

#endif
