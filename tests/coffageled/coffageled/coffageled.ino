/**************************************
*
* test to cycle through coffin/garage
*
**************************************/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// setup oled
#define OLED_CS 14   //No longer needed with multiplexer
#define OLED_MOSI 2
#define OLED_CLK 3
#define OLED_DC 4
#define OLED_RESET 5

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

//ICONS -------//NEED COMMA AT END ?
static unsigned char __attribute__ ((progmem)) micicon[]={
0xF8,0x04,0x04,0x04,0xF4,0xC4,0x04,0x04,0x84,0x44,0xF4,0x04,0x04,0x04,0xF4,0x04,0x04,0xE4,0x34,0x14,0x14,0x14,0x24,0x04,0x04,0x04,0xF8,0x7F,0x80,0x80,0x80,0x9F,0x81,0x86,0x8C,0x83,0x80,0x9F,0x80,0x80,0x80,0x9F,0x80,0x80,0x8F,0x98,0x90,0x90,0x90,0x88,0x80,0x80,0x80,0x7F
};
//27x14
static unsigned char __attribute__ ((progmem)) lineicon[]={
0xF8,0x04,0x04,0xF4,0x04,0x04,0x04,0x04,0x04,0x04,0xF4,0x04,0x04,0xF4,0x64,0x84,0x04,0x04,0xF4,0x04,0x04,0xF4,0x14,0x14,0x14,0x14,0x04,0x04,0xF8,0x7F,0x80,0x80,0x9F,0x90,0x90,0x90,0x90,0x80,0x80,0x9F,0x80,0x80,0x9F,0x80,0x81,0x82,0x8C,0x9F,0x80,0x80,0x9F,0x91,0x91,0x91,0x90,0x80,0x80,0x7F
};
//29x14

static unsigned char __attribute__ ((progmem)) tapeicon[]={
0xF8,0xA8,0x04,0x14,0x14,0xF4,0x14,0x14,0x04,0x04,0x04,0x04,0xC4,0x34,0xC4,0x04,0x04,0x04,0xF4,0x14,0x14,0x14,0x14,0xE4,0x04,0x04,0xF4,0x14,0x14,0x14,0x14,0x04,0x04,0xF8,0x7F,0xAA,0x80,0x80,0x80,0x9F,0x80,0x80,0x80,0x80,0x98,0x87,0x84,0x84,0x85,0x87,0x98,0x80,0x9F,0x81,0x81,0x81,0x81,0x80,0x80,0x80,0x9F,0x91,0x91,0x91,0x90,0x80,0x90,0x7F
};
//34x14

static unsigned char __attribute__ ((progmem)) bussicon[]={
0xF8,0x04,0x04,0xF4,0x14,0x14,0x14,0xE4,0x04,0x04,0x04,0xF4,0x04,0x04,0x04,0x04,0xF4,0x04,0x04,0xE4,0x14,0x14,0x14,0x24,0x04,0x04,0x04,0xE4,0x14,0x14,0x14,0x24,0x04,0x04,0xF8,0x7F,0x80,0x80,0x9F,0x91,0x91,0x91,0x8E,0x80,0x80,0x80,0x8F,0x90,0x90,0x90,0x90,0x8F,0x80,0x80,0x88,0x91,0x91,0x91,0x8E,0x80,0x80,0x80,0x88,0x91,0x91,0x91,0x8E,0x80,0x80,0x7F
};
//35x14

static unsigned char __attribute__ ((progmem)) staicon[]={
0xF0,0x1C,0x04,0x04,0x04,0x04,0xC4,0x34,0xC4,0x04,0x04,0x04,0x04,0x1C,0xF0,0x7F,0x80,0x80,0x80,0x98,0x87,0x84,0x84,0x85,0x87,0x98,0x80,0x80,0xE0,0x3F
};
//15x14

static unsigned char __attribute__ ((progmem)) stbicon[]={
0xF0,0x1C,0x04,0x04,0x04,0xF4,0x14,0x14,0x14,0xE4,0x04,0x04,0x04,0x1C,0xF0,0x7F,0x80,0x80,0x80,0x80,0x9F,0x91,0x91,0x91,0x8E,0x80,0x80,0x80,0xE0,0x3F
};
//15x14

