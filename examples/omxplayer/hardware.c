//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
#include <wiringPi.h>

#define nextLED		4
#define quitButton	8

void setup_LED_and_button() {
   if(wiringPiSetup () == -1) {
      printf("Error start wiringPi.\n");
      exit(1);
   }

   pinMode(nextLED, OUTPUT);
   digitalWrite(nextLED, 0);

   pinMode(quitButton, INPUT);
}

void check_quitButton() {
   if (digitalRead(quitButton) == LOW) {
      printf("Button detected. Quitting");
      exit(1);
   }
}
