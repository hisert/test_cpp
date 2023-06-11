#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>
using namespace std;

string getSystemIPAddress() {
    struct ifaddrs* ifAddrStruct = nullptr;
    struct ifaddrs* ifa = nullptr;
    void* tmpAddrPtr = nullptr;
    string ipAddress;
    if (getifaddrs(&ifAddrStruct) == -1) {
        cerr << "Ağ arabirimleri alınamadı." << endl;
        return ipAddress;
    }
    for (ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // IPv4 adresi
            tmpAddrPtr = &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            string interfaceName(ifa->ifa_name);
            string address(addressBuffer);
            
            // localhost IP adresini atla
            if (address != "127.0.0.1") {
                ipAddress = address;
                break;
            }
        }
    }
    if (ifAddrStruct != nullptr) {
        freeifaddrs(ifAddrStruct);
    }
    return ipAddress;
}
string getCPUtemperature() {
    ifstream file("/sys/class/thermal/thermal_zone0/temp");
    if (!file) {
        cerr << "İşlemci sıcaklık dosyası açılamadı." << endl;
        return "";
    }
    string temperature;
    getline(file, temperature);
    file.close();
    int tempValue = stoi(temperature);
    float cpuTemp = tempValue / 1000.0;

    return to_string(cpuTemp);
}