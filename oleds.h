#ifndef oled_h
#define oled_h

#include "pins.h"

//OledClass OLED;

const int OUTPUT_INDEX_STA = 0;
const int OUTPUT_INDEX_STB = 1;
const int OUTPUT_INDEX_STC = 2;
const int OUTPUT_INDEX_PAN = 3;
const int OUTPUT_INDEX_MTX = 4;

const int INPUT_INDEX_MIC  = 0;
const int INPUT_INDEX_LINE = 1;
const int INPUT_INDEX_BUS  = 2;
const int INPUT_INDEX_TAPE = 3;

// oled positions
int SM_X  = 0;
int SM_Y  = 0;

int LG_X  = 0;
int LG_Y  = 5;

int SMALL_INPUT_X  = 4;
int SMALL_INPUT_Y  = 0;

int SMALL_OUTPUT_X = 8;
int SMALL_OUTPUT_Y = 0;

int LARGE_INPUT_X  = 4;
int LARGE_INPUT_Y  = 5;

int LARGE_OUTPUT_X = 8;
int LARGE_OUTPUT_Y = 5;

int BUS_X    = 0;
int BUS_Y    = 7;

int SMALL_BUS_X    = 0;
int SMALL_BUS_Y    = 3;

int LARGE_BUS_X    = 0;
int LARGE_BUS_Y    = 7;

int CHAN_X    = 8;
int CHAN_Y    = 7;

int PAN_X    = 10;

int ROTARY_X = 0;
int ROTARY_Y = 4;

char* mic_text  = "MIC";
char* line_text = "LINE";
char* tape_text = "TAPE";
char* bus_text  = "BUS";

char* sta_text  = "A ";
char* stb_text  = "B ";
char* stc_text  = "C ";
char* mtx_text  = "MTX";


#endif

