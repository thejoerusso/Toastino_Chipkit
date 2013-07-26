/*********************************
*
* integrated toastino mockup 
*
*********************************/

//#include "Ethernet.h"
#include <OSCBundle.h>
#include <OSCMessage.h>
#include <SLIPEncodedSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// setup OSCBundles
OSCBundle bundle(SLIPSerial);
OSCBundle bundleOUT(SLIPSerial);

// setup oled 
#define OLED_DC 11
#define OLED_CS 12   //No longer needed with multiplexer
#define OLED_CLK 10
#define OLED_MOSI 9
#define OLED_RESET 13

int ca;
int ba;
int delaytime = 10;  

int allSelectPin = 30;

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

boolean DEBUG = false;

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
//--------------------------------------------
int dataPin       = 12;
int clockPin      = 13;
int strobePin     = 11;
int parkingPin    = 23;

int maxAddressPin = 53;

int boardAddressPins[]   = { 40, 42, 44, 46, 48, 50, 52 };
int driverAddressPins[]  = { 32, 34, 36, 38 };

// currently not wired
// int boardAddressPinSeven = 40;

int totalChannels = 48;
int garageChannel     = 0;
int garageNextChannel = 0;
int garageChannelCount[48];

/////////////////////////////////////////////////////
// setup convenience constants for "window" positions
/////////////////////////////////////////////////////
const int MIC_LINE_LARGE_FADER_INDEX      = 0;
const int BUS_TAPE_LARGE_FADER_INDEX      = 1;
const int TOGGLE_LARGE_FADER_INDEX        = 2;
const int TOGGLE_NEXT_LARGE_FADER_INDEX   = 3;
const int BUS_TAPE_NEXT_LARGE_FADER_INDEX = 4;
const int MIC_LINE_NEXT_LARGE_FADER_INDEX = 5;
const int MIC_LINE_SMALL_FADER_INDEX      = 6;
const int BUS_TAPE_SMALL_FADER_INDEX      = 7;
const int TOGGLE_SMALL_FADER_INDEX        = 8;
const int TOGGLE_NEXT_SMALL_FADER_INDEX   = 9;
const int BUS_TAPE_NEXT_SMALL_FADER_INDEX = 10;
const int MIC_LINE_NEXT_SMALL_FADER_INDEX = 11;

// setup "window" incrementer
int cycle = 0;

// scene data structure
typedef struct
{
    int channel;
    char *name;
    bool pan;
    int busses[48];
    char *small_input;
    char *small_outputs;
    char *large_input;
    char *large_outputs;

} channel;

// init scene
channel scene[48];

bool endOfChannels = false;

//--------------------------------------------
// TODO : NEED TO REALLY SET THESE PINS UP
//--------------------------------------------
void setup() {

    Serial.begin(9600);
    
    // reserve 200 bytes for the inputString:
    inputString.reserve(200);

    // Initialize arrays to zeros
    memset(garageChannelCount,0,sizeof(garageChannelCount));

    // setup scene data
    for (int sceneCount=0; sceneCount<48; sceneCount++) {

        scene[sceneCount].channel                 = sceneCount;
        scene[sceneCount].name                    = "test channel";
        scene[sceneCount].busses[sceneCount]      = 1;
        scene[sceneCount].small_input             = "TAPE";
        scene[sceneCount].small_outputs           = "STA";
        scene[sceneCount].large_input             = "LINE";
        scene[sceneCount].large_outputs           = "STB";
        scene[sceneCount].pan                     = false;
    }
    
    pinMode(allSelectPin, OUTPUT);
    pinMode(strobePin, OUTPUT);
    
    digitalWrite(strobePin, HIGH);
    
    //OLED SETUP
    digitalWrite(allSelectPin, LOW); //select all oleds
    display.begin(SSD1306_SWITCHCAPVCC);
    display.display(); // show splashscreen   
    delay(1000); 
    display.clearDisplay();       
    digitalWrite(allSelectPin, HIGH); // deselect all oleds

    pinMode(dataPin  , OUTPUT);
    pinMode(clockPin , OUTPUT);
    pinMode(strobePin, OUTPUT);
    pinMode(parkingPin, OUTPUT);

    // set initial state
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin , HIGH);
    digitalWrite(strobePin , LOW);
    digitalWrite(parkingPin, HIGH);
    
    // initialize pins 32-52 and set to low
    for (int addressPin = 32;addressPin<maxAddressPin;addressPin++) {
        pinMode(addressPin, OUTPUT);
        digitalWrite(addressPin, HIGH);
    }

    // set channel address pins
    for (int boardAddressPin = 0; boardAddressPin<(sizeof(boardAddressPins)/sizeof(int));boardAddressPin++) {
        pinMode(boardAddressPin, OUTPUT);
        digitalWrite(boardAddressPin, LOW);
    }

    // set driver address pins
    for (int driverAddressPin = 0;driverAddressPin<(sizeof(driverAddressPins)/sizeof(int));driverAddressPin++) {
        pinMode(driverAddressPin, OUTPUT);
        digitalWrite(driverAddressPin, LOW);
    }

    digitalWrite(strobePin, LOW);

    delay(10);
}

