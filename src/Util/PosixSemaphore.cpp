/*
 * PosixSemaphore.cpp
 *
 *  Created on: 25.09.2013
 *      Author: lode
 */

#include "PosixSemaphore.h"

#include <stdio.h>
#include <stdlib.h>

//PosixSemaphore::PosixSemaphore()
//{
//  // Auto-generated constructor stub
//
//}

//PosixSemaphore::~PosixSemaphore()
//{
//  // Auto-generated destructor stub
//}

void PosixSemaphore::acquire(void)
{
  int res = sem_wait(&posixSema);

  if (res)
  {
    perror("releasing posix sema");
    exit(-1);
  }
}

void PosixSemaphore::release(void)
{
  int res = sem_post(&posixSema);

  if (res)
  {
    perror("releasing posix sema");
    exit(-1);
  }
}

void PosixSemaphore::init(int initial)
{
  int res = sem_init(&posixSema, 0, initial);

  if (res)
  {
    perror("releasing posix sema");
    exit(-1);
  }
}

