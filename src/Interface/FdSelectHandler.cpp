/*
 * FdSelectHandler.cpp
 *
 *  Created on: 04.10.2012
 *      Author: lode
 */

#include "FdSelectHandler.h"

#include <sys/select.h>
#include <stdio.h>
#include <time.h>

FdSelectHandler::FdSelectHandler()
: waitForRead(false), waitForWrite(false),
  prev(0), next(0), chain(0)
{}

FdSelectHandler::~FdSelectHandler()
{
  removeFromChain();
}

void FdSelectHandler::addTo(FdSelectHandler* &_chain)
{
  // prepend it to the chain
  prev = 0;
  next = _chain;
  chain = &_chain;

  if (next)
  {
    next->prev = this;
  }
  _chain = this;
}

void FdSelectHandler::removeFromChain()
{
  if (prev)
    prev->next = next;
  else if (chain)
    *chain = next;

  if (next)
    next->prev = prev;

  prev = 0;
  next = 0;
  chain = 0;
}

bool FdSelectHandler::waitOn(FdSelectHandler* &chain, struct timespec* relativeTimeout)
{
  bool didWork = false;

  if (chain)
  {
    FdSelectHandler *h, *h_next;
    fd_set readSet, writeSet;
    int fdMax = -1;

    FD_ZERO(&readSet);
    FD_ZERO(&writeSet);

    for (h = chain; h; h = h->next)
    {
      int fd = h->fd();
      if (0 > fd)
        continue;

      // check maxFd
      if (fdMax < fd)
        fdMax = fd;

      // collect fd in set
      if (h->waitForRead)
        FD_SET(fd, &readSet);

      if (h->waitForWrite)
        FD_SET(fd, &writeSet);
    }

    if (fdMax > -1)
    {
      // do select
      int ready;

      ready = ::pselect(fdMax + 1, &readSet, &writeSet, 0, relativeTimeout, 0);

      if (ready > 0)
      {
        // kein timeout, einige fd's sind ready, also suchen und abarbeiten
        for (h = chain; h; h = h_next)
        {
          int fd = h->fd();
          bool readable = false;
          bool writable = false;

          // check fd in sets
          readable = (h->waitForRead && FD_ISSET(fd, &readSet));
          writable = (h->waitForWrite && FD_ISSET(fd, &writeSet));

          // store next pointer, as h->execute() is allowed to destroy/invalidate the h object
          h_next = h->next;
          // execute handler, if needed
          if (readable || writable)
            h->execute(readable, writable);
        }
        didWork = true;
      }
      else if (ready < 0)
      {
        // Posix-Fehlerbehandlung: Fehler ausgeben, weiter arbeiten
        ::perror("Error handling I/O with select()");
      }
    }
    else if (relativeTimeout)
    {
      // do relative wait
      if (0 != nanosleep(relativeTimeout, 0))
      {
        ::perror("Error while sleeping instead of select()");
      }
    }
  }
  else if (relativeTimeout)
  {
    // do relative wait
    if (0 != nanosleep(relativeTimeout, 0))
    {
      ::perror("Error while sleeping instead of select()");
    }
  }

  return didWork;
}

bool FdSelectHandler::checkAll(FdSelectHandler* &chain)
{
  struct timespec zeroTimeout;

  zeroTimeout.tv_sec = 0;
  zeroTimeout.tv_nsec = 0;

  return waitOn(chain, &zeroTimeout);
}

