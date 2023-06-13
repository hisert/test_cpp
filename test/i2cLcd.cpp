#include <iostream>
#include "I2CDevice.cpp"
using namespace std;

I2CDevice lcdx("/dev/i2c-0");

#define i2c_write lcdx.write
#define slave_address_writing 0x20
class LCD
{
public:

unsigned char Port(unsigned char a) 
{
    if ( a & 1 ) PIN_P4 = 1 ;
    else PIN_P4 = 0 ;
    if ( a & 2 ) PIN_P5 = 1 ;
    else PIN_P5 = 0 ;
    if ( a & 4 ) PIN_P6 = 1 ;
    else PIN_P6 = 0 ;
    if ( a & 8 ) PIN_P7 = 1 ;
    else PIN_P7 = 0 ;
    unsigned char I2C_Data = PIN_P7 * 128 + PIN_P6 * 64 + PIN_P5 * 32 + PIN_P4 * 16 ;
    return I2C_Data ;
}

void Write_Cmd(unsigned char a)
{
    unsigned char I2C_Data_Port ;
    unsigned char I2C_Data ;
    unsigned char ack_received ;
    unsigned char sender[2];
    PIN_P0 = 0 ; // RS - Send Commands
    PIN_P1 = 0 ; // RW - Write
    I2C_Data_Port = Port(a) ; // Sending Command
    PIN_P2 = 1 ; // E - Drive High Enable to Send Command
    I2C_Data = I2C_Data_Port + PIN_P3 * 8 + PIN_P2 * 4 + PIN_P1 * 2 + PIN_P0 * 1 ;
    sender[0] = 0x00;
    sender[1] = I2C_Data;
    i2c_write(slave_address_writing,sender,2);
    PIN_P2 = 0 ; // Reset Enable
    I2C_Data = I2C_Data_Port + PIN_P3 * 8 + PIN_P2 * 4 + PIN_P1 * 2 + PIN_P0 * 1 ;
    sender[0] = 0x00;
    sender[1] = I2C_Data;
    i2c_write(slave_address_writing,sender,2);
}

void Write_Data(unsigned char a)
{
    unsigned char I2C_Data_Port ;
    unsigned char I2C_Data ;
    unsigned char ack_received ;
    unsigned char sender[2];
    PIN_P0 = 1 ; // RS - Send Data
    PIN_P1 = 0 ; // RW - Write 
    I2C_Data_Port = Port(a) ; // Sending Command
    PIN_P2 = 1 ; // Drive High Enable to Send Command
    I2C_Data = I2C_Data_Port + PIN_P3 * 8 + PIN_P2 * 4 + PIN_P1 * 2 + PIN_P0 * 1 ;
    
    sender[0] = 0x00;
    sender[1] = I2C_Data;
    i2c_write(slave_address_writing,sender,2);
    PIN_P2 = 0 ; // E - Enable, First 4 Bits Sent
    I2C_Data = I2C_Data_Port + PIN_P3 * 8 + PIN_P2 * 4 + PIN_P1 * 2 + PIN_P0 * 1 ;
    sender[0] = 0x00;
    sender[1] = I2C_Data;
    i2c_write(slave_address_writing,sender,2);
}

void Four_Bit_Init() 
{
    Write_Cmd(0x00) ;
    Write_Cmd(0x03) ; // Send 0000 0011 to LCD (Cursor Home)         
    Write_Cmd(0x03) ; // Send 0000 0011 to LCD (Cursor Home) 
    Write_Cmd(0x03) ; // Send 0000 0011 to LCD (Cursor Home)
    Write_Cmd(0x02) ; // Send 0000 0010 to LCD (Cursor Home) 
    Write_Cmd(0x02) ; // Send 0000 0010 to LCD (Cursor Home))
    Write_Cmd(0x08) ; // Send 0000 1000 to LCD (Cursor/display shift)
    Write_Cmd(0x00) ; // Send 0000 0000 to LCD  
    Write_Cmd(0x0C) ; // Send 0000 1100 to LCD  (Cursor/display shift)
    Write_Cmd(0x00) ; // Send 0000 0000 to LCD  
    Write_Cmd(0x06) ; // Send 0000 0110 to LCD  (Entry mode set))
}

void Display_Char(char c)
{
    unsigned char LowNibble , HighNibble ;
    LowNibble = c & 0x0F ;
    HighNibble = c & 0xF0 ; 
    Write_Data(HighNibble >> 4) ;
    Write_Data(LowNibble) ;
}

void Display_String(string s) 
{
    for ( unsigned char i = 0 ; s[i] != '\0' ; i++ ) Display_Char(s[i]) ;
}

void INIT()
{
    if (!lcdx.openDevice()) {
    cerr << "I2C aygıtı açılamadı." << endl;
    }
    Four_Bit_Init() ;
}

void Display_Cursor(unsigned char state) 
{
    Display_State = Display_State & 0xFD ; 
    Write_Cmd(0x00) ;
    Write_Cmd(Display_State) ;
}

void Display_Cursor_Blinking(unsigned char state) 
{
    Display_State = Display_State & 0xFE ; 
    Write_Cmd(0x00) ;
    Write_Cmd(Display_State) ;
}
void Set_Cursor(unsigned char y , unsigned char x) 
{
    unsigned char temp ;
    unsigned char LowNibble , HighNibble ;

    if ( y == 1 )
    {
        temp = 0x80 + (x - 1) ; 
        HighNibble = temp >> 4 ; 
        LowNibble = temp & 0x0F ; 
        Write_Cmd(HighNibble) ;
        Write_Cmd(LowNibble) ; 
    }
    else if ( y == 2 )
    {
        temp = 0xC0 + x - 1 ; 
        HighNibble = temp >> 4 ;
        LowNibble = temp & 0x0F ; 
        Write_Cmd(HighNibble) ; 
        Write_Cmd(LowNibble) ; 
    }
}
void Clear()
{
    Write_Cmd(0x00) ; // Clear Command 
    Write_Cmd(0x01) ; // Sending 00000001
}
void Home()
{
    Write_Cmd(0x02) ; 
}

void Shift_Right() 
{
    Write_Cmd(0x01) ; 
    Write_Cmd(0x0C) ;
}

void Shift_Left() 
{
    Write_Cmd(0x01) ; 
    Write_Cmd(0x08) ; 
}

unsigned char PIN_P7 ; 
unsigned char PIN_P6 ; 
unsigned char PIN_P5 ;
unsigned char PIN_P4 ; 
unsigned char PIN_P3 = 1;
unsigned char PIN_P2 ;
unsigned char PIN_P1 ; 
unsigned char PIN_P0 ;
unsigned char Display_State = 0x0C ;
};
