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
thread oled_update;
void process_tx(string data) 
{
 if(data == "<OPI SHUTDOWN>") 
  {
     system("shutdown now");
  }
}
void oled_Update()
{   
//    while(1) 
    {    
    oled.ClearDisplay();
    //oled.Write_Text(0,0,getSystemIPAddress());
    oled.Write_Text(0,8,getSystemIPAddress());
    //oled.Write_Text(0,8,"<------>");
    oled.Write_Text(0,16,"QTT----->HIS");
    oled.Write_Text(0,32,getCPUtemperature());
    oled.Write_Text(0,48,"ELBET");
    oled.Update();
    this_thread::sleep_for(chrono::milliseconds(100)); 
    }
}
void INIT_led()
{
  led.start();
}
void INIT_oled()
{
  oled.INIT();
  oled_Update();
}
void INIT_tcp()
{
  tcp.start();
}
void INIT_threads()
{
   oled_update = thread([]() { oled_Update(); });
}
void INIT_all()
{
  INIT_led();
  INIT_oled();
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
