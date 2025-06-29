#include "addrconv.h"

#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "errors.h"

#define MIN_SIZE_ADDR 7
#define MAX_SIZE_PORT 5
#define MAX_SIZE_ADDRPORT                                                      \
  22 // Ex: "192.168.150.255:65535\0" -> 22 characters long.

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
ErrCode getAddrPort(const char buff[], uint32_t *addr, uint16_t *port) {
  size_t argumentLenght = strlen(buff);
  bool error = false;
  bool pair = false;

  for (uint8_t i = MIN_SIZE_ADDR; i < argumentLenght; ++i) {
    if (buff[i] == ':') {
      pair = true;
      break;
    }
  }

  if (pair)
    error = strToAddrPort(buff, addr, port);
  else if (argumentLenght > MAX_SIZE_PORT)
    error = strToAddr(buff, addr);
  else
    *port = atoi(buff);
  return error ? EXIT_FAILURE : EXIT_SUCCESS;
}

void fprintAddrPort(FILE *fp, uint32_t addr, uint16_t port) {
  char addrPortString[MAX_SIZE_ADDRPORT];
  addrPortToStr(addr, port, addrPortString, MAX_SIZE_ADDRPORT);
  fprintf(fp, "%s", addrPortString);
}
