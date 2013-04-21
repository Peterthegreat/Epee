#include "epee.c"
#

int main(int argc, char **argv) {

	int com;
	char buff[1024];
	FILE *f;

	com=serialOpen("/dev/ttyUSB0");
	f=fdopen(com, "r");
	fgets(buff, 1024, f); //First line usually only comes in half; kill it.
        printf("Serialul pare ok..\n");



	if(argc<2) {
		printf("Usage: play inputfile");
		return 1;
	}


	while(1){
		float class = classifyInput(f, argv[2]);
		printf("Am detectat %i ", class);
	}

}