//--------------------------------------------
// main()
//--------------------------------------------
void loop() {
  
    bundle.clear();
        
    bundleReceive();
  
    if (stringComplete) {
        setGarage();
        setCoffin();
        setOLED();
    }    
}


void bundleReceive(){ 
    if (bundle.receive()>0){
         // "/*"  = "/\052" 
         //bc of a bug in the Arduino IDE, it won't compile with "/*" in a string.
         bundle.route("/d", handleDigital);
         sendBundle();
         bundle.route("/s", handleSystem);
         sendBundle();
         bundle.route("/p", handlePolling);
    }
}

void setOLED() {
  
    if (DEBUG == true) { Serial.println("1> string complete"); }
     
    // setup separators

    //char first_sep  = inputString.indexOf(',');
    // grab items
    if (DEBUG == true) {Serial.println("2> ----------"); }       
    
    //String chip      = inputString;
    
    if (DEBUG == true) { Serial.println("3> converting"); }                    
    //char board_char[board.length() + 1];
    //board.toCharArray(board_char, sizeof(board_char));
    
    //ba = atoi(board_char);
    
    char chip_char[inputString.length() + 1];
    inputString.toCharArray(chip_char, sizeof(chip_char));
    
    ca = atoi(chip_char);
    
//        selectBoard(ba);  

    // cycle through oled boards
//        for (int board = 0; board < 1; board++) {     
        
//            int ba = 1;
        // set strobePin low to allow data flow
        digitalWrite(strobePin, LOW);
        if (DEBUG == true) { Serial.println("4> ---------");}           
        //shiftData(2, 3, boardAddressPins);      
        
        // select chip on board
        if (DEBUG == true) { Serial.println("5> selecting chip");}            
        //chipSelect(ca, 2, chipSelectPins);
  
//            Serial.println(ca);      
        
        // lock strobe
       // digitalWrite(strobePin, HIGH);        
//        } 
       
    
    display.clearDisplay();
            
   /***************************************
    *
    * OLED inputs
    *
    **************************************/       
    // toggle small input icon
    if (DEBUG == true) { Serial.println("6> handling small oled inputs");}            
    
    Serial.flush();
    if (scene[ca].small_input == "MIC") {
        display.drawBitmap(SMALL_INPUT_X, SMALL_INPUT_Y, micicon, 27, 16, 1);    
    }  
    if (scene[ca].small_input == "TAPE") {
        display.drawBitmap(SMALL_INPUT_X, SMALL_INPUT_Y, tapeicon, 34, 16, 1);  
    }  
    if (scene[ca].small_input == "BUS") {
        display.drawBitmap(SMALL_INPUT_X, SMALL_INPUT_Y, bussicon, 35, 16, 1);   
    }      
    if (scene[ca].small_input == "LINE") {
        display.drawBitmap(SMALL_INPUT_X, SMALL_INPUT_Y, lineicon, 29, 16, 1);        
    }      
    display.display();      

    // toggle large input icon      
    Serial.flush();        
    if (DEBUG == true) { Serial.println("7> handling large oled inputs");}           
    if (scene[ca].large_input == "MIC") {
        display.drawBitmap(LARGE_INPUT_X, LARGE_INPUT_Y, micicon, 27, 16, 1);    
    }  
    if (scene[ca].large_input == "TAPE") {
        display.drawBitmap(LARGE_INPUT_X, LARGE_INPUT_Y, tapeicon, 34, 16, 1);   
    }  
    if (scene[ca].large_input == "BUS") {
        display.drawBitmap(LARGE_INPUT_X, LARGE_INPUT_Y, bussicon, 35, 16, 1);    
    }      
    if (scene[ca].large_input == "LINE") {
        display.drawBitmap(LARGE_INPUT_X, LARGE_INPUT_Y, lineicon, 29, 16, 1);     
    }                  
            
    display.display();                       

   /***************************************
    *
    * OLED outputs
    *
    **************************************/       
    boolean buss_to_small_fader = false;
    boolean buss_to_large_fader = false;       
            
    // handle outputs for small fader

    int padding = SMALL_OUTPUT_X;
    Serial.flush();        
    if (DEBUG == true) { Serial.println("8> handling small oled outputs");}            
    
    for (int o=0; o<4; o++) {        
      
        //if (scene[ca].small_outputs[o] == "STA") {
        //    padding = padding + 15;
        //    display.drawBitmap(padding, SMALL_OUTPUT_Y, staicon, 15, 16, 1);         
        //}  
        //if (scene[ca].small_outputs[o] == "STB") {
        //    padding = padding + 15;   
        //    display.drawBitmap(padding, SMALL_OUTPUT_Y, stbicon, 15, 16, 1);                         
        //}  
        //if (scene[ca].small_outputs[o] == "STC") {
        //    padding = padding + 15;        
        //    display.drawBitmap(padding, SMALL_OUTPUT_Y, stcicon, 15, 16, 1);                         
        //}      
        //if (scene[ca].small_outputs[o] == "PAN") {
        //    padding = padding + 15;      
        //    display.drawBitmap(padding, SMALL_OUTPUT_Y, panicon, 27, 16, 1);                         
        //}    
        //if (scene[ca].small_outputs[o] == "MTX") {
        //    buss_to_small_fader = true;              
        //}                            
        //display.display();                       
    }

    if (DEBUG == true) { Serial.println("9> handling large oled outputs");}            

    padding = LARGE_OUTPUT_X;

    for (int o=0; o<4; o++) {        
      
        //if (scene[ca].large_outputs[o] == "STA") {
        //    padding = padding + 15;
        //    display.drawBitmap(padding, LARGE_OUTPUT_Y, staicon, 15, 16, 1);         
        //}  
        //if (scene[ca].large_outputs[o] == "STB") {
        //    padding = padding + 15;   
        //    display.drawBitmap(padding, LARGE_OUTPUT_Y, stbicon, 15, 16, 1);                         
        //}  
        //if (scene[ca].large_outputs[o] == "STC") {
        //    padding = padding + 15;        
        //    display.drawBitmap(padding, LARGE_OUTPUT_Y, stcicon, 15, 16, 1);                         
        //}      
        //if (scene[ca].large_outputs[o] == "PAN") {
        //    padding = padding + 15;      
        //    display.drawBitmap(padding, LARGE_OUTPUT_Y, panicon, 27, 16, 1);                         
        //}    
        //if (scene[ca].large_outputs[o] == "MTX") {
        //    buss_to_large_fader = true;              
        //}                            
        //display.display();                       
    }

    // TODO : add error handling condition here
    //if buss_to_large_fader == true && buss_to_small_fader == true:
    //   wtf!!
    
    //if buss_to_large_fader == false && buss_to_small_fader == false:
    //   wtf!!
       
    if (DEBUG == true) { Serial.println("10> drawing grid");}            

    if (buss_to_small_fader == true) {
        drawGrid("sf");        
    }
    else {
        drawGrid("lf");                  
    }

   /***************************************
    *
    * OLED busses
    *
    **************************************/     
    if (DEBUG == true) { Serial.println("11> handling oled busses");}            
    
    Serial.flush();       
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(BUS_X, BUS_Y);
    
    // TODO : fix this shit
    for (int b=0; b<8; b++) {
        if (scene[ca].busses[b] > 0) {
            display.print(scene[ca].busses[b]);
            display.print(" "); 
        }      
    }
    
    if (scene[ca].pan == true) {
      display.print("P");
    }
      
    display.display();
                              
    // clear the string:
    inputString = "";
    stringComplete = false;
    digitalWrite(strobePin, HIGH); 
}

