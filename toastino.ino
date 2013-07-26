//**************************************************************
//
//  Name    : toastino
//  Notes   : translation layer between api console & interface
//
//**************************************************************

//--------------------------------------------
// setup libs
//--------------------------------------------

#include <DSPI.h>
#include <IOShieldOled.h>
#include "coffin.h"
#include "garage.h"
#include "oleds.h"
#include "pins.h"
#include "toastino.h"
#include "encoder.h"

//#include <malloc.h>

#define CHANGE_HEAP_SIZE(size)  __asm__ volatile ("\t.globl _min_heap_size\n\t.equ _min_heap_size, " #size "\n")

CHANGE_HEAP_SIZE(110000); 
//0x1ae00 = 110080 bytes
//0x18000 = 98304bytes (worked OK with 41 CH with 1 BUS)
//0x1b000 = 122880bytes


extern __attribute__((section("linker_defined"))) char _heap;
extern __attribute__((section("linker_defined"))) char _min_heap_size;  




//DSPI2    spi;

//--------------------------------------------
// setup()
//--------------------------------------------
void setup() {

  Serial.begin(28800);
  Serial1.begin(28800);

  //   //set spi speed in oledDriver.c
  //   
  //   //--------------------------------------------
  //   //PRINT HEAP SIZE
  //   //--------------------------------------------
  //   byte * pTopHeap = (byte *) &_heap + (unsigned int) &_min_heap_size; 
  //   Serial.print("Heap: ");
  //   Serial.println ((unsigned int)&_heap, HEX);
  //   Serial.print("Min Heap Size: ");
  //   Serial.println ((unsigned int)&_min_heap_size, HEX);
  //   Serial.print("Bottom of stack: ");
  //   Serial.println((unsigned int) &pTopHeap, HEX);
  //   Serial.print("top of heap: ");
  //   Serial.println((unsigned int) pTopHeap, HEX);
  //   

  initializeScenes();
  setupPins();
  attachInterrupt(5, interrupttrigger, LOW);    

  digitalWrite(allSelectPin, LOW);     // select all OLEDs
  IOShieldOled.begin();  
  IOShieldOled.clearBuffer();
  IOShieldOled.setCursor(0,0);
  IOShieldOled.putString("Lets Be Discrete");
  IOShieldOled.updateDisplay();
  IOShieldOled.setCursor(0,2);
  IOShieldOled.putString("1/24/13");
  IOShieldOled.updateDisplay();
  IOShieldOled.setCursor(0,4);
  IOShieldOled.putString("R.A.D. NYC");
  IOShieldOled.updateDisplay();
  delay(3000);
  IOShieldOled.clearBuffer();
  IOShieldOled.updateDisplay();
  digitalWrite(allSelectPin, HIGH);    // deselect all OLEDs
  IOShieldOled.setCharUpdate(0);

}

//--------------------------------------------
// interrupt trigger for rotary encoder
//--------------------------------------------
void interrupttrigger() {

  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();

  if (interrupt_time - last_interrupt_time > 200) {

    if (encoderToggle == LOW) {
      encoderToggle = HIGH;
    }
    else {
      encoderToggle = LOW;
    }
  }
  last_interrupt_time = interrupt_time;
}

//--------------------------------------------
// main()
//--------------------------------------------
void loop() {  
  //    Serial.println("loop");
  Serial.flush();
  while(!Serial1.available()) ; 

  inChar = (char)Serial1.read(); // Read a character

  if(sindex < 29) {   

    if (inChar == '\n') {                             
      inData[sindex] = '\0';  // Null terminate the string
      parseSerialCommand(inData);
      Serial1.flush();
      Serial.flush();        
      sindex = 0;        
    }
    else {                          
      inData[sindex] = inChar;    // Store it
      sindex++;                   // Increment where to write next
    }                      
  }     
}

