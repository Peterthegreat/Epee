#include "epee.c"
#include "commands.h"

//#define TTYPORT "/dev/ttyUSB0"  -- is already in epee.c


int main(int argc, char **argv) {
        int com;
        FILE *f;
        char buff[1024];
        int samps;
        int n;

	if (argc < 3) {
                printf("Usage: [-printavg samps|-gentrain label [no_samples]|-autotrain classes filename [no_samples]|-classify svm-model-prefix]\n");
                exit(1);
        }

	//open serial port
        com = serialOpen(TTYPORT);
        f = fdopen(com, "r");
        if( fgets(buff, 1024, f) == NULL) { //First line usually only comes in half; kill it.
		printf("Error reading serial device. No '\n' caracter.");
		return 1;
	}
        printf("Serialul e ok.\n");



        if (strcmp(argv[1], "-printavg")==0) {
                printAvgValsOver(f, atoi(argv[2]));
        } else if (strcmp(argv[1], "-gentrain")==0) {
                if(argc==4) {
                        printf("Am 3 parametrii\n");
                        genLibSvmTrainSet(f, atoi(argv[2]), argv[3],DEFAULT_SAMPLES);
                }else if(argc==5){
                        printf("Am 4 parametrii\n");
                        genLibSvmTrainSet(f, atoi(argv[2]), argv[3],atoi(argv[4]));
                }else{
                        printf("Error");
                }
        } else if (strcmp(argv[1], "-autotrain")==0) {
                int i = 0;
		printf("Trec pe aici\n");
		//remove old files
		char remove[100];
		sprintf(remove, "rm -f %s*",argv[3]);
		printf("Deleted:  %s", remove);
		system(remove);

                for(i = 0; i< atoi(argv[2]); i++) {

                        printf("Ne antrenam pt comanda %i.\n",i);
			fflush(stdout);

			char path[100];
			sprintf(path, "../examples/nonconductive/%s", argv[3]);

                        if (argc == 4){ //assume default no_sample
                                genLibSvmTrainSet(f, i, argv[3],DEFAULT_SAMPLES);
                        } else if (argc == 5){
				printf("Sunt aici");
                                genLibSvmTrainSet(f, i, argv[3],atoi(argv[4]));
                        } else {
				//error
				printf("Usage: -autotrain output_file no_of_samples.\n");
				return 1;
			}

                        if(i< atoi(argv[2]) - 1) {
				char mycmd[100];
				sprintf(mycmd,"%s",COMANDA_NEXT);
                                printf("\n\n\nUrmatoarea clasa in 2secunde..\n");
				fflush(stdout);
				system(mycmd);
				sleep(3);
				//system(COMANDA_PREDEFINITA);
                        }

		 	char chh[200];
                        sprintf(chh,"%s",COMANDA_PREDEFINITA);
			printf("Am trimis comanda  %s\n",chh);
			system(chh);
                }

                //printf("Dau svm-easy\n");
                //printf(argv[3]);
                //printf("\n");
                char cmd2[90];
  		sprintf(cmd2, "svm-easy %s", argv[3]);
                printf("Sending command %s",cmd2);
                system(cmd2);

                //run the new classification
		char cmd3[120];
		sprintf(cmd3, "../examples/nonconductive/play %s", argv[3]);
		printf("Sending command: %s",cmd3);
                system(cmd3);

        } else if (strcmp(argv[1], "-classify")==0) {
                        classifyInput(f, argv[2]);
        } else {
		printf("Incorrect usage\n");
	}
}
