/*
 * SlirpHandler.h
 *
 *  Created on: 29.08.2020
 *      Author: lode
 */

#ifndef SLIRPHANDLER_H_
#define SLIRPHANDLER_H_

#include "FdSelectHandler.h"
#include "Slirp4TincTypes.h"
#include <slirp/libslirp.h>
#include <slirp/libvdeslirp.h>

class SlirpHandler: public FdSelectHandler
{
public:
  SlirpHandler(struct vdeslirp *slirpDesc);
  virtual ~SlirpHandler();

  void connectTransferBuffers(PacketRingBuffer* txBuffers, PacketRingBuffer* rxBuffers);

  void destroy() override;

protected:
  void execute(bool readable, bool writable) override;
  int fd() override;

private:
  struct vdeslirp *myslirp;

  PacketRingBuffer *txPackets, *rxPackets;
};

inline
void SlirpHandler::connectTransferBuffers(PacketRingBuffer* txBuffers, PacketRingBuffer* rxBuffers)
{
  txPackets = txBuffers;
  rxPackets = rxBuffers;
}

#endif /* SLIRPHANDLER_H_ */
