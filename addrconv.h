#ifndef ADDRCONV_CHTTPSERVER_ANT_H
#define ADDRCONV_CHTTPSERVER_ANT_H
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "errors.h"

void addrPortToStr(uint32_t addr, uint16_t port, char buff[], size_t buff_size);

// fails if buff is formated incorrectly
ErrCode strToAddrPort(const char buff[], uint32_t *addr, uint16_t *port);
ErrCode strToAddr(const char buff[], uint32_t *addr);

ErrCode getAddrPort(const char buff[], uint32_t *addr, uint16_t *port);

void fprintAddrPort(FILE *fp, uint32_t addr, uint16_t port);

#endif // !ADDRCONV_CHTTPSERVER_ANT_H
