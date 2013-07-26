/*********************************
*
* test to cycle through boards/oleds
*
*********************************/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// setup oled 
#define OLED_DC 11
#define OLED_CS 12   //No longer needed with multiplexer
#define OLED_CLK 10
#define OLED_MOSI 9
#define OLED_RESET 13

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

//----------------
// configure pins
//----------------
int boardAddressPins[] = { 22, 24, 26, 28 }; //REMOVED PIN 30 
int chipSelectPins[]   = { 32, 34, 36 };
int allSelectPin       = 30;
int strobePin          = 38;

int delaytime = 10;  

String board;
byte chip;
String text;


int ca;
int ba;

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

// scene data structure
typedef struct
{ 
    int channel;
    char name;
    boolean pan;
    int busses[48];
    char *small_input;       // should really use ints mapped to globals for this
    char *small_outputs[4];  // should really use ints mapped to globals for this too
    char *large_input;       // should really use ints mapped to globals for this
    char *large_outputs[4];  // should really use ints mapped to globals for this too

} channel;

// init scene 
channel scene[48];

//----------------
// setup arduino
//----------------
void setup() {
  
    // setup scene data
    
    scene[0].channel          = 2;
    scene[0].name             = 'Snare';
    scene[0].busses[0]        = 1;
//    scene[0].busses[1]        = 2;
//    scene[0].busses[2]        = 3;
//    scene[0].busses[3]        = 4;
//    scene[0].busses[4]        = 5;
//    scene[0].busses[5]        = 6;
//    scene[0].busses[6]        = 7;
//    scene[0].busses[7]        = 8;    
    scene[0].small_input      = "MIC";          
    scene[0].small_outputs[0] = "STA";  
    scene[0].large_input      = "TAPE";          
    scene[0].large_outputs[0] = "STB";  
    scene[0].pan =               false;
    
    scene[1].channel          = 2;
    scene[1].name             = 'Snare';
    scene[1].busses[0]        = 2;
//    scene[1].busses[1]        = 2;
//    scene[1].busses[2]        = 3;
//    scene[1].busses[3]        = 4;
//    scene[1].busses[4]        = 5;
//    scene[1].busses[5]        = 6;
//    scene[1].busses[6]        = 7;
//    scene[1].busses[7]        = 8;    
    scene[1].small_input      = "MIC";          
    scene[1].small_outputs[0] = "STA";  
    scene[1].large_input      = "TAPE";          
    scene[1].large_outputs[0] = "STB";
    scene[1].pan =               true;  
    
    scene[2].channel          = 3;
    scene[2].name             = 'Drum Ovhd L';
    scene[2].busses[0]        = 3;
//    scene[2].busses[1]        = 2;
//    scene[2].busses[2]        = 3;
//    scene[2].busses[3]        = 4;
//    scene[2].busses[4]        = 5;
//    scene[2].busses[5]        = 6;
//    scene[2].busses[6]        = 7;
//    scene[2].busses[7]        = 8;    
    scene[2].small_input      = "MIC";          
    scene[2].small_outputs[0] = "STA";  
    scene[2].large_input      = "TAPE";          
    scene[2].large_outputs[0] = "STB";
    scene[2].pan =               false;  
    
    scene[3].channel          = 4;
    scene[3].name             = 'Drum Ovhd R';
    scene[3].busses[0]        = 4;
//    scene[3].busses[1]        = 10;
//    scene[3].busses[2]        = 11;
//    scene[3].busses[3]        = 12;
//    scene[3].busses[4]        = 13;
//    scene[3].busses[5]        = 14;
//    scene[3].busses[6]        = 15;
//    scene[3].busses[7]        = 16;  
    scene[3].small_input      = "MIC";          
    scene[3].small_outputs[0] = "STA";  
    scene[3].large_input      = "TAPE";          
    scene[3].large_outputs[0] = "STB";
    scene[3].pan =               true;     
  
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
    
     // initialize serial:
    Serial.begin(9600);
    
    // reserve 200 bytes for the inputString:
    inputString.reserve(200);
  
    // set board address pins
    for (int i=0;i<5;i++) {
             pinMode(boardAddressPins[i], OUTPUT);       
        digitalWrite(boardAddressPins[i], LOW);
    }   

    // set chip select pins
    for (int i=0;i<3;i++) {
             pinMode(chipSelectPins[i], OUTPUT);       
        digitalWrite(chipSelectPins[i], LOW);
    }  

//    digitalWrite(strobePin, HIGH); 
    digitalWrite(strobePin, LOW); 
            
//    delay(100); 
    
}    

//----------------------------
// main()
//----------------------------
void loop() {
    
    // print the string when a newline arrives:
    if (stringComplete) {
     
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
            selectBoard(2);      
            
            // select chip on board
            if (DEBUG == true) { Serial.println("5> selecting chip");}            
            chipSelect(ca);
      
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
          
            if (scene[ca].small_outputs[o] == "STA") {
                padding = padding + 15;
                display.drawBitmap(padding, SMALL_OUTPUT_Y, staicon, 15, 16, 1);         
            }  
            if (scene[ca].small_outputs[o] == "STB") {
                padding = padding + 15;   
                display.drawBitmap(padding, SMALL_OUTPUT_Y, stbicon, 15, 16, 1);                         
            }  
            if (scene[ca].small_outputs[o] == "STC") {
                padding = padding + 15;        
                display.drawBitmap(padding, SMALL_OUTPUT_Y, stcicon, 15, 16, 1);                         
            }      
            if (scene[ca].small_outputs[o] == "PAN") {
                padding = padding + 15;      
                display.drawBitmap(padding, SMALL_OUTPUT_Y, panicon, 27, 16, 1);                         
            }    
            if (scene[ca].small_outputs[o] == "MTX") {
                buss_to_small_fader = true;              
            }                            
            display.display();                       
        }

        if (DEBUG == true) { Serial.println("9> handling large oled outputs");}            

        padding = LARGE_OUTPUT_X;

        for (int o=0; o<4; o++) {        
          
            if (scene[ca].large_outputs[o] == "STA") {
                padding = padding + 15;
                display.drawBitmap(padding, LARGE_OUTPUT_Y, staicon, 15, 16, 1);         
            }  
            if (scene[ca].large_outputs[o] == "STB") {
                padding = padding + 15;   
                display.drawBitmap(padding, LARGE_OUTPUT_Y, stbicon, 15, 16, 1);                         
            }  
            if (scene[ca].large_outputs[o] == "STC") {
                padding = padding + 15;        
                display.drawBitmap(padding, LARGE_OUTPUT_Y, stcicon, 15, 16, 1);                         
            }      
            if (scene[ca].large_outputs[o] == "PAN") {
                padding = padding + 15;      
                display.drawBitmap(padding, LARGE_OUTPUT_Y, panicon, 27, 16, 1);                         
            }    
            if (scene[ca].large_outputs[o] == "MTX") {
                buss_to_large_fader = true;              
            }                            
            display.display();                       
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
    
    delay(500);
} 

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

void selectBoard(int boardAddress ) {
  
    // iterate through registers
    for (int i=0; i<4; i++)  {
            
        if ( boardAddress & (1<<i) ) {
            digitalWrite(boardAddressPins[i], HIGH);
        }
        else {
            digitalWrite(boardAddressPins[i], LOW);
        }
    }
}

void chipSelect(int chipAddress ) {
  
    // iterate through registers
    for (int i=0; i<3; i++)  {
            
        if ( chipAddress & (1<<i) ) {

            digitalWrite(chipSelectPins[i], HIGH);
        }
        else {
            digitalWrite(chipSelectPins[i], LOW);
        }
    }
}

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
