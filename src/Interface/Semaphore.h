#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include "Mutex.h"

class Semaphore: public Mutex
{
public:
//  virtual ~Semaphore();
//  virtual void acquire(void) = 0;
//  virtual void release(void) = 0;
  virtual void init(int initial) = 0;
};

#endif
