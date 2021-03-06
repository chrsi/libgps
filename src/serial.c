#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <inttypes.h>
#include <string.h>

#include "serial.h"

int uart0_filestream = -1;

serial_code serial_init(char* interface)
{
    uart0_filestream = open(interface, O_RDWR | O_NOCTTY | O_NDELAY);

    if (uart0_filestream == -1)
    {
        return SERIAL_CANNOT_OPEN;
    }

    return SERIAL_OK;
}

void serial_config(void)
{
    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
}

void serial_println(const char *line, int len)
{
    if (uart0_filestream != -1) {
        char *cpstr = (char *)malloc((len+1) * sizeof(char));
        strcpy(cpstr, line);
        cpstr[len-1] = '\r';
        cpstr[len] = '\n';

        int count = write(uart0_filestream, cpstr, len+1);
        if (count < 0) {
            //TODO: handle errors...
        }
        free(cpstr);
    }
}

// Read a line from UART.
// Return a 0 len string in case of problems with UART
serial_code serial_readln(char *buffer, int len)
{
    char c;
    char *b = buffer;
    int curr_len = 0;

    int rx_length = read(uart0_filestream, (void*)(&c), 1);

    while((c != '\n') && (curr_len < len-1)){
      if (rx_length <= 0){
        sleep(1);
      }else{
        *b++ = c;
        curr_len++;
      }
      rx_length = read(uart0_filestream, (void*)(&c), 1);
    }
    *b++ = '\0';
    if (curr_len >= len-1){
      return SERIAL_BUFFER_OVERFLOW;
    }
    return SERIAL_OK;
}

void serial_close(void)
{
    close(uart0_filestream);
}
