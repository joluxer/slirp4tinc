/*
 * HexdumpToByteStream.h
 *
 *  Created on: 01.02.2020
 *      Author: lode
 */

#ifndef HEXDUMPTOBYTESTREAM_H_
#define HEXDUMPTOBYTESTREAM_H_

unsigned hexdumpToByteStream(ByteStream& output, uint8_t* buf, uintptr_t baseAddr, unsigned dumplen = 64);

#endif /* HEXDUMPTOBYTESTREAM_H_ */