static unsigned char __attribute__ ((progmem)) stcicon[]={
0xF0,0x1C,0x04,0x04,0xE4,0x34,0x14,0x14,0x14,0x24,0x04,0x04,0x04,0x1C,0xF0,0x7F,0x80,0x80,0x80,0x8F,0x98,0x90,0x90,0x90,0x88,0x80,0x80,0x80,0xE0,0x3F
};
//15x14

static unsigned char __attribute__ ((progmem)) panicon[]={
0xF8,0x04,0x04,0xF4,0x14,0x14,0x14,0x14,0xE4,0x04,0x04,0xC4,0x34,0xC4,0x04,0x04,0x04,0x04,0xF4,0x64,0x84,0x04,0x04,0xF4,0x04,0x04,0xF8,0x7F,0x80,0x80,0x9F,0x81,0x81,0x81,0x81,0x80,0x98,0x87,0x84,0x84,0x85,0x87,0x98,0x80,0x80,0x9F,0x80,0x81,0x82,0x8C,0x9F,0x80,0x80,0x7F
};
//27x14

// oled positions
int SMALL_INPUT_X  = 20;
int SMALL_INPUT_Y  = 0;

int SMALL_OUTPUT_X = 20;
int SMALL_OUTPUT_Y = 20;

int LARGE_INPUT_X  = SMALL_INPUT_X + 64;
int LARGE_INPUT_Y  = 0;

int LARGE_OUTPUT_X = SMALL_OUTPUT_X + 64;
int LARGE_OUTPUT_Y = 20;

int BUS_X    = 0;
int BUS_Y    = 54;

//--------------------------------------------
// configure pins
// TODO : NEED TO REALLY SET THESE PINS UP
//--------------------------------------------

int strobePin     = 11;
int dataPin       = 12;
int clockPin      = 13;
int parkingPin    = 23;
int allSelectPin  = 30;
int OLEDstrobePin = 31;

int boardAddressPins[]  = { 40, 42, 44, 46, 48, 50, 52 };
int driverAddressPins[] = { 32, 34, 36, 38 };

// currently not wired
// int boardAddressPinSeven = 40;

int garageChannel      = 0;
int garageNextChannel  = 0;
int garageBoardAddress = 0;
int empty              = 0;
int relay              = 0;
int driverAddress      = 0;

boolean strobeOn = false;

int coffinBusCount = 0;

boolean DEBUG = false;

String board;
byte chip;
String text;

//////////////////////////////////////////////////////////////
// setup convenience constants for garage "window" positions
//////////////////////////////////////////////////////////////
int MIC_LINE_LARGE_FADER_INDEX      = 0;
int BUS_TAPE_LARGE_FADER_INDEX      = 1;
int TOGGLE_LARGE_FADER_INDEX        = 2;
int TOGGLE_NEXT_LARGE_FADER_INDEX   = 3;
int BUS_TAPE_NEXT_LARGE_FADER_INDEX = 4;
int MIC_LINE_NEXT_LARGE_FADER_INDEX = 5;
int MIC_LINE_SMALL_FADER_INDEX      = 6;
int BUS_TAPE_SMALL_FADER_INDEX      = 7;
int TOGGLE_SMALL_FADER_INDEX        = 8;
int TOGGLE_NEXT_SMALL_FADER_INDEX   = 9;
int BUS_TAPE_NEXT_SMALL_FADER_INDEX = 10;
int MIC_LINE_NEXT_SMALL_FADER_INDEX = 11;

//////////////////////////////////////////////////////////////
// setup convenience constants for coffin "window" positions
//////////////////////////////////////////////////////////////
int LARGE_A_OUTPUT = 0;
int LARGE_B_OUTPUT = 1;
int LARGE_C_OUTPUT = 2;

int SMALL_A_OUTPUT = 3;
int SMALL_B_OUTPUT = 4;
int SMALL_C_OUTPUT = 5;

int PAN_OUTPUT     = 6;
int MATRIX_OUTPUT  = 7;   // toggle

// scene data structure
typedef struct
{
    int channel;
    char *name;
    bool pan;
    bool busses[48];
    char *small_input;
    char *small_output;
    char *large_input;
    char *large_output;

} channel;

// init scene
channel scene[48];

