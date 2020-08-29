/*
 * UmlNetworkSocketPort.h
 *
 *  Created on: 29.08.2020
 *      Author: lode
 */

#ifndef UMLNETWORKSOCKETPLUG_H_
#define UMLNETWORKSOCKETPLUG_H_

#include "FdSelectHandler.h"

#include <string>

#include "daemon.h"
#include "Slirp4TincTypes.h"
#include "Callback.h"

class UmlNetworkSocketPlug: public FdSelectHandler
{
public:
  UmlNetworkSocketPlug(const char* socketPath);
  virtual ~UmlNetworkSocketPlug();

  void connectTransferBuffers(PacketRingBuffer* txBuffers, PacketRingBuffer* rxBuffers);

  bool hasPosixError() const;
  bool getPosixError(int& errnum, const char** eText) const;

  void destroy() override;

//  void txAvailableSignal(bool dataAvailable = true);

protected:
  void execute(bool readable, bool writable) override;
  int fd() override;

private:
  ::std::string socketPath;
  daemon_data privData;
  int xferFd;
  int myErrno;

  PacketRingBuffer *txPackets, *rxPackets;
  XferBuffer* runningTxPacket;
  size_t txDoneLength;
};

inline
void UmlNetworkSocketPlug::connectTransferBuffers(PacketRingBuffer* txBuffers, PacketRingBuffer* rxBuffers)
{
  txPackets = txBuffers;
  rxPackets = rxBuffers;
}

#endif /* UMLNETWORKSOCKETPLUG_H_ */
