#include "dht11_lx3.h"

dht11_lx3 :: dht11_lx3 ( )
{
    for(int i = 0; i < 5; i++)
    {
        buffer[i] = 0;
    }
}

//dht11_lx3::~dht11_lx3()
//{


//}

void dht11_lx3 :: pininit ( )
{
    //printf( "Humidity = %c%% Temperature = %c*C\n", 'a', 'b');
    wiringPiSetup ( ) ;
    pinMode ( dht11_pin , OUTPUT ) ; //Set Pin 0 as output mode, if it is'INPUT', it is input mode, and the encoding format is wiringpi
    digitalWrite ( dht11_pin , HIGH ) ; //Pull up
}

void dht11_lx3 :: dht11_reset ( void )
{
    digitalWrite ( dht11_pin , LOW ) ;
    delay ( 20 ) ;
    digitalWrite ( dht11_pin , HIGH ) ;
    delayMicroseconds ( 30 ) ;
    pinMode ( dht11_pin , INPUT ) ; //Set as input and insert the code chip here
    pullUpDnControl ( dht11_pin , PUD_UP ) ; //pull up

}


uint dht11_lx3 :: dht11_read_bit ( void )
{
    uint count1 = 0;
    while  ( digitalRead ( dht11_pin )  ==  0 ) ;
    delayMicroseconds ( 35 ) ;
    if  ( digitalRead ( dht11_pin )  ==  1 )
    {
        //while (digitalRead( dht11_pin) == 1);
        while  ( digitalRead ( dht11_pin )  ==  1 )
        {
            delayMicroseconds ( 1 ) ;
            count1 ++;
            if ( count1 > 50 )
                break ;
        }
        return  1 ;
    }
    else
    {
        return  0 ;
    }
}

uint dht11_lx3 :: dht11_read_byte ( void )
{
    uint i ;
    uint data =  0 ;
    for  ( i =  0 ; i <  8 ; i ++ )
    {
        data <<=  1 ;
        data |=  dht11_read_bit ( ) ;
    }
    return data ;
}

uint dht11_lx3 :: dht11_read_data ( )
{
    uint i =  0 ;
    uchar checksum ;
    dht11_reset ( ) ;
    if  ( digitalRead ( dht11_pin )  ==  0 )
    {
        while  ( digitalRead ( dht11_pin )  ==  0 ) ;
        while  ( digitalRead ( dht11_pin )  ==  1 ) ;
        for  ( i =  0 ; i <  5 ; i ++ )
        {
            buffer [ i ]  =  dht11_read_byte ( ) ; //Temperature and humidity data into array
        }
        while  ( digitalRead ( dht11_pin )  ==  0 ) ;
        pinMode ( dht11_pin , OUTPUT ) ;
        digitalWrite ( dht11_pin , HIGH ) ;

        checksum = buffer [ 0 ]  + buffer [ 1 ]  + buffer [ 2 ]  + buffer [ 3 ] ; //check
        if  ( checksum != buffer [ 4 ] )
        {
            //checksum error
            printf( "Humidity = %d%% Temperature = %d*C\n", buffer [ 0 ], buffer [ 2 ]);
            delay(1000);
            return  1 ;
        }
    }

    return  0 ;
}
