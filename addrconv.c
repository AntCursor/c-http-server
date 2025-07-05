#include "addrconv.h"

#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"

#define IPV4_OCTET_COUNT  4
#define IPV4_ADDR_PARTS   5 // 4 octets + port
#define MIN_IPV4_ADDR_LEN 7
#define MAX_IPV4_ADDR_LEN 15
#define MAX_PORT_LEN      5
#define MAX_IPV4_ADDRPORT_STR_LEN                                              \
  MAX_IPV4_ADDR_LEN + MAX_PORT_LEN + 1 +                                       \
    1 // Ex: "192.168.150.255:65535\0" -> 22 characters long.

void
addrPortToStr(uint32_t addr, uint16_t port, char buff[], size_t buff_size)
{
  snprintf(buff,
           buff_size,
           "%d.%d.%d.%d:%d",
           (addr & 0xff000000) >> 24,
           (addr & 0x00ff0000) >> 16,
           (addr & 0x0000ff00) >> 8,
           (addr & 0x000000ff),
           port);
}

ErrCode
strToAddrPort(const char buff[], uint32_t* addr, uint16_t* port)
{
  uint8_t  address[IPV4_OCTET_COUNT];
  uint32_t addrN = 0;
  uint16_t portN = 0;

  uint8_t nValues = sscanf(buff,
                           "%hhu.%hhu.%hhu.%hhu:%hu",
                           address,
                           address + 1,
                           address + 2,
                           address + 3,
                           &portN);
  if (nValues != IPV4_ADDR_PARTS)
    return ERR_INVALID_ARGS;

  addrN |= address[0] << 24;
  addrN |= address[1] << 16;
  addrN |= address[2] << 8;
  addrN |= address[3];

  *addr = addrN;
  *port = portN;

  return ERR_SUCCESS;
}
ErrCode
strToAddr(const char buff[], uint32_t* addr)
{
  uint8_t  address[IPV4_OCTET_COUNT];
  uint32_t addrN = 0;

  uint8_t nValues = sscanf(buff,
                           "%hhu.%hhu.%hhu.%hhu",
                           address,
                           address + 1,
                           address + 2,
                           address + 3);
  if (nValues != IPV4_OCTET_COUNT)
    return ERR_INVALID_ARGS;

  addrN |= address[0] << 24;
  addrN |= address[1] << 16;
  addrN |= address[2] << 8;
  addrN |= address[3];

  // addrN doesn't need to be reversed as we shift it in reverse
  *addr = addrN;

  return ERR_SUCCESS;
}
ErrCode
getAddrPort(const char buff[], uint32_t* addr, uint16_t* port)
{
  if (!buff || !addr || !port)
    return ERR_INVALID_ARGS;

  size_t  argumentLenght = strlen(buff);
  ErrCode error          = ERR_SUCCESS;
  bool    pair           = false;

  for (uint8_t i = MIN_IPV4_ADDR_LEN;
       i < argumentLenght && i < MAX_IPV4_ADDR_LEN + 1;
       ++i) {
    if (buff[i] == ':') {
      pair = true;
      break;
    }
  }

  if (pair)
    error = strToAddrPort(buff, addr, port);
  else if (argumentLenght > MAX_PORT_LEN)
    error = strToAddr(buff, addr);
  else
    *port = atoi(buff);
  return error;
}

void
fprintAddrPort(FILE* fp, uint32_t addr, uint16_t port)
{
  // Assumes values are in network byte order
  char addrPortString[MAX_IPV4_ADDRPORT_STR_LEN];
  addrPortToStr(
    ntohl(addr), ntohs(port), addrPortString, MAX_IPV4_ADDRPORT_STR_LEN);
  fprintf(fp, "%s", addrPortString);
}
