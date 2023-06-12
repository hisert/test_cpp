#include <iostream>
#include <cstdlib>
#include "ledAlive.cpp"
#include "i2cLcd.cpp"
#include "tcpSocket.cpp"
#include "serial.cpp"
#include "functs.cpp"
#include <thread>
#include <chrono>
using namespace std;
string systemIP;
string systemTemperature;
LCD lcd;
LED led(22,900,100);
TCP tcp("192.168.1.110", 8080);
SP sp("/dev/ttyS1");
//thread lcd_update;
void process_tx(string data) 
{
 if(data == "<OPI SHUTDOWN>") 
  {
     system("shutdown now");
  }
}
void lcd_Update()
{
    lcd.Clear();
    lcd.Set_Cursor(1,1);
    lcd.Display_String(getSystemIPAddress());
    lcd.Set_Cursor(2,1);
    lcd.Display_String("QTT --> H.i.S");
}
void INIT_led()
{
  led.start();
}
void INIT_lcd()
{
  lcd.INIT();
  lcd_Update();
}
void INIT_tcp()
{
  tcp.start();
}
void INIT_threads()
{
   //lcd_update = thread([]() { lcd_Update(); });
}
void INIT_all()
{
  INIT_led();
  INIT_lcd();
  INIT_tcp();
  INIT_threads();
}
void WHILE_tcp()
{
  string dataArrived = tcp.getRx();
  if(dataArrived != "") 
  {
    process_tx(dataArrived);
    sp.sendString(dataArrived);
  }
}
void WHILE_serial()
{
     if (sp.isOpen()) {
       string txTemp = sp.readString();
       if(txTemp != "") 
      {
        process_tx(txTemp);
        tcp.sendString(txTemp);
      }       
    }
}
void WHILE_all()
{
  while(1)
  {
    this_thread::sleep_for(chrono::milliseconds(1)); 
    WHILE_tcp();
    WHILE_serial();
  }
}
int main()
{  
  INIT_all();
  WHILE_all();
}
