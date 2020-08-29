/*
 * HexdumpToByteStream.cpp
 *
 *  Created on: 01.02.2020
 *      Author: lode
 */

#include "ByteStream.h"
#include "Util/PrintfToStream.h"

#include <cctype> // isprint()

unsigned hexdumpToByteStream(ByteStream& output, uint8_t* buf, uintptr_t baseAddr, unsigned dumplen)
{
  PrintfToStream fio(output);

  unsigned count = 0;
  unsigned i, j;
  for (i = 0; i < dumplen; i += 16)
  {
    count += fio.printf("%08lx: ", baseAddr + i);
    for (j = 0; j < 16; j++)
      if (i + j < dumplen)
        count += fio.printf("%02x ", buf[i + j]);
      else
        count += fio.printf("   ");
    count += fio.printf(" ");
    for (j = 0; j < 16; j++)
      if (i + j < dumplen)
        count += fio.printf("%c", isprint(buf[i + j]) ? buf[i + j] : '.');
    count += fio.printf("\n");
  }

  return count;
}

