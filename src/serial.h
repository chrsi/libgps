#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <inttypes.h>

#ifndef PORTNAME
#define PORTNAME "/dev/ttyAMA0"
#endif

typedef enum {
  SERIAL_OK,
  SERIAL_CANNOT_OPEN,
  SERIAL_BUFFER_OVERFLOW
} serial_code;

serial_code serial_init(char* interface);
void serial_config(void);
void serial_println(const char *, int);
serial_code serial_readln(char *, int);
void serial_close(void);

#endif
