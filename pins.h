#ifndef pins_h
#define pins_h

//--------------------------------------------
// configure pins
//--------------------------------------------

int strobePin     = 11;
int dataPin       = 12;
int clockPin      = 13;
int parkingPin    = 23;
int allSelectPin  = 7;
int OLEDstrobePin = 9;

int boardAddressPins[]  = { 37, 35, 33, 31, 28, 22, 25 };//changed pins for chipkit!!!
int driverAddressPins[] = { 45, 42, 41, 39 }; //changed 43 to 42!!!!

// setup oleds
//#define OLED_CS    53 //RG9
//#define OLED_MOSI  51 //RG8
//#define OLED_CLK   52 //RG6
//#define OLED_DC    10 //RD4
//#define OLED_RESET 8  //RD12
//
//// int SLAVE_SELECT = 53

enum enDigitalPins {
    encoderPinA     = 2,
    encoderPinB     = 3,
    encoderpressPin = 4,
    interruptPin    = 21,
    acknowledgePin  = 20,
    enablePin       = 6
};

#endif


