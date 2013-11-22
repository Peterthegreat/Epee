/*
  tuxx.c:
 *
 *	Tux Crossing. A variant on the UK "Pelican" crossing for
 *	pedestrians going over roads.
 *
 *	There is a set of Red, Yellow (sometimes called amber) and
 *	Green traffic lights to control the traffic, and a "Red Man"
 *	and "Green Man" indicators for pedestrians, and a button for
 *	them to push.
 *	Push the button and the lights cycle to Red, then the Green Man
 *	comes on (often with a beeping sound), then afte a short while
 *	the Green man starts to flash, meaning to not start crossing,
 *	and the Yellow traffic light flashes too - meaning that if the
 *	crossing is clear, traffic can pass... Then after a few more seconds
 *	the flashing stops and it revers to Go for traffic and Stop for
 *	pedestrians.
 *
 *	Gordon Henderson, June 2012
 ***********************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include "commands.h"

// The input buttons - see wiringPI pinout
#define	CYCLE_BUTTON		8
#define AUTOTRAIN_BUTTON	4


/*
 * setup:
 *	Program the GPIO correctly and initialise the lamps
 ***********************************************************************
 */

void setup (void)
{
  int i ;

  if (geteuid () != 0)
  {
    fprintf (stderr, "tuxx: Need to be root to run (sudo?)\n") ;
    exit (0) ;
  }

 if (wiringPiSetup () == -1)
    exit (1) ;

  printf ("Setup... ") ; fflush (stdout) ;


  pinMode (CYCLE_BUTTON, INPUT);
  pinMode (AUTOTRAIN_BUTTON, INPUT);

  printf ("OK\n") ;
}


/*
 ***********************************************************************
 * The main program
 *	Call our setup routing once, then sit in a loop, waiting for
 *	the button to be pressed then execute the sequence.
 ***********************************************************************
 */

int main (void)
{
  int program = 0;
  char cmd[1024] = {};
  int started = 0;

  setup () ;

  //initial display
  sprintf(cmd, "%s -s %d", DISPLAY_PATH, display_msg[program]);
  system(cmd);
  printf("Have set diplay: %s.\n", cmd);

  for (;;)
  {
	if( digitalRead(CYCLE_BUTTON) == LOW) {
	   delay(500); //debounce time. TODO: ignore long presses;
	   if( digitalRead(CYCLE_BUTTON) == LOW) { //button is still pressed: start program
		if(!started) {
		   sprintf(cmd, "%s%s%s%s", PROGRAM_PATH, program_folders[program], "play ", program_models[program]);
		   printf("Have started program: %s.\n", cmd);
		   system(cmd);
		   fflush(stdout);
		   started = 1;
		  
		}
	   } else {
		started = 0;

		//button was released, cycle forward
	 	program++;
		if (program >= NO_OF_PROGRAMS)
		   program = 0;

		sprintf(cmd, "%s -s %d", DISPLAY_PATH, display_msg[program]);
		system(cmd);
		printf("Have set diplay: %s.\n", cmd);
		fflush(stdout);
	   }
	   delay(600);
	}

	if(digitalRead(AUTOTRAIN_BUTTON) == LOW) {
	  delay(1000);
	  sprintf(cmd, "%s%s%s %d %s %d", PROGRAM_PATH, program_folders[program], "pcsw -autotrain", NO_OF_CLASSES, program_models[program], NO_OF_SAMPLES);
	  printf("Autotraining: %s", cmd);
	  system(cmd);
	  fflush(stdout);
	}
  }
}
