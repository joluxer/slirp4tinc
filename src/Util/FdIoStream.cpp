/*
 * FdIoStream.cpp
 *
 *  Created on: 23.11.2011
 *      Author: lode
 */

#include "FdIoStream.h"
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>

FdIoStream::FdIoStream(int _sysFd)
: sysFd(_sysFd), myErrno(0)
{
  if (-1 < sysFd)
  {
    int flags = ::fcntl(sysFd, F_GETFL, 0);
    ::fcntl(sysFd, F_SETFL, flags | O_NONBLOCK);
  }
}

//FdIoStream::~FdIoStream()
//{
//  // Auto-generated destructor stub
//}

FdIoStream& FdIoStream::setFd(int _sysFd)
{
  sysFd = _sysFd;
  if (-1 < sysFd)
  {
    int flags = ::fcntl(sysFd, F_GETFL, 0);
    ::fcntl(sysFd, F_SETFL, flags | O_NONBLOCK);
  }

  return *this;
}

unsigned FdIoStream::writeBlock(const unsigned char *b, unsigned int numBytes)
{
  int result = ::write(sysFd, b, numBytes);

  myErrno = 0;
  if (result >= 0)
  {
    return result;
  }
  if ((errno != EAGAIN) && (errno != EWOULDBLOCK))
  {
    myErrno = errno;
  }

  return 0;
}

unsigned FdIoStream::readBlock(unsigned char *b, unsigned int numBytes)
{
  int result;
  int fdmax = sysFd + 1;
  fd_set fdset;
  struct timeval selectTimeout;

  FD_ZERO(&fdset);
  FD_SET(sysFd, &fdset);

  myErrno = 0;
  // Input abfrage
  selectTimeout.tv_sec  = 0;
  selectTimeout.tv_usec = 0;  // sofort zurückkehren
  result = select(fdmax, &fdset, NULL, NULL, &selectTimeout);
  if (result < 0)
  {
    myErrno = errno;
  }
  else if ((result > 0) && FD_ISSET(sysFd, &fdset))
  {
    result = ::read(sysFd, b, numBytes);

    if (result >= 0)
    {
      return result;
    }
    if ((errno != EAGAIN) && (errno != EWOULDBLOCK))
    {
      myErrno = errno;
    }
  }

  return 0;
}

unsigned FdIoStream::write(unsigned char b)
{
  return writeBlock(&b, 1);
}

unsigned FdIoStream::read(unsigned char &b)
{
  return readBlock(&b, 1);
}

