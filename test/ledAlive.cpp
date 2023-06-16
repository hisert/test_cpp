#include <iostream>
#include <wiringPi.h>
#include <thread>
//#include <chrono>

using namespace std;
class LED
{
public:
    LED(int pin,int ledOpen,int ledClose)
    {
        pin_ = pin;
        ledOpenTime_ = ledOpen;
        ledCloseTime_= ledClose;
        if (wiringPiSetup() == -1)
        {
            cout << "WiringPi initialization failed!" << endl;
            return;
        }
        pinMode(pin_, OUTPUT); 
    }

    void start()
    {
        stopBlinking_ = false;
        LedThread = thread([this]() { led_funct(); });
    }

    void stop()
    {
        stopBlinking_ = true;
        if (LedThread.joinable()) 
        {
        LedThread.join();
        }
    }

private:
    void led_funct()
    {
        while (!stopBlinking_)
        {
            digitalWrite(pin_, LOW);
            sleep(ledOpenTime_/1000);
          //  this_thread::sleep_for(chrono::milliseconds(ledOpenTime_)); 
            digitalWrite(pin_, HIGH);
         //   this_thread::sleep_for(chrono::milliseconds(ledCloseTime_));
            sleep(ledCloseTime_/1000);
        }
    }

    thread LedThread;
    int pin_;
    int ledOpenTime_;
    int ledCloseTime_;
    bool stopBlinking_;
};

