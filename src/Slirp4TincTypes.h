/*
 * Slirp4TincTypes.h
 *
 *  Created on: 29.08.2020
 *      Author: lode
 */

#ifndef SLIRP4TINCTYPES_H_
#define SLIRP4TINCTYPES_H_

#include <array>

#include "umlplug/daemon.h"
#include "Util/ProducerConsumer.h"
#include "Util/PosixSemaphore.h"


struct XferBuffer
{
  ::std::array<uint8_t, ETH_MAX_PACKET + ETH_ADDR_LEN + ETH_HEADER_ETHERTAP + ETH_HEADER_OTHER> data;
  size_t fill;
};

typedef ProducerConsumer<XferBuffer, 256, PosixSemaphore> PacketRingBuffer;

#endif /* SLIRP4TINCTYPES_H_ */
