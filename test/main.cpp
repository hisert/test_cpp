#include <iostream>
#include <cstdlib>
#include "ledAlive.cpp"
#include "tcpSocket.cpp"
#include "serial.cpp"
#include "functs.cpp"
#include <thread>
#include <chrono>
#include "oled.cpp"
using namespace std;
string systemIP;
string systemTemperature;
OLED oled;
LED led(22,900,100);
TCP tcp("192.168.1.110", 8080);
SP sp("/dev/ttyS1");
thread oled_screen;
void process_tx(string data) 
{
 if(data == "<OPI SHUTDOWN>") 
  {
     system("shutdown now");
  }
}
void oled_screen_funct()
{   
    oled.PrintLogo();
   this_thread::sleep_for(chrono::milliseconds(3000)); 
    while(1) 
    {    
    oled.ClearDisplay();
    oled.Rectangle(0,0,127,15,1);
    oled.Rectangle(0,16,127,63,1);
    oled.Write_Text((0 + 15),(0 + 4),getSystemIPAddress());
    oled.Write_Text((0 + 4),(16 + 4),"QTT----->HIS");
    oled.Write_Text((0 + 4),(32 + 4),getCPUtemperature());
    oled.Write_Text((0 + 4),(48 + 4),GetCurrentDateTime());
    oled.Update();
    this_thread::sleep_for(chrono::milliseconds(1000)); 
    }
}
void INIT_led()
{
  led.start();
}
void INIT_oled()
{
  oled.INIT();
  //oled.InvertDisplay(1);
}
void INIT_tcp()
{
  tcp.open();
}
void INIT_threads()
{
   oled_screen = thread([]() { oled_screen_funct(); });
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
void INIT_all()
{
  INIT_led();
  INIT_oled();
  INIT_threads();
  INIT_tcp();
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
