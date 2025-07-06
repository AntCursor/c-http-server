#ifndef ADDRCONV_CHTTPSERVER_ANT_H
#define ADDRCONV_CHTTPSERVER_ANT_H
#include <stdint.h>
#include <stdio.h>

#include "errors.h"

ErrCode getAddrPort(const char buff[], uint32_t *addr, uint16_t *port);

void fprintAddrPort(FILE *fp, uint32_t addr, uint16_t port);

#endif // !ADDRCONV_CHTTPSERVER_ANT_H