void setup() {

    Serial.begin(9600);

    initializeScenes();

    // setup pin modes
    pinMode(dataPin    , OUTPUT);
    pinMode(clockPin   , OUTPUT);
    pinMode(strobePin  , OUTPUT);
    pinMode(parkingPin , OUTPUT);
    pinMode(allSelectPin , OUTPUT);
    pinMode(OLEDstrobePin , OUTPUT);

    //pinMode(52 , OUTPUT);
    //digitalWrite(52, LOW);

    // set initial state
    digitalWrite(clockPin   , LOW);
    digitalWrite(dataPin    , HIGH);
    digitalWrite(strobePin  , HIGH);
    digitalWrite(OLEDstrobePin, HIGH);
    digitalWrite(parkingPin , HIGH);

    //OLED SETUP
    digitalWrite(allSelectPin, LOW); //select all oleds
    display.begin(SSD1306_SWITCHCAPVCC);
    display.display(); // show splashscreen
    delay(1000);
    display.clearDisplay();
    digitalWrite(allSelectPin, HIGH); // deselect all oleds

     // initialize serial:
    Serial.begin(9600);

    // reserve 200 bytes for the inputString:
    inputString.reserve(200);

    // set channel address pins
    for (int boardAddressPin = 0; boardAddressPin<7;boardAddressPin++) {
        pinMode(boardAddressPins[boardAddressPin], OUTPUT);
        digitalWrite(boardAddressPins[boardAddressPin], LOW);
    }

    // set driver address pins
    for (int driverAddressPin = 0;driverAddressPin<4;driverAddressPin++) {

        pinMode(driverAddressPins[driverAddressPin], OUTPUT);
        digitalWrite(driverAddressPins[driverAddressPin], LOW);
    }

    digitalWrite(strobePin, HIGH);

    strobeOn = true;
}

//--------------------------------------------
// main()
//--------------------------------------------
void loop() {

    setGarage();

    delay(500);

    setCoffin();

    delay(100);

    setOLEDS();

    delay(30000);
}

