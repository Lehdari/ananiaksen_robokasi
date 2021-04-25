#include <iostream>
#include <string>
#include <sstream>
#include <serialib.h>

#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#if defined (_WIN32) || defined(_WIN64)
    #define SERIAL_PORT "COM1"
#endif
#if defined (__linux__) || defined(__APPLE__)
    #define SERIAL_PORT "/dev/ttyS6"
#endif

int main(int argc, char const *argv[])
{
    // Serial object
    serialib serial;

    // Connection to serial port
    char errorOpening = serial.openDevice(SERIAL_PORT, 9600);

    // If connection fails, return the error code otherwise, display a success message
    if (errorOpening==1)
        printf ("Successful connection to %s\n",SERIAL_PORT);
    else {
        printf("Couldn't establish connection to %s (%d)\n",SERIAL_PORT, (int)errorOpening);
        return -1;
    }

    // Defines initial state
    int state[6] = {90, 90, 90, 90, 90, 120};

    // Send and receive char buffers
    char send[512] = {0};
    unsigned char receive[512] = {0};

    while(true)
    {
        for (int i=0; i<512; ++i) {
            send[i] = 0;
            receive[i] = 0;
        }

        int joint;
        printf("Lol gief joint\n");
        std::cin >> joint;
        printf("Lol gief ankle\n");
        std::cin >> state[joint];

        std::stringstream ss;
        ss << "A" << state[0] << "B" << state[1] << "C" << state[2] << "D" << state[3] << "E" << state[4] << "F" << state[5];

        serial.writeString(ss.str().c_str());
        printf("sent: %s\n", ss.str().c_str());

        serial.flushReceiver();
    }
    return 0;
}
