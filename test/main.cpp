#include <iostream>
#include <cstdlib>
#include "tcpSocket.cpp"
#include "serial.cpp"
#include "functs.cpp"
#include <thread>
#include <chrono>
#include "oled.cpp"
#include "os.cpp"
using namespace std;
void oled_screen_funct();
void WHILE_serial();
OLED oled;
TCP tcp("192.168.1.110", 8080);
SP sp("/dev/ttyS1");
os_thread os_thread_oled(oled_screen_funct,100,0);
os_thread os_thread_serial(WHILE_serial,10,1);
void process_tx(string data) 
{
 if(data == "<OPI SHUTDOWN>") 
  {
     system("shutdown now");
  }
}
void oled_screen_funct()
{   
    oled.ClearDisplay();
    oled.Rectangle(0,0,127,15,1);
    oled.Rectangle(0,16,127,63,1);
    oled.Write_Text((0 + 15),(0 + 4),getSystemIPAddress());
  //  oled.Write_Text((0 + 4),(16 + 4),GetCurrentDateTime());
    oled.Write_Text((0 + 4),(16 + 4),"TEMP->");
    oled.Write_Text((0 + 4 +50),(16 + 4),getCPUtemperature());  
 //   oled.Write_Text((0 + 4),(48 + 4),"TIME->");
 //   oled.Write_Text((0 + 4 + 50),(48 + 4),getElapsedTimeInSeconds());       
    oled.Update();
}
void INIT_oled()
{
  oled.INIT(128,32,0x3C);
  oled.InvertDisplay(1);
  oled.PrintAtom();
  os_thread_oled.start();
}
void INIT_tcp()
{
  tcp.open();
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
  INIT_oled();
  INIT_tcp();
}
void WHILE_all()
{
  while(1)
  {
    this_thread::sleep_for(chrono::milliseconds(5));
    WHILE_tcp();
  }
}

int main()
{  
  INIT_all();
  WHILE_all();
}
