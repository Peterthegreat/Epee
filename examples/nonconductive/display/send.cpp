/*
 Usage: ./send <systemCode> <unitCode> <command>
 Command is 0 for OFF and 1 for ON
 */

#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {

    if(argc > 3) {
	printf("Usage: send [-m message] | [-c BIT] | -s [sequence no].");
	return -1;
    }

    /*
     output PIN is hardcoded for testing purposes
     see https://projects.drogon.net/raspberry-pi/wiringpi/pins/
     for pin mapping of the raspberry pi GPIO connector
     */

    char* parameter = argv[1];

    //start WiringPi
    int PIN = 0;
    if (wiringPiSetup () == -1) return 1;
    RCSwitch mySwitch = RCSwitch();
    mySwitch.enableTransmit(PIN);

   if(parameter[1] == 'c') {
	mySwitch.send(atoi(argv[2]),8);
	printf("Am trimis caracterul %d.", atoi(argv[2]));
	return 0;
   }else if (parameter[1] == 's') {
        mySwitch.send(1,8); //stop
        mySwitch.send(4,8); //sequence
        mySwitch.send(atoi(argv[2]) + '0', 8);
        mySwitch.send(13, 8); //run
	return 0;
   }

//    for(int i = 0; i < sizeof(&message); i++) {
//	mySwitch.send(message+i,8);
//    }

    //char* systemCode = argv[1];
    //int unitCode = atoi(argv[2]);
    //int command  = atoi(argv[3]);
    
    //if (wiringPiSetup () == -1) return 1;
//	printf("sending systemCode[%s] unitCode[%i] command[%i]\n", systemCode, unitCode, command);
//	RCSwitch mySwitch = RCSwitch();
//	mySwitch.enableTransmit(PIN);
    
    return 0;
}
