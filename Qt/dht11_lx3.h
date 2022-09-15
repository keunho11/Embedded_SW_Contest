#ifndef DHT11_LX3_H
#define DHT11_LX3_H

# include  <iostream>
# include  <wiringPi.h>

# define uchar unsigned char
# define dht11_pin 7	 //wiringPi mode pin, dht11 signal line is connected to pin 7
class  dht11_lx3
{
public :
    int buffer[5];

    dht11_lx3 ( ) ;
    //~dht11_lx3();
    void  pininit ( ) ;
    void  dht11_reset ( void ) ;
    uint dht11_read_bit ( void ) ;
    uint dht11_read_byte ( void ) ;
    uint dht11_read_data ( void ) ;

} ;

# endif //DHT11_LX3_H
