#include <iostream>
#include <string>
#include <sstream>
#include <RS-232/rs232.h>

#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif


int main()
{
    int cport_nr=5;        /* /dev/ttyS0 (COM1 on windows) */
    int bdrate=9600;       /* 9600 baud */

    char mode[]={'8','N','1',0};

    char send[512] = {0};
    unsigned char receive[512] = {0};

    int state[6] = { 90, 90, 90, 90, 90, 120 };

    if(RS232_OpenComport(cport_nr, bdrate, mode, 0))
    {
        printf("Can not open comport\n");

        return(0);
    }

    RS232_flushRXTX(cport_nr);

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

        RS232_cputs(cport_nr, ss.str().c_str());
        printf("sent: %s\n", ss.str().c_str());

        RS232_flushTX(cport_nr);

        RS232_PollComport(cport_nr, receive, 511);

        RS232_flushRX(cport_nr);

        printf("received: %s\n", receive);

#ifdef _WIN32
        Sleep(100);
#else
        usleep(1000000);  /* sleep for 1 Second */
#endif
    }

    return(0);
}
