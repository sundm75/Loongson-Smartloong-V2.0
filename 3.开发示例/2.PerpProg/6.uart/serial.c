/*
 * Program:     serial.c
 * Author:      Paul Dean
 * Version:     0.0.3
 * Date:        2002-02-19
 * Description: To provide underlying serial port function,
 *              for high level applications.
 *
*/

#include <termios.h>            /* tcgetattr, tcsetattr */
#include <stdio.h>              /* perror, printf, puts, fprintf, fputs */
#include <unistd.h>             /* read, write, close */
#include <fcntl.h>              /* open */
#include <sys/signal.h>
#include <termios.h>
#include <sys/types.h>
#include <string.h>             /* bzero, memcpy */
#include <limits.h>             /* CHAR_MAX */

#include "serial.h"



/* set serial port baudrate by use of file descriptor fd */
static void set_baudrate (struct termios *opt, unsigned int baudrate)
{
	cfsetispeed(opt, baudrate);
	cfsetospeed(opt, baudrate);
}

static void set_data_bit (struct termios *opt, unsigned int databit)
{
    opt->c_cflag &= ~CSIZE;
    switch (databit) {
    case 8:
        opt->c_cflag |= CS8;
        break;
    case 7:
        opt->c_cflag |= CS7;
        break;
    case 6:
        opt->c_cflag |= CS6;
        break;
    case 5:
        opt->c_cflag |= CS5;
        break;
    default:
        opt->c_cflag |= CS8;
        break;
    }
}

static void set_stopbit (struct termios *opt, const char *stopbit)
{
    if (0 == strcmp (stopbit, "1")) {
        opt->c_cflag &= ~CSTOPB; /* 1 stop bit */
    }	else if (0 == strcmp (stopbit, "1")) {
        opt->c_cflag &= ~CSTOPB; /* 1.5 stop bit */
    }   else if (0 == strcmp (stopbit, "2")) {
        opt->c_cflag |= CSTOPB;  /* 2 stop bits */
    } else {
        opt->c_cflag &= ~CSTOPB; /* 1 stop bit */
    }
}

static void set_parity (struct termios *opt, char parity)
{
    switch (parity) {
    case 'N':                  /* no parity check */
        opt->c_cflag &= ~PARENB;
        break;
    case 'E':                  /* even */
        opt->c_cflag |= PARENB;
        opt->c_cflag &= ~PARODD;
        break;
    case 'O':                  /* odd */
        opt->c_cflag |= PARENB;
        opt->c_cflag |= ~PARODD;
        break;
    default:                   /* no parity check */
        opt->c_cflag &= ~PARENB;
        break;
    }
}


int  set_port_attr (
			  int fd,
                          int  baudrate,          // B1200 B2400 B4800 B9600 .. B115200
                          int  databit,           // 5, 6, 7, 8
                          const char *stopbit,    //  "1", "1.5", "2"
                          char parity,            // N(o), O(dd), E(ven)
			  int vtime,
			  int vmin )
{
	 struct termios opt;
//    	bzero(&opt, sizeof (opt));
//        cfmakeraw (&termios_new);

	tcgetattr(fd, &opt);

        set_baudrate(&opt, baudrate);

    	opt.c_cflag 		 |= CLOCAL | CREAD;      /* | CRTSCTS */

    	set_data_bit(&opt, databit);
    	set_parity(&opt, parity);
    	set_stopbit(&opt, stopbit);

    	opt.c_oflag 		 = 0;
    	opt.c_lflag            	|= 0;
    	opt.c_oflag          	&= ~OPOST;
    	opt.c_cc[VTIME]     	 = vtime;        
    	opt.c_cc[VMIN]         	 = vmin; 
    	tcflush (fd, TCIFLUSH);

        return (tcsetattr (fd, TCSANOW, &opt));
}
