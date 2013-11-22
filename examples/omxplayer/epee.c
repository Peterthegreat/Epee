/*
Software for EnGarde, a gesture classifying touch sensor based on the
Pittsburg Disney Research Labs Touche-sensor.
(C) 2012 Jeroen Domburg (jeroen AT spritesmods.com)

This program is free software: you can redistribute it and/or modify
t under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
	    
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
			    
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <fcntl.h>
#include <termios.h>
#include <libsvm/svm.h>
#include "kbhit.h"
#include "commands.h"

#define TTYPORT "/dev/ttyUSB0"
#define BAUDRATE B19200

#define NOSAMPS 32
#define DEFAULT_SAMPLES 12

//int com;

int serialOpen(char *port) {
	struct termios oldtio,newtio;
	int com;
	com=open(port, O_RDWR | O_NOCTTY);
	if (com<0) {
		perror("Opening comport"); 
		exit(1);
	}
	tcgetattr(com,&oldtio); // save current port settings
	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag =  (CS8 | CREAD | CLOCAL);
	newtio.c_lflag = 0;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;
	cfsetospeed(&newtio,BAUDRATE);
	newtio.c_cc[VTIME]    = 0;   // inter-character timer unused 
	newtio.c_cc[VMIN]     = 1;   // blocking read until 1 chars received 
	tcflush(com,TCIFLUSH);
	tcsetattr(com,TCSANOW,&newtio);
	return com;
}

void getSamples(FILE *f, int samp[]) {
	char buff[1024];
	int n, valid=0;
	int min, max;
	char *endp=buff;
	int com;
	int y = 0;

	while (!valid) {
		y++;
		if (y > 3) {	//something's wrong. try to open again
			//system("pwd");
			sprintf(buff, "%s", COMANDA_EROARE);
			printf("Am trimis  %d.\n", buff);
			system(buff);
			exit(1); //TODO: try again

			printf("Strugleing..");
			y = 0;
			fclose(f);
//			close(com);
			com = serialOpen(TTYPORT);
			f = fdopen(com, "r");
			fflush(f);
			fgets(buff, 1024, f);	//first line comes in half. Dump it.
		}

		printf("%i Inainte de fgets, ", y);
		fflush(stdout);
		fgets(buff, 1024, f);
		printf("Dupa fgets.");
		valid=1;
		for (n=0; n<NOSAMPS; n++) {
			samp[n]=strtol(endp, &endp, 10);
			if (samp[n]==0) valid=0;
			endp++; //skip space
			printf("%d ", samp[n]);
		}
		printf("\n");
	}

	//Rescale so everything is in [0,1024]: we don't want amplitude as a variable (dry hands, ...)
	//Commented out: seems to fail more than it helps... :/
/*
	min=1024; max=0;
	for (n=0; n<NOSAMPS; n++) {
		if (samp[n]<min) min=samp[n];
		if (samp[n]>max) max=samp[n];
	}
	for (n=0; n<NOSAMPS; n++) {
		samp[n]=((float)(samp[n]-min)/(float)(max-min))*1024;
	}
*/
}


void printAvgValsOver(FILE *f, int loops) {
	int x,y;
	int samp[NOSAMPS];
	int avg[NOSAMPS];
	for (x=0; x<NOSAMPS; x++) avg[x]=0;
	for (x=0; x<loops; x++) {
		getSamples(f, samp);
		for (y=0; y<NOSAMPS; y++) avg[y]+=samp[y];
	}
	for (x=0; x<NOSAMPS; x++) {
		printf("%i\n", avg[x]/loops);
	}
}


void genLibSvmTrainSet(FILE *f, int label, char* output, int samples) {
	int y;
	int samp[NOSAMPS];
	char buff[10240];
	char valstr[1024];
	printf("Am ajuns in genLibSvmTrainSet(..)\n");
	FILE *out_file;
	out_file = fopen(output, "a+");
	//printf("Samples ramase:%i\n", samples);
	//printf("Condifie: \n", (samples>0));
	printf("Am deschis fisierul de iesire.\n");
	while(!kbhit() && (samples>0)) {
		printf("Incerc citirea de pe atmega");
		getSamples(f, samp);

		printf("Am terminat citirea de pe atmega");
		sprintf(buff, "%i", label);
		for (y=0; y<NOSAMPS; y++) {
			sprintf(valstr, " %i:%i", y+1, samp[y]);
			strcat(buff, valstr);
		}
		fprintf(out_file,"%s\n", buff);
		printf("Am scris in fisier!\n");
		printf("%s\n", buff);
		samples--;
	}

	printf("Inchid fisierul de iesire");
	fclose(out_file);
	printf("Done");
}

void loadRangeFile(char *rangeFile, int rmin[], int rmax[]) {
	FILE *f;
	int i, min, max;
	char buff[1024];
	f=fopen(rangeFile, "r");
	if (f==NULL) {
		perror("loading rangefile");
		exit(1);
	}
	fgets(buff, 1024, f); //first line is bogus
	fgets(buff, 1024, f); //second line is bogus
	while(fscanf(f, "%d %d %d\n", &i, &min, &max)>0) {
		if (i>0) {
			rmin[i]=min;
			rmax[i]=max;
			printf("%d %d %d\n", i, rmin[i], rmax[i]);
		}
	}
	fclose(f);
}


float classifyInput(FILE *f, char *modelfile) {
	struct svm_model *model;
	struct svm_node features[NOSAMPS+1];
	int samp[NOSAMPS];
	int rmin[NOSAMPS];
	int rmax[NOSAMPS];
	double *decVals;
	int x, noLabels, noDecVals;
	double r;
	char buff[1024];

	strcpy(buff, modelfile);
	strcat(buff, ".range");
	loadRangeFile(buff, rmin, rmax);

	strcpy(buff, modelfile);
	strcat(buff, ".model");
	model=svm_load_model(buff);
	if (model==NULL) {
		printf("Couldn't load model!\n");
		exit(0);
	}
	noLabels=svm_get_nr_class(model);
	noDecVals=noLabels*(noLabels-1)/2;
	decVals=malloc(sizeof(double)*noDecVals);

	while(1) {
		getSamples(f, samp);
		for (x=0; x<NOSAMPS; x++) {
			features[x].index=x;
			//rescale to [-1, 1]
			r=(samp[x]-rmin[x+1]);
			r=r/(rmax[x+1]-rmin[x+1]);
			r=(r*2)-1;
			features[x+1].value=r;
//			printf("%f ", features[x].value);
		}
//		printf("\n");
		features[x].index=-1;
		r=svm_predict_values(model,features,decVals);
		printf("prediction value: %f\n", r);
		return r;
//		for (x=0; x<noDecVals; x++) printf("%f ", decVals[x]);
//		printf("\n");
	}
}
