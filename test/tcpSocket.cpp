#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <chrono>

using namespace std;

class TCP {
private:
    int socketFD;
    struct sockaddr_in serverAddress;
    const char* ipAddress;
    int port;
    bool autoConnect;
    bool connected = false;
    thread Listener;
    string Rx;
    bool RxArrived = false;
    bool go=false;

public:

    TCP(const char* ipAddress, int port) : socketFD(-1), ipAddress(ipAddress), port(port) {
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = inet_addr(ipAddress);
        serverAddress.sin_port = htons(port);
        autoConnect = true;
        RxArrived = false;
        connected = false;
        Rx = "";
        Listener = thread([this]() { ThreadListen(); });
    }
    ~TCP() {
        closeSocket();
    }

    bool start() {
        socketFD = socket(AF_INET, SOCK_STREAM, 0);
        if (socketFD == -1) {
            cerr << "Hata: Soket oluşturulamadı." << endl;
            return false;
        }

        while (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
            cerr << "Hata: Sunucuya bağlanılamadı. Tekrar deneyecek..." << endl;
            this_thread::sleep_for(chrono::seconds(5));
        }
        connected = true;
        go = true;
        return true;
    }

    void sendString(const string& message) {
        ssize_t bytesSent = send(socketFD, message.c_str(), message.length(), 0);
        if (bytesSent == -1)
            cerr << "Hata: Veri gönderilemedi." << endl;
    }

    string receiveString() {
        const int bufferSize = 1024;
        char buffer[bufferSize];

        ssize_t bytesRead = recv(socketFD, buffer, bufferSize - 1, 0);
        if (bytesRead == -1) {
            cerr << "Hata: Veri alınamadı." << endl;
            return "";
        }

        buffer[bytesRead] = '\0'; // Null karakterle sonlandır
        return string(buffer);
    }

    bool isOpen() const {
        return socketFD != -1;
    }

    void closeSocket() {
        if (socketFD != -1) {
            close(socketFD);
            socketFD = -1;
        }
        connected = false;
    }
    
    void ThreadListen() 
    {
     while(1) {
         this_thread::sleep_for(chrono::milliseconds(1)); 
         if(go ==true) {
     if (!connected)
     {
       if(start());
       else this_thread::sleep_for(chrono::seconds(5));
     } 
     else {
        if(RxArrived == false) {
           string tempRx = receiveString();
           if (!tempRx.empty()) {
               if(tempRx != "") 
               {
                  RxArrived = true; 
                  Rx = tempRx;
               } 
           }
           else closeSocket();
        }
     }
    } }
    }
    
    string getRx() 
    {
        if (Rx == "") return "";
        else 
        {
          string temp = Rx;
          Rx = "";
          RxArrived = false;
          return temp;
        }
    }
};