void setOLEDS() {


    int channelCounter = 0;
    boolean buss_to_small_fader = false;
    boolean buss_to_large_fader = false;

    // handle outputs for small fader
    int smallPadding = SMALL_OUTPUT_X;
    int largePadding = LARGE_OUTPUT_X;

    // cycle through oled boards
    for (int board = 0; board < 6; board++) {
        Serial.println("On Board");
        Serial.println(board);

        // set strobePin low to allow data flow
        digitalWrite(OLEDstrobePin, LOW);
        shiftData(board, 3, boardAddressPins);

        // cycle through oled chips
        for (int chip=0; chip<8; chip++) {
            Serial.println("On Chip");
            Serial.println(chip);

            // select chip on board
            shiftData(chip, 2, driverAddressPins);
            display.clearDisplay();

            // Serial.flush();
            // SMALL INPUTS
            if (scene[channelCounter].small_input == "MIC") {
                display.drawBitmap(SMALL_INPUT_X, SMALL_INPUT_Y, micicon, 27, 16, 1);
            }
            if (scene[channelCounter].small_input == "TAPE") {
                display.drawBitmap(SMALL_INPUT_X, SMALL_INPUT_Y, tapeicon, 34, 16, 1);
            }
            if (scene[channelCounter].small_input == "BUS") {
                display.drawBitmap(SMALL_INPUT_X, SMALL_INPUT_Y, bussicon, 35, 16, 1);
            }
            if (scene[channelCounter].small_input == "LINE") {
                display.drawBitmap(SMALL_INPUT_X, SMALL_INPUT_Y, lineicon, 29, 16, 1);
            }

            display.display();

            // LARGE INPUTS
            if (scene[channelCounter].large_input == "MIC") {
                display.drawBitmap(LARGE_INPUT_X, LARGE_INPUT_Y, micicon, 27, 16, 1);
            }
            if (scene[channelCounter].large_input == "TAPE") {
                display.drawBitmap(LARGE_INPUT_X, LARGE_INPUT_Y, tapeicon, 34, 16, 1);
            }
            if (scene[channelCounter].large_input == "BUS") {
                display.drawBitmap(LARGE_INPUT_X, LARGE_INPUT_Y, bussicon, 35, 16, 1);
            }
            if (scene[channelCounter].large_input == "LINE") {
                display.drawBitmap(LARGE_INPUT_X, LARGE_INPUT_Y, lineicon, 29, 16, 1);
            }

            display.display();

            // SMALL OUTPUT
            if (scene[channelCounter].small_output == "STA") {
                smallPadding += 15;
                display.drawBitmap(smallPadding, SMALL_OUTPUT_Y, staicon, 15, 16, 1);
            }
            if (scene[channelCounter].small_output == "STB") {
                smallPadding += 15;
                display.drawBitmap(smallPadding, SMALL_OUTPUT_Y, stbicon, 15, 16, 1);
            }
            if (scene[channelCounter].small_output == "STC") {
                smallPadding += 15;
                display.drawBitmap(smallPadding, SMALL_OUTPUT_Y, stcicon, 15, 16, 1);
            }
            if (scene[channelCounter].small_output == "PAN") {
                smallPadding += 15;
                display.drawBitmap(smallPadding, SMALL_OUTPUT_Y, panicon, 27, 16, 1);
            }
            if (scene[channelCounter].small_output == "MTX") {
                buss_to_small_fader = true;
            }

            display.display();

            // LARGE OUTPUT
            if (scene[channelCounter].large_output == "STA") {
                largePadding += 15;
                display.drawBitmap(largePadding, LARGE_OUTPUT_Y, staicon, 15, 16, 1);
            }
            if (scene[channelCounter].large_output == "STB") {
                largePadding += 15;
                display.drawBitmap(largePadding, LARGE_OUTPUT_Y, stbicon, 15, 16, 1);
            }
            if (scene[channelCounter].large_output == "STC") {
                largePadding += 15;
                display.drawBitmap(largePadding, LARGE_OUTPUT_Y, stcicon, 15, 16, 1);
            }
            if (scene[channelCounter].large_output == "PAN") {
                largePadding += 15;
                display.drawBitmap(largePadding, LARGE_OUTPUT_Y, panicon, 27, 16, 1);
            }
            if (scene[channelCounter].large_output == "MTX") {
                buss_to_large_fader = true;
            }

            display.display();

            // BUSSES
            if (buss_to_small_fader == true) {
                drawGrid("sf");
            }
            else {
                drawGrid("lf");
            }

            display.display();

            // Serial.flush();
            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.setCursor(BUS_X, BUS_Y);

            // TODO : fix this shit
            if (scene[channelCounter].busses[channelCounter] > 0) {
                display.print(scene[channelCounter].busses[channelCounter]);
                display.print(" ");
            }

            display.display();

            if (scene[channelCounter].pan == true) {
              display.print("P");
            }

            display.display();
            // clear the string:
            inputString = "";
            stringComplete = false;

            display.display();
            channelCounter++;
            Serial.println("CHANNEL COUNT");
            Serial.println(channelCounter);

        }

        digitalWrite(OLEDstrobePin, HIGH);
    // TODO : add error handling condition here
    //if buss_to_large_fader == true && buss_to_small_fader == true:
    //   wtf!!
    }
    //if buss_to_large_fader == false && buss_to_small_fader == false:
    //   wtf!!

}

