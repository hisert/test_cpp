#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string>

using namespace std;

class SP {
private:
    int fd; // Dosya tanıtıcısı

public:
    SP(const char* portName) {
        fd = open(portName, O_RDWR | O_NOCTTY | O_NDELAY);
        if (fd == -1) {
            cerr << "Hata: Seri port açılamadı." << endl;
            return;
        }
        struct termios options;
        tcgetattr(fd, &options);
        options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
        options.c_iflag = IGNPAR;
        options.c_oflag = 0;
        options.c_lflag = 0;
        tcflush(fd, TCIFLUSH);
        tcsetattr(fd, TCSANOW, &options);
    }

    ~SP() {
        if (fd != -1)
            close(fd);
    }

    void write(const char* data, size_t length) {
        ssize_t bytesWritten = ::write(fd, data, length);
    }

    void sendString(const string& str) {
        write(str.c_str(), str.length());
    }

    string readString() {
        const int bufferSize = 1024;
        char buffer[bufferSize];

        ssize_t bytesRead = ::read(fd, buffer, bufferSize - 1);
        if (bytesRead == -1) return "";        
        else if (bytesRead == 0) return "";        
        buffer[bytesRead] = '\0'; // Null karakterle sonlandır
        return string(buffer);
    }

    bool isOpen() const {
        return fd != -1;
    }
};