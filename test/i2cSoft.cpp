#include <iostream>
#include <wiringPi.h>
#include <unistd.h>
using namespace std;

#define PORT_SDA 0
#define PORT_SCK 1
#define I2C_CLK_DELAY  usleep(1)
#define I2C_DATA_DELAY usleep(1)
#define I2C_WAIT_DELAY usleep(1000)

class i2cSoft
{
public:

    i2cSoft(int SCK_PIN  ,int SDA_PIN )
    {
    if (wiringPiSetup() == -1)
    {
    cout << "i2cSoft Error!" << endl;
    return;
    }
    SCK_PIN_ =  SCK_PIN  ;
    SDA_PIN_ =  SDA_PIN ;
    }

void I2C_TRIS_WRITE(int port,int direct ) 
{
   if(port == PORT_SDA ) 
   {
     if(direct==1) 
     {
        pinMode( SDA_PIN_ , INPUT ); 
     }
     else if(direct==0) 
     {
        pinMode( SDA_PIN_ , OUTPUT); 
     }
   }
   else if(port == PORT_SCK ) 
   {
     if(direct==1) 
     {
        pinMode( SCK_PIN_ , INPUT ); 
     }
     else if(direct==0) 
     {
        pinMode( SCK_PIN_ , OUTPUT); 
     }
   }
}
void I2C_LAT_WRITE(int port,int direct ) 
{
   if(port == PORT_SDA ) 
   {
     if(direct==1) 
     {
        digitalWrite( SDA_PIN_ , HIGH ); 
     }
     else if(direct==0) 
     {
        digitalWrite( SDA_PIN_ , LOW ); 
     }
   }
   else if(port == PORT_SCK ) 
   {
     if(direct==1) 
     {
        digitalWrite( SCK_PIN_ , HIGH ); 
     }
     else if(direct==0) 
     {
        digitalWrite( SCK_PIN_ , LOW ); 
     }
   }
}
unsigned char I2C_PORT_READ(int port) 
{
   unsigned char data ; 
   if(port == PORT_SDA ) 
   {
      data = digitalRead( SDA_PIN_ ); 
   }
   else if(port == PORT_SCK ) 
   {
      data = digitalRead( SCK_PIN_ ); 
   }
return data;
}
void I2C_INIT()
{
    I2C_TRIS_WRITE(PORT_SDA , 1) ;
    I2C_TRIS_WRITE(PORT_SCK , 1) ;
    I2C_DATA_DELAY ;
    I2C_LAT_WRITE(PORT_SDA , 0) ;
    I2C_LAT_WRITE(PORT_SCK , 0) ;
    I2C_WAIT_DELAY;
}

void I2C_START()
{
    I2C_LAT_WRITE(PORT_SDA , 0) ;
    I2C_LAT_WRITE(PORT_SCK , 0) ;
    I2C_DATA_DELAY ;
    I2C_TRIS_WRITE(PORT_SCK , 1) ;
    I2C_TRIS_WRITE(PORT_SDA , 1) ;
    I2C_CLK_DELAY ;
    I2C_TRIS_WRITE(PORT_SDA , 0) ;
    I2C_CLK_DELAY ;
    I2C_WAIT_DELAY;
}

void I2C_RESTART()
{
    I2C_TRIS_WRITE(PORT_SCK , 0) ;
    I2C_TRIS_WRITE(PORT_SDA , 1) ;
    I2C_DATA_DELAY ;
    I2C_TRIS_WRITE(PORT_SCK , 1) ;
    I2C_TRIS_WRITE(PORT_SDA , 0) ;
    I2C_DATA_DELAY ;
    I2C_WAIT_DELAY;
}

void I2C_STOP()
{
    I2C_TRIS_WRITE(PORT_SCK , 0) ;
    I2C_TRIS_WRITE(PORT_SDA , 0) ;
    I2C_DATA_DELAY ;
    I2C_TRIS_WRITE(PORT_SCK , 1) ;
    I2C_TRIS_WRITE(PORT_SDA , 1) ;
    I2C_DATA_DELAY ;
    I2C_WAIT_DELAY;
}

void I2C_SEND_ACK()
{
    I2C_TRIS_WRITE(PORT_SCK , 0) ;
    I2C_TRIS_WRITE(PORT_SDA , 0) ;
    I2C_DATA_DELAY ;
    I2C_TRIS_WRITE(PORT_SCK , 1) ;
    I2C_CLK_DELAY ;
    I2C_WAIT_DELAY;
}

void I2C_SEND_NACK()
{
    I2C_TRIS_WRITE(PORT_SCK , 0) ;
    I2C_TRIS_WRITE(PORT_SDA , 1) ;
    I2C_DATA_DELAY ;
    I2C_TRIS_WRITE(PORT_SCK , 1) ;
    I2C_CLK_DELAY ;
    I2C_WAIT_DELAY;
}

unsigned char I2C_WRITE( unsigned char data)
{
    unsigned char i ;
    for ( i = 0 ; i < 8 ; i++ )
    {
        I2C_TRIS_WRITE(PORT_SCK , 0) ;
        if ( (data << i) & 0x80 ) I2C_TRIS_WRITE(PORT_SDA , 1) ;
        else I2C_TRIS_WRITE(PORT_SDA , 0) ;
        I2C_DATA_DELAY ;
        I2C_TRIS_WRITE(PORT_SCK , 1) ;
        I2C_CLK_DELAY ;
    }
    I2C_TRIS_WRITE(PORT_SCK , 0) ;
    I2C_TRIS_WRITE(PORT_SDA , 1) ;
    I2C_DATA_DELAY ;
    I2C_TRIS_WRITE(PORT_SCK , 1) ;
    I2C_DATA_DELAY ;
    I2C_WAIT_DELAY;
    return I2C_PORT_READ(PORT_SDA) ;
}

unsigned char I2C_READ()
{
    unsigned char i , RxData = 0 ;
    for ( i = 0 ; i < 8 ; i++ )
    {
        I2C_TRIS_WRITE(PORT_SCK , 0) ;
        I2C_TRIS_WRITE(PORT_SDA , 1) ;
        I2C_CLK_DELAY ;
        I2C_TRIS_WRITE(PORT_SCK , 1) ;
        I2C_DATA_DELAY ;
        RxData = (RxData | (I2C_PORT_READ(PORT_SDA) << (7 - i))) ;
        I2C_DATA_DELAY ;
    }
    I2C_WAIT_DELAY;
    return RxData ;
}

void I2C_Detect() 
{
    for(int i=0;i<255;i++)    
    {
        I2C_START();
        if(I2C_WRITE(i) == 0) cout << i << endl;
        I2C_STOP();
    }
}

int SCK_PIN_  ;
int SDA_PIN_ ;
};