//--------------------------------------------
// set coffin
//--------------------------------------------
void setCoffin() {
  
    // cycle through channels
    for (int channel = 0; channel < 48; channel++) {

        // set strobePin low to allow data flow
        digitalWrite(strobePin, HIGH);

        // shift data out
        //shiftData(channel, 7, channelAddressPins);

        delay(delaytime);

        // cycle through driver addresse
        for (int driverAddress=0; driverAddress < 7; driverAddress++) {

           shiftData(driverAddress, 3, driverAddressPins);

           //delayMicroseconds(500);

            // NOTE!! check driver relay order.. might have order reversed here!
            // cycle through relay addresses
            for (int relayAddress=0; relayAddress<8; relayAddress++) {


                if (relayAddress == 7) {
                    digitalWrite(dataPin, LOW);  // HEADS UP: INVERSE LOGIC DUE TO BUFFER DRIVER????
                }
                else {
                    digitalWrite(dataPin, HIGH);
                }

                digitalWrite(strobePin, LOW);

                // pulse clock
                delayMicroseconds(10);
                digitalWrite(clockPin, LOW);
                delayMicroseconds(10);
                digitalWrite(clockPin, HIGH);

            }
        }
    }
    delay(2000);  
}

//--------------------------------------------
// set parking garage
//--------------------------------------------
void setGarage() {
  
    while (endOfChannels == false) {
      
        Serial.println("end of channels = False");
        
        /////////////////////////////////////////////////////
        // set strobePin low to allow data flow
        /////////////////////////////////////////////////////
        digitalWrite(strobePin, HIGH);

        /////////////////////////////////////////////////////
        // run through boards!
        /////////////////////////////////////////////////////
        for (int garageBoardAddress=0; garageBoardAddress < 4; garageBoardAddress++) {

            // select board
            shiftData(garageBoardAddress, 7, boardAddressPins);

            /////////////////////////////////////////////////////
            // run through drivers!
            /////////////////////////////////////////////////////
            for (int driverAddress = 0; driverAddress < 9; driverAddress++) {

                // select driver
                shiftData(driverAddress, 3, driverAddressPins);

                /////////////////////////////////////////////////////
                // loop through relays!
                /////////////////////////////////////////////////////
                for (int relay=0; relay<8; relay++) {

                    // logic to set max next channel
                    if (garageChannel < 48) {
                        garageNextChannel = garageChannel + 1;
                    }

                    switch (cycle) {

                        //////////////////////////////////////////////////////
                        // FLIP TOGGLE FOR LARGE FADER
                        //////////////////////////////////////////////////////
                        case TOGGLE_LARGE_FADER_INDEX:

                            Serial.println("-----------------------");
                            Serial.println("LARGE");
                            Serial.println(scene[garageChannel].large_input);

                            if (scene[garageChannel].large_input == "MIC" ) {
                                digitalWrite(dataPin, LOW);
                                Serial.println("TOGGLE_LARGE_FADER_INDEX");
                            }

                            if (scene[garageChannel].large_input == "LINE") {
                                Serial.println("TOGGLE_LARGE_FADER_INDEX");
                                digitalWrite(dataPin, LOW);
                            }
                            break;

                        // case TOGGLE_NEXT_LARGE_FADER_INDEX:
                        //     if (scene[garageNextChannel].large_input == "MIC" || scene[garageChannel].large_input == "LINE") {
                        //         digitalWrite(dataPin, LOW);
                        //         Serial.println("TOGGLE_NEXT_LARGE_FADER_INDEX");
                        // }
                        // break;

                        //////////////////////////////////////////////////////
                        // FLIP TOGGLE FOR SMALL FADER
                        //////////////////////////////////////////////////////
                        case TOGGLE_SMALL_FADER_INDEX:

                            Serial.println("SMALL");
                            Serial.println(scene[garageChannel].small_input);

                            if (scene[garageChannel].small_input == "TAPE" || scene[garageChannel].small_input == "BUS" ) {
                                digitalWrite(dataPin, HIGH);
                                Serial.println("TOGGLE_SMALL_FADER_INDEX");
                            }
                            break;

                        // case TOGGLE_NEXT_SMALL_FADER_INDEX:
                        //     if (scene[garageNextChannel].small_input == "TAPE" || scene[garageChannel].small_input == "BUS" ) {
                        //         digitalWrite(dataPin, LOW);
                        //         // Serial.println("TOGGLE_NEXT_SMALL_FADER_INDEX");
                        //     }
                        //     break;

                        //////////////////////////////////////////////////////
                        // FLIP TOGGLE FOR MIC/LINE LARGE FADER
                        //////////////////////////////////////////////////////
                        case MIC_LINE_LARGE_FADER_INDEX:
                            if (scene[garageChannel].large_input == "MIC") {
                                digitalWrite(dataPin, HIGH);
                                Serial.println("MIC_LINE_LARGE_FADER_INDEX");

                            }
                            break;

                        // case MIC_LINE_NEXT_LARGE_FADER_INDEX:
                        //     if (scene[garageNextChannel].large_input == "MIC") {
                        //         digitalWrite(dataPin, LOW);
                        //         Serial.println("MIC_LINE_NEXT_LARGE_FADER_INDEX");
                        //     }
                        //     break;

                        //////////////////////////////////////////////////////
                        // FLIP TOGGLE FOR MIC/LINE SMALL FADER
                        //////////////////////////////////////////////////////
                        case MIC_LINE_SMALL_FADER_INDEX:
                            if (scene[garageChannel].small_input == "LINE") {
                                digitalWrite(dataPin, LOW);
                                Serial.println("MIC_LINE_SMALL_FADER_INDEX");
                            }
                            break;

                        // case MIC_LINE_NEXT_SMALL_FADER_INDEX:
                        //     if (scene[garageNextChannel].small_input == "LINE") {
                        //         digitalWrite(dataPin, LOW);
                        //         // Serial.println("MIC_LINE_NEXT_SMALL_FADER_INDEX");
                        //     }
                        //     break;

                        //////////////////////////////////////////////////////
                        // FLIP TOGGLE FOR BUS/TAPE LARGE FADER
                        //////////////////////////////////////////////////////
                        case BUS_TAPE_LARGE_FADER_INDEX:
                            if (scene[garageChannel].large_input == "TAPE") {
                                digitalWrite(dataPin, LOW);
                                Serial.println("BUS_TAPE_LARGE_FADER_INDEX");
                            }
                            break;

                        // case BUS_TAPE_NEXT_LARGE_FADER_INDEX:
                        //     if (scene[garageNextChannel].large_input == "TAPE") {
                        //         digitalWrite(dataPin, LOW);
                        //         // Serial.println("BUS_TAPE_NEXT_LARGE_FADER_INDEX");
                        //     }
                        //     break;

                        //////////////////////////////////////////////////////
                        // FLIP TOGGLE FOR BUS/TAPE SMALL FADER
                        //////////////////////////////////////////////////////
                        case BUS_TAPE_SMALL_FADER_INDEX:
                            if (scene[garageChannel].small_input == "TAPE") {
                                digitalWrite(dataPin, LOW);
                                Serial.println("BUS_TAPE_SMALL_FADER_INDEX");
                            }
                            break;

                        // Serial.println(scene[garageChannel].small_input);
                        // case BUS_TAPE_NEXT_SMALL_FADER_INDEX:
                        //     if (scene[garageNextChannel].small_input == "TAPE") {
                        //         digitalWrite(dataPin, LOW);
                        //         // Serial.println("BUS_TAPE_NEXT_SMALL_FADER_INDEX");

                        //     }
                        //     break;

                        //////////////////////////////////////////////////////
                        // SEND "OFF" SIGNAL BY DEFAULT
                        //////////////////////////////////////////////////////
                        default:
                            digitalWrite(dataPin, HIGH);

                    }

                    if (garageChannel < 48) {
                        // Serial.println(garageChannelCount[garageChannel]);
                        // keep track of channel completeness within relay "window"
                        if (garageChannelCount[garageChannel] < 6) {
                            garageChannelCount[garageChannel]++;
                        }
                        else {
                            // Serial.println("GARAGE CHANNEL: ");
                            // Serial.println(garageChannel);

                            // If we finish all 48 channels exit the loop
                            if (garageChannel == 47) {
                                endOfChannels = true;
                                Serial.println("DONE");
                            }
                            garageChannel++;
                        }
                    }

                    //////////////////////////////////////////////////////
                    // lock data flow
                    //////////////////////////////////////////////////////
                    digitalWrite(strobePin, LOW);

                    //////////////////////////////////////////////////////
                    // pulse clock
                    //////////////////////////////////////////////////////
                    // delayMicroseconds(10);
                    delay(1);
                    digitalWrite(clockPin, LOW);
                    // delayMicroseconds(10);
                    delay(1);
                    digitalWrite(clockPin, HIGH);

                    //////////////////////////////////////////////////////
                    // reset the our "window"
                    //////////////////////////////////////////////////////
                    if (cycle == 11) {
                        cycle = 0;
                    }
                    else {
                        cycle++;
                    }
                }
            }

        }
    }  
}

//-----------------------------------------------------------
// serial callback
//-----------------------------------------------------------	
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


//-----------------------------------------------------------
// serial callback
//-----------------------------------------------------------	
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

//-----------------------------------------------------------
// draw grid
//-----------------------------------------------------------	
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