//--------------------------------------------
// setGarage()
//--------------------------------------------
void setGarage() {

    Serial.println("in setGarage()");

    // flip on parking-garage
    digitalWrite(parkingPin, LOW);
    digitalWrite(parkingPin, HIGH);

    digitalWrite(strobePin, HIGH);

    // select board
    shiftData(garageBoardAddress, 7, boardAddressPins);

    // shift out driver
    shiftData(driverAddress, 3, driverAddressPins);

    digitalWrite(strobePin, LOW);
    strobeOn = false;

    while(garageChannel < 47) {

        //Serial.println(garageChannel);

        /////////////////////////////////////////////////////
        // run through boards
        /////////////////////////////////////////////////////
        while (garageBoardAddress < 4) {

            /////////////////////////////////////////////////////
            // run through channel groups
            /////////////////////////////////////////////////////
            for (int cycleGroup = 0; cycleGroup < 6; cycleGroup++) {

                // TODO : verify channel increment logic
                if (garageChannel < 48) {
                    garageChannel     = garageNextChannel + 1;
                    garageNextChannel = garageChannel     + 1;
                }

                /////////////////////////////////////////////////////
                // loop through cycle-window
                /////////////////////////////////////////////////////
                for (int cycle=0; cycle < 12; cycle++) {

                    //----------------------------------------------
                    // toggle large fader
                    //----------------------------------------------
                    if (cycle == TOGGLE_LARGE_FADER_INDEX) {
                        if (scene[garageChannel].large_input == "MIC") {
                            relayOn();
                        }
                        else if (scene[garageChannel].large_input == "LINE") {
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    //----------------------------------------------
                    // mic/line large fader
                    //----------------------------------------------
                    else if (cycle == MIC_LINE_LARGE_FADER_INDEX) {
                        if (scene[garageChannel].large_input == "LINE") {
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    //----------------------------------------------
                    // bus/tape large fader
                    //----------------------------------------------
                    else if (cycle == BUS_TAPE_LARGE_FADER_INDEX) {
                        if (scene[garageChannel].large_input == "TAPE") {
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    //----------------------------------------------
                    // toggle large fader
                    //----------------------------------------------
                    else if (cycle == TOGGLE_SMALL_FADER_INDEX) {
                        if (scene[garageChannel].small_input == "BUS") {
                            relayOn();
                        }
                        else if (scene[garageChannel].small_input == "TAPE") {
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    //----------------------------------------------
                    // mic/line small fader
                    //----------------------------------------------
                    else if (cycle == MIC_LINE_SMALL_FADER_INDEX) {
                        if (scene[garageChannel].small_input == "LINE") {
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    //----------------------------------------------
                    // bus/tape small fader
                    //----------------------------------------------
                    else if (cycle == BUS_TAPE_SMALL_FADER_INDEX) {
                        if (scene[garageChannel].small_input == "TAPE") {
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    //----------------------------------------------
                    // toggle next large fader
                    //----------------------------------------------
                    if (cycle == TOGGLE_NEXT_LARGE_FADER_INDEX) {
                        if (scene[garageNextChannel].large_input == "MIC") {
                            relayOn();
                        }
                        else if (scene[garageNextChannel].large_input == "LINE") {
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    //----------------------------------------------
                    // mic/line next large fader
                    //----------------------------------------------
                    else if (cycle == MIC_LINE_NEXT_LARGE_FADER_INDEX) {
                        if (scene[garageNextChannel].large_input == "LINE") {
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    //----------------------------------------------
                    // bus/tape next large fader
                    //----------------------------------------------
                    else if (cycle == BUS_TAPE_NEXT_LARGE_FADER_INDEX) {
                        if (scene[garageNextChannel].large_input == "TAPE") {
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    //----------------------------------------------
                    // toggle next large fader
                    //----------------------------------------------
                    else if (cycle == TOGGLE_NEXT_SMALL_FADER_INDEX) {
                        if (scene[garageNextChannel].small_input == "BUS") {
                            relayOn();
                        }
                        else if (scene[garageNextChannel].small_input == "TAPE") {
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    //----------------------------------------------
                    // mic/line next small fader
                    //----------------------------------------------
                    else if (cycle == MIC_LINE_NEXT_SMALL_FADER_INDEX) {
                        if (scene[garageNextChannel].small_input == "LINE") {
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    //----------------------------------------------
                    // bus/tape next small fader
                    //----------------------------------------------
                    else if (cycle == BUS_TAPE_NEXT_SMALL_FADER_INDEX) {
                        if (scene[garageNextChannel].small_input == "TAPE") {
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    //////////////////////////////////////////////////////
                    // reset relay count, increment driver, set strobe
                    //////////////////////////////////////////////////////
                    if (relay == 7) {

                        relay = 0;

                        digitalWrite(strobePin, HIGH);

                        delayMicroseconds(750);

                        if (driverAddress == 8) {
                            driverAddress = 0;

                            garageBoardAddress++;
                            shiftData(garageBoardAddress, 7, boardAddressPins);
                        }
                        else {
                            driverAddress = driverAddress + 1;
                        }

                        shiftData(driverAddress, 3, driverAddressPins);

                        delayMicroseconds(50);

                        digitalWrite(strobePin, LOW);
                        strobeOn = false;

                    }
                    else {
                        relay = relay + 1;
                    }
                }
            }
        }
    }
}

//--------------------------------------------
// setCoffin()
//--------------------------------------------
void setCoffin() {

    Serial.println("in setCoffin()");

    digitalWrite(parkingPin, LOW);

    // cycle through channels
    for (int channel = 0; channel < 48; channel++) {

        // set strobePin low to allow data flow
        digitalWrite(strobePin, HIGH);

        // shift data out
        shiftData(channel, 7, boardAddressPins);

        delay(10);

        // reset bus count for each channel
        coffinBusCount = 0;

        // cycle through driver addresse
        for (int driverAddress=0; driverAddress < 7; driverAddress++) {

           shiftData(driverAddress, 3, driverAddressPins);

            // cycle through relay addresses
            for (int relayAddress=0; relayAddress<8; relayAddress++) {

                // set bus for channel
                if (driverAddress < 6) {
                    if (scene[channel].busses[coffinBusCount] == true) {
                        relayOn();
                    }
                    else {
                        relayOff();
                    }
                }
                else {

                    if (relayAddress == LARGE_A_OUTPUT) {
                        if (scene[channel].large_output == "STA") {
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    if (relayAddress == LARGE_B_OUTPUT) {
                        if (scene[channel].large_output == "STB") {
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    if (relayAddress == LARGE_C_OUTPUT) {
                        if (scene[channel].large_output == "STC") {
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    if (relayAddress == SMALL_A_OUTPUT) {
                        if (scene[channel].small_output == "STA") {
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    if (relayAddress == SMALL_B_OUTPUT) {
                        if (scene[channel].small_output == "STB") {
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    if (relayAddress == SMALL_C_OUTPUT) {
                        if (scene[channel].small_output == "STC") {
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    if (relayAddress == PAN_OUTPUT) {
                        if (scene[channel].small_output == "PAN") {
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    if (relayAddress == MATRIX_OUTPUT) {
                        if (scene[channel].small_output == "MTX") {
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }
                }
                digitalWrite(strobePin, LOW);

                coffinBusCount++;
            }
        }
    }
    delay(2000);
}

void initializeScenes() {

        // setup scene data
    for (int sceneCount=0; sceneCount < 48; sceneCount++) {

        scene[sceneCount].channel                 = sceneCount;
        scene[sceneCount].name                    = "test channel";

        // set busses 1 & 2 on channels 1-4

        if (sceneCount < 32) {
            scene[sceneCount].busses[28]      = true;
            scene[sceneCount].busses[40]      = true;
            scene[sceneCount].busses[47]      = true;
        }
        else {
            for (int s=0;s<48;s++) {
                if (s != 28 || s != 40 || s != 47) {
                    scene[sceneCount].busses[s] = false;
                }
            }
        }

        scene[sceneCount].small_input             = "MIC";
        scene[sceneCount].small_output            = "STA";
        scene[sceneCount].large_input             = "TAPE";
        scene[sceneCount].large_output            = "MTX";
        scene[sceneCount].pan                     = false;
    }

}
//--------------------------------------------
// turn relay on / pulse clock
//--------------------------------------------
void relayOn() {

    if (strobeOn) {
        digitalWrite(strobePin, LOW);
    }

    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, LOW);
    delayMicroseconds(250);
    digitalWrite(clockPin, HIGH);
    delayMicroseconds(250);
}

//--------------------------------------------
// turn relay off / pulse clock
//--------------------------------------------
void relayOff() {

    if (strobeOn) {
        digitalWrite(strobePin, LOW);
    }

    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, HIGH);
    delayMicroseconds(250);
    digitalWrite(clockPin, HIGH);
    delayMicroseconds(250);
}

//--------------------------------------------
// shift data out
//--------------------------------------------
void shiftData(int data, int maxRegister, int pins[] ) {

    for (int i=0; i<=maxRegister;i++)  {

        if ( data & (1<<i) ) {
            digitalWrite(pins[i], HIGH);
        }
        else {
            digitalWrite(pins[i], LOW);
        }
    }
}

//--------------------------------------------
// draw grid on oleds
//--------------------------------------------
void drawGrid(char *fader) {

    // draw vertical line
    display.drawLine(64, 0, 64, 48, WHITE);

    if (fader == "sf") {
        display.drawLine(0, 48, 64, 48, WHITE);
    }
    else {
        display.drawLine(65, 48, 128, 48, WHITE);
    }
}

//--------------------------------------------
// serial event callback
//--------------------------------------------
void serialEvent() {

    while (Serial.available()) {

      // get the new byte:
        char inChar = (char)Serial.read();

        // add it to the inputString:
        inputString += inChar;

        // if the incoming character is a newline, set a flag
        // so the main loop can do something about it:
        if (inChar == '\n') {
            stringComplete = true;
            Serial.flush();
        }
    }
}
