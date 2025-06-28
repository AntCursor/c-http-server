#ifndef ADDRCONV_CHTTPSERVER_ANT_H
#define ADDRCONV_CHTTPSERVER_ANT_H
#include <stddef.h>
#include <stdint.h>

void addrPortToStr(uint32_t addr, uint16_t port, char buff[], size_t buff_size);

// fails if buff is formated incorrectly
uint32_t strToAddrPort(const char buff[], uint32_t *addr, uint16_t *port);
uint32_t strToAddr(const char buff[], uint32_t *addr);
#endif  // !ADDRCONV_CHTTPSERVER_ANT_H
