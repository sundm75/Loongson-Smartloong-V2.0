#ifndef _SERIAL_H
#define _SERIAL_H 1

//void set_baudrate (struct termios *opt, unsigned int baudrate);
//void set_data_bit (struct termios *opt, unsigned int databit);
//void set_stopbit (struct termios *opt, const char *stopbit);
//void set_parity (struct termios *opt, char parity);

int  set_port_attr (
                          int fd,
                          int  baudrate,          // B1200 B2400 B4800 B9600 .. B115200
                          int  databit,           // 5, 6, 7, 8
                          const char *stopbit,    //  "1", "1.5", "2"
                          char parity,            // N(o), O(dd), E(ven)
                          int vtime,
                          int vmin );


#endif /* serial.c */