//--------------------------------------------
// set OLEDs
//--------------------------------------------
void setOLEDS() {


  Serial.println("OLED");


  int channelCounter = 1;

  // cycle through OLED boards
  for (int board = 0; board < 6; board++) {

    // set strobePin low to allow data flow
    digitalWrite(OLEDstrobePin, LOW);
    shiftData(board, 3, boardAddressPins);

    // cycle through OLED chips
    for (int chip=0; chip<8; chip++) {

      if (channelCounter < 49) {

        // select chip on board
        shiftData(chip, 2, driverAddressPins);
        IOShieldOled.clearBuffer();

        // handle outputs for small fader
        int smallPadding = SMALL_OUTPUT_X;
        int largePadding = LARGE_OUTPUT_X;

        boolean buss_to_small_fader = false;
        boolean buss_to_large_fader = false; 

        IOShieldOled.setCursor(SM_X, SM_Y);
        IOShieldOled.putString("SM:");  

        IOShieldOled.setCursor(LG_X, LG_Y);
        IOShieldOled.putString("LG:");        

        // SMALL INPUTS
        if (scene[channelCounter].small_input == INPUT_INDEX_MIC) {
          IOShieldOled.setCursor(SMALL_INPUT_X, SMALL_INPUT_Y);
          IOShieldOled.putString(mic_text);
        }
        if (scene[channelCounter].small_input == INPUT_INDEX_TAPE) {
          IOShieldOled.setCursor(SMALL_INPUT_X, SMALL_INPUT_Y);
          IOShieldOled.putString(tape_text);
        }
        if (scene[channelCounter].small_input == INPUT_INDEX_BUS) {
          IOShieldOled.setCursor(SMALL_INPUT_X, SMALL_INPUT_Y);
          IOShieldOled.putString(bus_text);              
        }
        if (scene[channelCounter].small_input == INPUT_INDEX_LINE) {
          IOShieldOled.setCursor(SMALL_INPUT_X, SMALL_INPUT_Y);
          IOShieldOled.putString(line_text);       
        }


        // LARGE INPUTS
        if (scene[channelCounter].large_input == INPUT_INDEX_MIC) {
          IOShieldOled.setCursor(LARGE_INPUT_X, LARGE_INPUT_Y);
          IOShieldOled.putString(mic_text);               
        }
        if (scene[channelCounter].large_input == INPUT_INDEX_TAPE) {
          IOShieldOled.setCursor(LARGE_INPUT_X, LARGE_INPUT_Y);
          IOShieldOled.putString(tape_text);
        }
        if (scene[channelCounter].large_input == INPUT_INDEX_BUS) {
          IOShieldOled.setCursor(LARGE_INPUT_X, LARGE_INPUT_Y);
          IOShieldOled.putString(bus_text);
        }
        if (scene[channelCounter].large_input == INPUT_INDEX_LINE) {
          IOShieldOled.setCursor(LARGE_INPUT_X, LARGE_INPUT_Y);
          IOShieldOled.putString(line_text);
        }

        // check for boolean value of small-output assignment
        if (scene[channelCounter].small_outputs[OUTPUT_INDEX_STA] == true) {               
          smallPadding += 1;
          IOShieldOled.setCursor(smallPadding, SMALL_OUTPUT_Y);
          IOShieldOled.putString(sta_text);                
        }
        if (scene[channelCounter].small_outputs[OUTPUT_INDEX_STB] == true) {
          smallPadding += 2;
          IOShieldOled.setCursor(smallPadding, SMALL_OUTPUT_Y);
          IOShieldOled.putString(stb_text);                
        }
        if (scene[channelCounter].small_outputs[OUTPUT_INDEX_STC] == true) {
          smallPadding += 2;
          IOShieldOled.setCursor(smallPadding, SMALL_OUTPUT_Y);
          IOShieldOled.putString(stc_text);                
        }
        // TODO : add logic to set MTX scene data            
        if (scene[channelCounter].small_outputs[OUTPUT_INDEX_MTX] == true) {
          buss_to_small_fader = true;
        }



        // check for boolean value of large-output assignment
        if (scene[channelCounter].large_outputs[OUTPUT_INDEX_STA] == true) {                
          largePadding += 1;
          IOShieldOled.setCursor(largePadding, LARGE_OUTPUT_Y);
          IOShieldOled.putString(sta_text);                                
        }
        if (scene[channelCounter].large_outputs[OUTPUT_INDEX_STB] == true) {
          largePadding += 2;
          IOShieldOled.setCursor(largePadding, LARGE_OUTPUT_Y);
          IOShieldOled.putString(stb_text);                        
        }
        if (scene[channelCounter].large_outputs[OUTPUT_INDEX_STC] == true) {
          largePadding += 2;
          IOShieldOled.setCursor(largePadding, LARGE_OUTPUT_Y);
          IOShieldOled.putString(stc_text);                        
        }
   
                // PRINT BUSSES FOR SMALL FADER     
                if (scene[channelCounter].small_outputs[OUTPUT_INDEX_MTX] == true) {
                  IOShieldOled.setCursor(SMALL_BUS_X, SMALL_BUS_Y);
                  Serial.println("SMALL MTX");
                  if (scene[channelCounter].pan == true) {  
                    IOShieldOled.putString("PAN");
                    IOShieldOled.setCursor(5, SMALL_BUS_Y); 
                  }
                  
                  for (int b =0;b<49;b++) {
                    if (scene[channelCounter].busses[b]) {                                      
                      if (scene[channelCounter].busses[b] == true) {
                        char bus_buffer[2];  //reserve the string space first
                        sprintf(bus_buffer, "%d", b);              
                        IOShieldOled.putString(bus_buffer); 
                        IOShieldOled.putString(" ");
                      }     
                    }  
                  }     
                }
              
                   // PRINT BUSSES FOR LARGE FADER
                   if (scene[channelCounter].large_outputs[OUTPUT_INDEX_MTX] == true) {
                     IOShieldOled.setCursor(LARGE_BUS_X, LARGE_BUS_Y);
                     if (scene[channelCounter].pan == true) {  
                       IOShieldOled.putString("PAN");
                       //largePanPadding += 4;
                       IOShieldOled.setCursor(5, LARGE_BUS_Y);
                     }
                    
                     for (int b=0;b<49;b++) {
                       if (scene[channelCounter].busses[b]) {                                      
                         if (scene[channelCounter].busses[b] == true) {      
                           char bus_buffer[2];  //reserve the string space first
                           sprintf(bus_buffer, "%d", b);              
                           IOShieldOled.putString(bus_buffer);         
                           IOShieldOled.putString(" "); 
                       }
                     }
                   }
                 }  
               
             

        //      //  CHANNEL SETTING - enable this code to debug board select on //OLED boards
        //      String chan_str = "CHAN: ";
        //      chan_str += channelCounter;
        //      char chan_chr[chan_str.length()+1]; //create char buffer
        //      chan_str.toCharArray(chan_chr, chan_str.length()+1); //convert to char
        //
        //      IOShieldOled.setCursor(CHAN_X, CHAN_Y);
        //      IOShieldOled.putString(chan_chr);

        //IOShieldOled.setCursor(BUS_X, BUS_Y);
        //IOShieldOled.putString("BUS");

        //IOShieldOled.updateDisplay();

        //PUT A LINE TO DIVIDE SMALL AND LARGE FADER
        IOShieldOled.moveTo(0, 36);
        IOShieldOled.drawLine(127, 36);
        IOShieldOled.updateDisplay();
       channelCounter++;
       }
    }
    digitalWrite(OLEDstrobePin, HIGH);
  }
}
//--------------------------------------------
// setGarage()
//--------------------------------------------
void setGarage() {
  Serial.println("GARAGE");
  // flip on parking-garage
  //digitalWrite(parkingPin, LOW); // PROBABLY DON'T NEED THIS
  digitalWrite(parkingPin, HIGH);

  digitalWrite(strobePin, HIGH);

  garageBoardAddress = 0;

  // select board
  shiftData(garageBoardAddress, 7, boardAddressPins);

  // shift out driver
  shiftData(driverAddress, 3, driverAddressPins);

  digitalWrite(strobePin, LOW);
  strobeOn = false;

  garageChannel      = 0;
  garageNextChannel  = 0;
  garageBoardAddress = 0;

  while(garageChannel < 47) {

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

            if (scene[garageChannel].large_input == INPUT_INDEX_MIC) {
              relayOn();
            }
            else if (scene[garageChannel].large_input == INPUT_INDEX_LINE) {
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
            if (scene[garageChannel].large_input == INPUT_INDEX_LINE) {
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
            if (scene[garageChannel].large_input == INPUT_INDEX_TAPE) {
              relayOn();
            }
            else {
              relayOff();
            }
          }

          //----------------------------------------------
          // toggle small fader
          //----------------------------------------------
          else if (cycle == TOGGLE_SMALL_FADER_INDEX) {

            if (scene[garageChannel].small_input == INPUT_INDEX_BUS) {
              relayOn();
            }
            else if (scene[garageChannel].small_input == INPUT_INDEX_TAPE) {
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
            if (scene[garageChannel].small_input == INPUT_INDEX_LINE) {                          
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
            if (scene[garageChannel].small_input == INPUT_INDEX_TAPE) {            
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
            if (scene[garageNextChannel].large_input == INPUT_INDEX_MIC) {                                                
              relayOn();
            }
            else if (scene[garageNextChannel].large_input == INPUT_INDEX_LINE) {                                                
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
            if (scene[garageNextChannel].large_input == INPUT_INDEX_LINE) {
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
            if (scene[garageNextChannel].large_input == INPUT_INDEX_TAPE) {
              relayOn();
            }
            else {
              relayOff();
            }
          }

          //----------------------------------------------
          // toggle next small fader
          //----------------------------------------------
          else if (cycle == TOGGLE_NEXT_SMALL_FADER_INDEX) {                      
            if (scene[garageNextChannel].small_input == INPUT_INDEX_BUS) {
              relayOn();
            }
            else if (scene[garageNextChannel].small_input == INPUT_INDEX_TAPE) {
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
            if (scene[garageNextChannel].small_input == INPUT_INDEX_LINE) {
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
            if (scene[garageNextChannel].small_input == INPUT_INDEX_TAPE) {
              relayOn();
            }
            else {
              relayOff();
            }
          }

          //----------------------------------------------
          // reset relay count, increment driver, set strobe
          //----------------------------------------------
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
  Serial.println("COFFIN");
  digitalWrite(parkingPin, LOW); 

  // cycle through channels
  for (int channel = 1; channel < 49; channel++) {
    // shift data out
    int boardChannel = channel - 1;
    shiftData((boardChannel), 7, boardAddressPins);

    // reset bus count for each channel
    coffinBusCount = 1;

    // cycle through driver addresse
    for (int driverAddress=0; driverAddress < 7; driverAddress++) {

      shiftData(driverAddress, 3, driverAddressPins);           

      // cycle through relay addresses
      for (int relayAddress=0; relayAddress<8; relayAddress++) {

        digitalWrite(strobePin, LOW);              
        delayMicroseconds(50);

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
            if (scene[channel].large_outputs[OUTPUT_INDEX_STA]) {
              relayOn();
            }
            else {
              relayOff();
            }
          }

          if (relayAddress == LARGE_B_OUTPUT) {
            if (scene[channel].large_outputs[OUTPUT_INDEX_STB]) {
              relayOn();
            }
            else {
              relayOff();
            }
          }

          if (relayAddress == LARGE_C_OUTPUT) {
            if (scene[channel].large_outputs[OUTPUT_INDEX_STC]) {
              relayOn();
            }
            else {
              relayOff();
            }
          }

          if (relayAddress == SMALL_A_OUTPUT) {
            if (scene[channel].small_outputs[OUTPUT_INDEX_STA]) {
              relayOn();
            }
            else {
              relayOff();
            }
          }

          if (relayAddress == SMALL_B_OUTPUT) {
            if (scene[channel].small_outputs[OUTPUT_INDEX_STB]) {
              relayOn();
            }
            else {
              relayOff();
            }
          }

          if (relayAddress == SMALL_C_OUTPUT) {
            if (scene[channel].small_outputs[OUTPUT_INDEX_STC]) {
              relayOn();
            }
            else {
              relayOff();
            }
          }

          if (relayAddress == PAN_OUTPUT) {
            if (scene[channel].pan == true) {
              relayOff(); // COURTESY OF WOLF ENGINEERING
            }
            else {
              relayOn();
            }
          }

          if (relayAddress == MATRIX_OUTPUT) {
            if (scene[channel].small_outputs[OUTPUT_INDEX_MTX]) {
              relayOn();
            }
            else {
              relayOff();
            }
          }
        }
        coffinBusCount++;
      }
      digitalWrite(strobePin, HIGH);
      delayMicroseconds(50);
    }
  }
}

//--------------------------------------------
// initialize scenes
//--------------------------------------------
void initializeScenes() {

  // setup scene data   
  for (int sceneCount=0; sceneCount < 49; sceneCount++) {
    scene[sceneCount].small_input                     = INPUT_INDEX_MIC;
    scene[sceneCount].large_input                     = INPUT_INDEX_BUS;
    scene[sceneCount].pan                             = false;

    for (int i=0;i<5;i++) {
      scene[sceneCount].small_outputs[i] = false;
    }
    for (int i=0;i<7;i++) {
      scene[sceneCount].large_outputs[i] = false;
    }
    for (int i=0;i<49;i++) {
      scene[sceneCount].busses[i] = false;
    }
  }
}


//--------------------------------------------
// clear scene channel data
//--------------------------------------------
void clearSceneChannel(int channel) {

  // clear channel data
  if (channel < 49) {

    scene[channel].small_input = INPUT_INDEX_MIC;
    scene[channel].large_input = INPUT_INDEX_BUS;
    for (int i=0;i<5;i++) {
      scene[channel].small_outputs[i] = false;
    }
    for (int i=0;i<7;i++) {
      scene[channel].large_outputs[i] = false;
    }
    for (int i=0;i<49;i++) {
      scene[channel].busses[i] = false;
    }

    scene[channel].pan = false;
  }
}


//--------------------------------------------
// setup arduino pins
//--------------------------------------------
void setupPins() {

  // setup pin modes
  pinMode(dataPin       , OUTPUT);
  pinMode(clockPin      , OUTPUT);
  pinMode(strobePin     , OUTPUT);
  pinMode(parkingPin    , OUTPUT);
  pinMode(allSelectPin  , OUTPUT);
  pinMode(OLEDstrobePin , OUTPUT);

  // set initial state
  digitalWrite(clockPin      , LOW);
  digitalWrite(dataPin       , HIGH);
  digitalWrite(strobePin     , HIGH);
  digitalWrite(OLEDstrobePin , HIGH);
  digitalWrite(parkingPin    , HIGH);

  // OLED setup was here


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

  _resetEncoderPins();
}

/*********************************************
 *
 *
 * TODO : optimize timings in relay flipping!
 *
 *
 *********************************************/
//--------------------------------------------
// turn relay on / pulse clock
//--------------------------------------------
void relayOn() {

  if (strobeOn) {
    digitalWrite(strobePin, LOW);
  }

  digitalWrite(clockPin, LOW);
  digitalWrite(dataPin, LOW);
  delayMicroseconds(50);
  digitalWrite(clockPin, HIGH);    
  delayMicroseconds(50);
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
  delayMicroseconds(50);
  digitalWrite(clockPin, HIGH);
  delayMicroseconds(50);
}

//--------------------------------------------
// generic function to shift data out
//--------------------------------------------
void shiftData(int sData, int maxRegister, int pins[] ) {

  for (int i=0; i<=maxRegister;i++)  {

    if ( sData & (1<<i) ) {
      digitalWrite(pins[i], HIGH);
    }
    else {
      digitalWrite(pins[i], LOW);
    }
  }
}

//--------------------------------------------
// draw grid on OLEDs
//--------------------------------------------
void drawGrid(char *fader) {

  // draw vertical line
  //OLED.drawLine(64, 0, 64, 48, WHITE);

  //if (fader == "sf") {
  //    //OLED.drawLine(0, 48, 64, 48, WHITE);
  //}
  //else {
  //    //OLED.drawLine(65, 48, 128, 48, WHITE);
  //}
}

//--------------------------------------------
// set scene data for small input from serial cmd
//--------------------------------------------
void setSceneSmallInput(int sceneSmallInputChannel, char *sceneSmallInputData) {

  //Serial.println(sceneSmallInputData);
  //Serial.print(" : ");
  //Serial.println(sceneSmallInputData);    

  if (sceneSmallInputChannel < 49) {
    scene[sceneSmallInputChannel].small_input =  atoi(sceneSmallInputData);        
  }
}

//--------------------------------------------
// set scene data for small outputs from serial cmd
//--------------------------------------------
void setSceneSmallOutput(int sceneSmallOutputChannel, char *sceneSmallOutputData) {

  if (sceneSmallOutputChannel < 49) {

    char *str;
    char *o = strdup(sceneSmallOutputData);

    while ((str = strtok_r(o, ",", &o)) != NULL) {
      char *temp_output = strdup(str);

      if (strcmp(temp_output, "STA") == 0) {
        scene[sceneSmallOutputChannel].small_outputs[OUTPUT_INDEX_STA] = true;
      }
      if (strcmp(temp_output, "STB") == 0) {
        scene[sceneSmallOutputChannel].small_outputs[OUTPUT_INDEX_STB] = true;
      }
      if (strcmp(temp_output, "STC") == 0) {
        scene[sceneSmallOutputChannel].small_outputs[OUTPUT_INDEX_STC] = true;
      }
      if (strcmp(temp_output, "MTX") == 0) {
        scene[sceneSmallOutputChannel].small_outputs[OUTPUT_INDEX_MTX] = true;
      }

      free(temp_output);
    }
    free(str);
    free(o);
  }
}

//--------------------------------------------
// set scene data for large input from serial cmd
//--------------------------------------------
void setSceneLargeInput(int sceneLargeInputChannel, char *sceneLargeInputData) {

  if (sceneLargeInputChannel < 49) {
    scene[sceneLargeInputChannel].large_input =  atoi(sceneLargeInputData);
  }
}

//--------------------------------------------
// set scene data for large output from serial cmd
//--------------------------------------------
void setSceneLargeOutput(int sceneLargeOutputChannel, char *sceneLargeOutputData ) {

  if (sceneLargeOutputChannel < 49) {

    char *str;
    char *o = strdup(sceneLargeOutputData);

    while ((str = strtok_r(o, ",", &o)) != NULL) {
      char *temp_output = strdup(str);

      if (strcmp(temp_output, "STA") == 0) {
        scene[sceneLargeOutputChannel].large_outputs[OUTPUT_INDEX_STA] = true;
      }
      if (strcmp(temp_output, "STB") == 0) {
        scene[sceneLargeOutputChannel].large_outputs[OUTPUT_INDEX_STB] = true;
      }
      if (strcmp(temp_output, "STC") == 0) {
        scene[sceneLargeOutputChannel].large_outputs[OUTPUT_INDEX_STC] = true;
      }
      if (strcmp(temp_output, "MTX") == 0) {
        scene[sceneLargeOutputChannel].large_outputs[OUTPUT_INDEX_MTX] = true;
      }

      free(temp_output);
    }
    free(str);
    free(o);
  }
}

//--------------------------------------------
// set scene data for busses from serial cmd
//--------------------------------------------
void setSceneBusses(int sceneBusChannel, char *sceneBusData ) {

  if (sceneBusChannel < 49) {

    char *str;
    char *o = strdup(sceneBusData);

    while ((str = strtok_r(o, ",", &o)) != NULL) {
      char *temp_output = strdup(str);
      int temp_bus = atoi(temp_output);

      if (temp_bus < 49) {
        scene[sceneBusChannel].busses[temp_bus] = true;
      }
      free(temp_output);
    }
    free(str);
    free(o);
  }
}

//--------------------------------------------
// set scene data for pan from serial cmd
//--------------------------------------------
void setScenePan(int scenePanChannel, char *scenePanData) {

  //char *str = strdup(scenePanData);

  if (scenePanChannel < 49) {
    if (strcmp(scenePanData, "1") == 0) {       
      scene[scenePanChannel].pan = true;            
    }
    else {
      scene[scenePanChannel].pan = false;            
    }        
  }
  //free(str);
}

//--------------------------------------------
// update console
//--------------------------------------------
void updateConsole() {
  Serial.println("UPDATE");
//
//  digitalWrite(allSelectPin, LOW);     // select all OLEDs
//  IOShieldOled.clearBuffer();
//  IOShieldOled.setCursor(4,3);
//  IOShieldOled.putString("LOADING");
//  IOShieldOled.updateDisplay();
//  digitalWrite(allSelectPin, HIGH);    // deselect all OLEDs

  setGarage();
  setCoffin();
  setOLEDS();

  Serial.println("DONE!");

}

//--------------------------------------------
// reset encoder pins
//--------------------------------------------
static void _resetEncoderPins() {
  pinMode(encoderPinA     , INPUT);
  pinMode(encoderPinB     , INPUT);
  pinMode(encoderpressPin , INPUT);
  pinMode(interruptPin    , INPUT);
  pinMode(acknowledgePin  , INPUT);
  pinMode(enablePin       , OUTPUT);

  digitalWrite(encoderPinA     , HIGH);
  digitalWrite(encoderPinB     , HIGH);
  digitalWrite(encoderpressPin , HIGH);
  digitalWrite(interruptPin    , HIGH);
  digitalWrite(acknowledgePin  , HIGH);
  digitalWrite(enablePin       , HIGH);
}

//--------------------------------------------
// read rotation/press from encoder
//--------------------------------------------
void _lowlevel_readEncoder(int &rotate, int& press) {

  rotate = (digitalRead(encoderPinB) * 2) + digitalRead(encoderPinA);
  press  = digitalRead(encoderpressPin);
}

//--------------------------------------------
// read encoder
//--------------------------------------------
void readEncoder() {

  int position, press;
  int isForward = 0;

  // hard-coded channel for now
  int encoderChannel = 17;

  _resetEncoderPins();

  digitalWrite(enablePin, LOW);

  // setup corresponding OLED for this channel
  selectOLED(encoderChannel);

  _lowlevel_readEncoder(position, press);

  while (!Serial.available()) {

    int position2, press2;

    do {

      _lowlevel_readEncoder(position2, press2);

      // finished w/ encoder, clean things
      if (encoderToggle == LOW) {

        digitalWrite(enablePin, HIGH);
        delay(500);
        updateConsole();
        encoderToggle = LOW;
        return;
      }
    } 
    while ((position2 == position) && (press2 == press));

    if (position2 != position) {

      int isFwd = ((position == 0) && (position2 == 1)) ||
        ((position == 1) && (position2 == 3)) ||
        ((position == 3) && (position2 == 2)) ||
        ((position == 2) && (position2 == 0));

      isFwd ? routings++ : routings--;

      // reset incrementer if we've gone below the min
      if (routings == 0) {
        routings = 64;
      }

      // reset incrementer if we've gone over the max
      if (routings == 65) {
        routings = 1;
      }

      IOShieldOled.setCursor(ROTARY_X, ROTARY_Y);

      if (routings < 48) {

        IOShieldOled.clearBuffer();
        IOShieldOled.putString("Bus : ");
        IOShieldOled.putChar(routings);
        IOShieldOled.updateDisplay();
      }
      if (routings > 48) {

        int i = routings - 48;

        IOShieldOled.clearBuffer();
        IOShieldOled.putString(extra_routings[i]);
        IOShieldOled.updateDisplay();
      }
    }

    // capture encoder presses
    if (press2 != press) {

      if (press) {

        // update scene for busses
        if (routings < 49) {
          scene[encoderChannel].busses[routings] = true;
        }

        // update scene for non-bus routings
        if (routings > 48) {

          int i = routings - 48;

          switch (i) {

          case ROTARY_LG_MIC:
            scene[encoderChannel].large_input = INPUT_INDEX_MIC;
            break;
          case ROTARY_LG_LINE:
            scene[encoderChannel].large_input = INPUT_INDEX_LINE;
            break;
          case ROTARY_LG_BUS:
            scene[encoderChannel].large_input = INPUT_INDEX_BUS;
            break;
          case ROTARY_LG_TAPE:
            scene[encoderChannel].large_input = INPUT_INDEX_TAPE;
            break;
          case ROTARY_LG_STA:
            scene[encoderChannel].large_outputs[OUTPUT_INDEX_STA] = true;
            break;
          case ROTARY_LG_STB:
            scene[encoderChannel].large_outputs[OUTPUT_INDEX_STB] = true;
            break;
          case ROTARY_LG_STC:
            scene[encoderChannel].large_outputs[OUTPUT_INDEX_STC] = true;
            break;
          case ROTARY_LG_PAN:
            scene[encoderChannel].large_outputs[OUTPUT_INDEX_PAN] = true;
            break;
          case ROTARY_LG_MTX:
            scene[encoderChannel].large_outputs[OUTPUT_INDEX_MTX] = true;
            break;
          case ROTARY_SM_MIC:
            scene[encoderChannel].small_input = INPUT_INDEX_MIC;
            break;
          case ROTARY_SM_LINE:
            scene[encoderChannel].small_input = INPUT_INDEX_LINE;
            break;
          case ROTARY_SM_BUS:
            scene[encoderChannel].small_input = INPUT_INDEX_BUS;
            break;
          case ROTARY_SM_TAPE:
            scene[encoderChannel].small_input = INPUT_INDEX_TAPE;
            break;
          case ROTARY_SM_STA:
            scene[encoderChannel].small_outputs[OUTPUT_INDEX_STA] = true;
            break;
          case ROTARY_SM_STB:
            scene[encoderChannel].small_outputs[OUTPUT_INDEX_STB] = true;
            break;
          case ROTARY_SM_STC:
            scene[encoderChannel].small_outputs[OUTPUT_INDEX_STC] = true;
            break;
          case ROTARY_SM_PAN:
            scene[encoderChannel].small_outputs[OUTPUT_INDEX_PAN] = true;
            break;
          case ROTARY_SM_MTX:
            scene[encoderChannel].small_outputs[OUTPUT_INDEX_MTX] = true;
            break;
          }
        }
      }
    }
    position = position2;
    press    = press2;
  }
}

//--------------------------------------------
// select OLED board
//--------------------------------------------
void selectOLED(int channel) {

  // TODO : figure out why we need to do this.
  channel--;

  int CHANNELS_PER_OLED_BOARD = 8;

  // set strobe low to allow data to flow
  digitalWrite(OLEDstrobePin, LOW);

  int board = 0;
  int chip  = 0;

  // find which board this channel is on
  board = channel / CHANNELS_PER_OLED_BOARD;

  // select OLED board
  shiftData(board, 3, boardAddressPins);

  // find which board this channel is on
  chip = channel % CHANNELS_PER_OLED_BOARD;

  // select OLED chip
  shiftData(chip, 2, driverAddressPins);
}


//--------------------------------------------
// toggle OLEDS on/off
//--------------------------------------------
void toggleOLED(int toggle) {
  
   Serial.println("in toggleOLED");
 
   Serial.println(toggle); 
 
   if (toggle == 1) {
       Serial.println("sleep");
       OLEDSleep();
   }
   else {
       Serial.println("wake");     
       OLEDWake();     
   }
}

//--------------------------------------------
// put OLEDs to sleep
//--------------------------------------------
void OLEDSleep() {
  digitalWrite(allSelectPin, LOW);     // select all OLEDs
  IOShieldOled.displayOff();
  digitalWrite(allSelectPin, HIGH);    // disable all OLED selection
}

//--------------------------------------------
// wake up OLEDs
//--------------------------------------------
void OLEDWake() {
  digitalWrite(allSelectPin, LOW);     // select all OLEDs
  IOShieldOled.displayOn();
  digitalWrite(allSelectPin, HIGH);     // disable all OLED selection
}

//--------------------------------------------
// parse command string coming in over serial
//--------------------------------------------
void parseSerialCommand(char* inputDataString) {

  //Serial.println("parse");

  char *routing;
  int channel;
  char *scData;
  char *token;
  char *rest;

  int i = 0;

  // grab comma delimited bus string from serial cmd message
  for (token = strtok_r(inputDataString, "/", &rest); token; token = strtok_r(NULL, "/", &rest)) {
    if (i < 3) {
      if (i == 0) {
        routing = strdup(token);
        //memcpy(routing, token, 1);
      }
      if (i == 1) {
        channel = atoi(strdup(token));
      }
      if (i > 1) {
        scData = strdup(token);      
        //Serial.print("|");
        //Serial.print(token);          
        //Serial.println("|");
        //memcpy(scData, token, 10);
      }
    }
    i++;
  }

  //Serial.println(routing);   
  //Serial.println(channel);

  if (routing && channel) {
    //Serial.println("route");
    routeSerialCommand(routing, channel, scData);
  }

  free(routing);
  free(scData);
  free(token);
  free(rest);
  //free(inputDataString);       
}

//--------------------------------------------
// route serial command ala OSC
//--------------------------------------------
void routeSerialCommand(char *routing, int channel, char *rData) {

  if (strcmp(routing, "0") == 0) {
    clearSceneChannel(channel);
  }

  if (strcmp(routing, "1") == 0) {        
    setSceneSmallInput(channel, rData);
  }

  if (strcmp(routing, "2") == 0) {
    setSceneLargeInput(channel, rData);
  }

  if (strcmp(routing, "3") == 0) {
    setScenePan(channel, rData);
  }

  if (strcmp (routing, "4" ) == 0) {
    setSceneSmallOutput(channel, rData);
  }

  if (strcmp(routing, "5") == 0) {
    setSceneLargeOutput(channel, rData);
  }

  if (strcmp(routing, "6") == 0) {
    setSceneBusses(channel, rData);
  }

  if (strcmp(routing, "7") == 0) {
    updateConsole();
    //Serial.println("update");
  }
  
  if (strcmp(routing, "8") == 0) {
    Serial.println("routing is 8");
    toggleOLED(channel);
  }  

  //free(routing);
  //free(rData);
}




