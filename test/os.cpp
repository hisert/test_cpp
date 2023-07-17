#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

class os_thread {
public:

os_thread(void (*func)(),int sleeptime,unsigned char onOff)
{
flag_enable= 1;
flag_start= onOff;
sleep_time = sleeptime;
th_ffunct = func;
th = thread(&os_thread::th_fun, this);
}
void funct(void (*func)())
{
th_ffunct = func;
}
void th_fun()
{
 while(1)
 {
  this_thread::sleep_for(chrono::milliseconds(sleep_time));
  if(!flag_enable) break;
  else
  {
   if(flag_start)
   {
    if (th_ffunct != nullptr) th_ffunct();
   }
  }
 }
}
void start()
{
flag_start= 1;
}
void stop()
{
flag_start= 0;
}
void clear()
{
flag_enable= 0;
}
private:
thread th;
unsigned int sleep_time = 0;
unsigned char flag_enable= 0;
unsigned char flag_start= 0;
void (*th_ffunct)() = nullptr;
};

void asd() {
    cout << "Thread 1 çalışıyor..." << endl;
}
void asd2() {
    cout << "Thread 2 çalışıyor..." << endl;
}

int main() {
    os_thread x(asd,100,1);
    os_thread y(asd2,200,1);
    while(1);
    return 0;
}
