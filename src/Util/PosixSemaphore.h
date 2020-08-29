/*
 * PosixSemaphore.h
 *
 *  Created on: 25.09.2013
 *      Author: lode
 */

#ifndef POSIXSEMAPHORE_H_
#define POSIXSEMAPHORE_H_

#include "Semaphore.h"

#include <semaphore.h>

/**
 * @note Link this class with -lpthread.
 */
class PosixSemaphore: public Semaphore
{
public:
//  PosixSemaphore();
//  virtual ~PosixSemaphore();

  virtual void acquire(void);
  virtual void release(void);
  virtual void init(int initial);

protected:
  sem_t posixSema;
};

#endif /* POSIXSEMAPHORE_H_ */
