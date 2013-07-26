#ifndef toastino_h
#define toastino_h

int relay              = 0;
int driverAddress      = 0;

boolean strobeOn = false;

//String inputString = "";
//byte chip;

char inputString[32];

// scene data structure
typedef struct {
    bool pan;
    bool busses[49];
    bool large_outputs[6];    
    bool small_outputs[6];        
    int small_input;
    int large_input;
} channel;

// init scene
channel scene[49];

char inData[100]; // Allocate some space for the string
char inChar=-1;   // Where to store the character read
int  sindex = 0;  // Index into array; where to store the character      

#endif


