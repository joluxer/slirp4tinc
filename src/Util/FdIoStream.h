/*
 * FdIoStream.h
 *
 *  Created on: 23.11.2011
 *      Author: lode
 */

#ifndef LINUXFDIOSTREAM_H_
#define LINUXFDIOSTREAM_H_

#include "ByteStream.h"

/***
 * Dies Klasse implementiert einen ByteStream auf einem klassischen
 * File-Descriptor unter Posix-Umgebungen. Wenn ein Fehler auftrat,
 * ist der Rückgabewert von hasPosixError() true, und es kann mit
 * Posix perror() das Problem gelesen werden, errno ist entsprechend gesetzt.
 */
class FdIoStream : public ByteStream
{
public:
  FdIoStream(int sysFd = -1);
//  virtual ~FdIoStream();

  FdIoStream& setFd(int fd);
  int fd() const;

  bool hasPosixError() const;
  int getErrno() const;


  // das ByteStream Interface
  virtual unsigned write(unsigned char b);
  virtual unsigned read(unsigned char &b);

  virtual unsigned writeBlock(const unsigned char *b, unsigned int numBytes);
  virtual unsigned readBlock(unsigned char *b, unsigned int numBytes);

protected:
  int sysFd;
  volatile int myErrno;
};

inline
bool FdIoStream::hasPosixError() const
{
  return (0 != myErrno);
}

inline
int FdIoStream::getErrno() const
{
  return myErrno;
}

inline
int FdIoStream::fd() const
{
  return sysFd;
}

#endif /* LINUXFDIOSTREAM_H_ */
