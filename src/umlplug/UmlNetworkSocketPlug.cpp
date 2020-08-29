/*
 * UmlNetworkSocketPort.cpp
 *
 *  Created on: 29.08.2020
 *      Author: lode
 */

#include "UmlNetworkSocketPlug.h"

#include <errno.h>
#include <string.h>
#include <assert.h>

UmlNetworkSocketPlug::UmlNetworkSocketPlug(const char* socketPathStr)
: socketPath(socketPathStr),
  xferFd(-1),
  myErrno(0),
  txPackets(nullptr), rxPackets(nullptr),
  runningTxPacket(nullptr),
  txDoneLength(0)
{
  privData.sock_type = "unix";
  privData.ctl_sock = socketPath.c_str();

  if (0 > daemon_user_info.init(&privData, this))
    myErrno = errno;
  else
  {
    xferFd = daemon_user_info.open(&privData);
    waitForRead = true;
    waitForWrite = false;
  }
}

UmlNetworkSocketPlug::~UmlNetworkSocketPlug()
{
  destroy();
}

void UmlNetworkSocketPlug::destroy()
{
  daemon_user_info.remove(&privData);
  xferFd = privData.fd;
}

bool UmlNetworkSocketPlug::hasPosixError() const
{
  return (!!myErrno);
}

bool UmlNetworkSocketPlug::getPosixError(int& errnum, const char** eText) const
{
  errnum = errno = myErrno;

  if (eText)
  {
    *eText = strerror(myErrno);
  }

  return hasPosixError();
}

int UmlNetworkSocketPlug::fd()
{
  assert(txPackets && rxPackets);

  waitForRead = !! rxPackets->slotsAvailable();
  waitForWrite = !! txPackets->itemsAvailable();

  return xferFd;
}

//void UmlNetworkSocketPort::txAvailableSignal(bool dataAvailable)
//{
//  waitForWrite = waitForWrite or dataAvailable;
//}

void UmlNetworkSocketPlug::execute(bool readable, bool writable)
{
  assert(txPackets && rxPackets);

  if (writable)
  {
    auto txBuffer = txPackets->consume(false);

    if (txBuffer)
    {
      if (txBuffer->fill)
      {
        auto doneBytes = daemon_user_write(xferFd, runningTxPacket->data.data() + txDoneLength, runningTxPacket->fill - txDoneLength, &privData);

        if (0 >= doneBytes)
        {
          if (0 == doneBytes)
            errno = -ENOTCONN;

          myErrno = errno;
        }
      }

      txPackets->cleanup(txBuffer);
    }
    else
    {
      waitForWrite = false;
    }
  }

  if (readable)
  {
    auto& rxBuffer = rxPackets->produce(false); // must hold the reference, copying the object is wrong, we need to pass back the same object reference

    auto doneBytes = net_recvfrom(xferFd, rxBuffer.data.data(), rxBuffer.data.size());

    if (0 < doneBytes)
    {
      rxBuffer.fill = doneBytes;
    }
    else
    {
      rxBuffer.fill = 0;
      if (0 == doneBytes)
        errno = -ENOTCONN;

      myErrno = errno;
    }

    rxPackets->release(rxBuffer);
  }
}
