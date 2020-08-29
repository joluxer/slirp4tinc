/*
 * SlirpHandler.cpp
 *
 *  Created on: 29.08.2020
 *      Author: lode
 */

#include "SlirpHandler.h"

#include <assert.h>

SlirpHandler::SlirpHandler(struct vdeslirp *slirpDesc)
: myslirp(slirpDesc),
  txPackets(nullptr), rxPackets(nullptr)
{
  waitForRead = false;
  waitForWrite = false;
}

SlirpHandler::~SlirpHandler()
{
  destroy();
}

void SlirpHandler::destroy()
{
  if (myslirp)
  {
    auto tmpPtr = myslirp;
    myslirp = nullptr;
    vdeslirp_close(tmpPtr);
  }
}

void SlirpHandler::execute(bool readable, bool writable)
{
  assert(txPackets && rxPackets && myslirp);

  if (readable)
  {
    auto rxBuffer = rxPackets->produce(false);

    auto doneBytes = vdeslirp_recv(myslirp, rxBuffer.data.data(), rxBuffer.data.size());

    if (0 < doneBytes)
      rxBuffer.fill = doneBytes;

    rxPackets->release(rxBuffer);
  }

  if (writable)
  {
    auto txBuffer = txPackets->consume(false);

    if (txBuffer)
    {
      if (txBuffer->fill)
        vdeslirp_send(myslirp, txBuffer->data.data(), txBuffer->fill);

      txPackets->cleanup(txBuffer);
    }
    else
    {
      waitForWrite = false;
    }
  }
}

int SlirpHandler::fd()
{
  assert(txPackets && rxPackets && myslirp);

  waitForRead = !! rxPackets->slotsAvailable();
  waitForWrite = !! txPackets->itemsAvailable();

  return vdeslirp_fd(myslirp);
}
