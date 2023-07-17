#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "os.cpp"

using namespace std;

class TCP {
  public:

    TCP(const char * ipAddress, int port): socketFD(-1), ipAddress(ipAddress),port(port)
   {
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(ipAddress);
    serverAddress.sin_port = htons(port);
    autoConnect = true;
    RxArrived = false;
    connected = false;
    Rx = "";
  }
  ~TCP() {
    sert.clear();
  }

  bool open() {
    socketFD = socket(AF_INET, SOCK_STREAM, 0);   
    if (socketFD == -1) {
      cerr << "Hata: Soket oluşturulamadı." << endl;
      return false;
    }
    sert.start();
    return true;
  }

  bool connec() {
    if (connect(socketFD, (struct sockaddr * ) & serverAddress, sizeof(serverAddress)) == -1) {
      connected = false;
      cerr << "Hata: Server baglanamadi" << endl;
      return false;
    }
    connected = true;
    return true;
  }

  void sendString(const string & message) {
    ssize_t bytesSent = send(socketFD, message.c_str(), message.length(), 0);
    if (bytesSent == -1)
      cerr << "Hata: Veri gönderilemedi." << endl;
  }

  string receiveString() {
    const int bufferSize = 1024;
    char buffer[bufferSize];

    ssize_t bytesRead = recv(socketFD, buffer, bufferSize - 1, 0);
    if (bytesRead == -1) {
      closeSocket();
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
    open();
  }

  void thFun() {
        if (!connected) {
          if (connec());
          else this_thread::sleep_for(chrono::seconds(5));
        } else {
          if (RxArrived == false) {
            string tempRx = receiveString();
            if (!tempRx.empty()) {
              if (tempRx != "") {
                RxArrived = true;
                Rx = tempRx;
              }
            }
          }
        }        
  }

  string getRx() {
    if (Rx == "") return "";
    else {
      string temp = Rx;
      Rx = "";
      RxArrived = false;
      return temp;
    }
  }
  private: 
  int socketFD;
  struct sockaddr_in serverAddress;
  const char * ipAddress;
  int port;
  bool autoConnect;
  bool connected = false;
  os_thread sert(thFun,10,0);
  string Rx;
  bool RxArrived = false;
};
