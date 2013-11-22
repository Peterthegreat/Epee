#include "epee.c"

int main(int argc, char **argv) {
        int com;
        FILE *f;
        char buff[1024];
        int samps;
        int n;
        com=serialOpen("/dev/ttyUSB0");
        f=fdopen(com, "r");
        fgets(buff, 1024, f); //First line usually only comes in half; kill it.
        printf("Serialul pare ok..\n");

        if (argc<3) {
                printf("Usage: %s [-printavg samps|-gentrain label [no_samples]|-autotrain classes filename [no_samples]|-classify svm-model-prefix]");
                return 1;
        }


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
                for(i = 0; i< atoi(argv[2]); i++) {

                        printf("Ne antrenam pt comanda %i.\n",i);

                        if(argc==4){ //assume default no_sample
                                genLibSvmTrainSet(f, i, argv[3],DEFAULT_SAMPLES);
                        } else {
                                genLibSvmTrainSet(f, i, argv[3],atoi(argv[4]));
                        }

                        if(i< atoi(argv[2]) - 1) {
                                printf("Urmatoarea clasa in 2secunde..");
                                sleep(3);
                        }
                }

                //printf("Dau svm-easy\n");
                //printf(argv[3]);
                //printf("\n");
                char cmd[90];
                sprintf(cmd, "svm-easy %s", argv[3]);
                printf("Comanda e %s",cmd);
                system(cmd);

                //run the new classification
                classifyInput(f, argv[3]);

        } else if (strcmp(argv[1], "-classify")==0) {
                        classifyInput(f, argv[2]);
        } else {
		printf("Incorrect usage\n");
	}
}

