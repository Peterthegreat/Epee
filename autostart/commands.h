#define NO_OF_PROGRAMS          4  //no. of commands

#define DIPLAY_DEFAULT          0
#define DISPLAY_PATH            "../examples/nonconductive/display/send"

//Cycle through each of this files
int display_msg[NO_OF_PROGRAMS] = {1,2,3,4};    //sequence no on the display
#define PROGRAM_PATH 		"../examples/"
char program_folders[NO_OF_PROGRAMS][20]  = {"nonconductive/", "omxplayer/", "omxplayer/", "omxplayer/"};
char program_models[NO_OF_PROGRAMS][20] = {"player","lichid","izolant","test"};

//Autotrain:
#define NO_OF_CLASSES		4
#define NO_OF_SAMPLES		7 //samples per class
