#include "addrconv.h"
#include "errors.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/endian.h>

void addrPortToStr(uint32_t addr, uint16_t port, char buff[],
                   size_t buff_size) {
  addr = ntohl(addr);
  port = ntohs(port);

  snprintf(buff, buff_size, "%d.%d.%d.%d:%d", (addr & 0xff000000) >> 24,
           (addr & 0x00ff0000) >> 16, (addr & 0x0000ff00) >> 8,
           (addr & 0x000000ff), port);
}

ErrCode strToAddrPort(const char buff[], uint32_t *addr, uint16_t *port) {
  uint8_t address[4];
  uint32_t addrN = 0;

  uint8_t nValues = sscanf(buff, "%hhd.%hhd.%hhd.%hhd:%hd", address,
                           address + 1, address + 2, address + 3, port);
  if (nValues != 5)
    return EXIT_FAILURE;

  addrN |= address[0] << 24;
  addrN |= address[1] << 16;
  addrN |= address[2] << 8;
  addrN |= address[3];

  *addr = addrN;

  return EXIT_SUCCESS;
}
ErrCode strToAddr(const char buff[], uint32_t *addr) {
  uint8_t address[4];
  uint32_t addrN = 0;

  uint8_t nValues = sscanf(buff, "%hhd.%hhd.%hhd.%hhd", address, address + 1,
                           address + 2, address + 3);
  if (nValues != 4)
    return EXIT_FAILURE;

  addrN |= address[0] << 24;
  addrN |= address[1] << 16;
  addrN |= address[2] << 8;
  addrN |= address[3];

  // addrN doesn't need to be reversed as we shift it in reverse
  *addr = addrN;

  return EXIT_SUCCESS;
}
