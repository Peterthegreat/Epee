#include "epee.c"
#define COMANDA_0 
#define COMANDA_1 "mpc play"
#define COMANDA_2 "mpc stop"
#define COMANDA_3 "mpc next"

#define DEBOUNCE_SAMPLES 2


int main(int argc, char **argv) {

	int com;
	char buff[1024];
	FILE *f;

	com=serialOpen("/dev/ttyUSB0");
	f=fdopen(com, "r");
	fgets(buff, 1024, f); //First line usually only comes in half; kill it.
        printf("Serialul pare ok..\n");


	if(argc<2) {
		printf("Usage: play inputfile\n");
		return 1;
	}

	char * modelfile = argv[1];


	//Declarations
	struct svm_model *model;
        struct svm_node features[NOSAMPS+1];
        int samp[NOSAMPS];
        int rmin[NOSAMPS];
        int rmax[NOSAMPS];
        double *decVals;
        int x, noLabels, noDecVals;
        double r;
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

	printf("Am ajuns la bucla..\n");


	while(1){


		//Compute the output
		printf("Getting the samples.. ");
		getSamples(f, samp);
		printf("Got them!\n");

                for (x=0; x<NOSAMPS; x++) {
                        features[x].index=x;
                        //rescale to [-1, 1]
                        r=(samp[x]-rmin[x+1]);
                        r=r/(rmax[x+1]-rmin[x+1]);
                        r=(r*2)-1;
                        features[x+1].value=r;
//                      printf("%f ", features[x].value);
                }
//              printf("\n");
                features[x].index=-1;
                r=svm_predict_values(model,features,decVals);
                printf("prediction value: %f\n", r);



		//Debounce the output
		int class = (int)r;
		static int last_class = 0;

		static int count = 0;

		if (class != last_class) {
			count = 0;
			printf("resetez count..\n");
		} else {
			count ++;
			printf("count = %i \n", count);
		}
		last_class = class;

		if (count == DEBOUNCE_SAMPLES -1) {
			//printf("Am facut debounce si am detectat clasa %f\n", class);

			//actiune pt clasa:
			switch(class){
				case 1: system(COMANDA_1);
					  printf(COMANDA_1);
					break;
				case 2: system(COMANDA_2);
                                        break;
				case 3: system(COMANDA_3);
                                        break;


			 }
		}

	}

}
