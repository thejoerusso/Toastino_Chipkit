 /*********************************
*
* test to cycle through garage
*
*********************************/

//--------------------------------------------
// configure pins
// TODO : NEED TO REALLY SET THESE PINS UP
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

int totalChannels     = 48;
int garageChannel     = 0;
int garageNextChannel = 0;
int garageChannelCount[48];

/////////////////////////////////////////////////////
// setup convenience constants for "window" positions
/////////////////////////////////////////////////////
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

    // Initialize arrays to zeros
    memset(garageChannelCount,0,sizeof(garageChannelCount));

    // setup scene data
    for (int sceneCount=0; sceneCount<48; sceneCount++) {

        scene[sceneCount].channel                 = sceneCount;
        scene[sceneCount].name                    = "test channel";
        scene[sceneCount].busses[sceneCount]        = 1;
        scene[sceneCount].small_input             = "TAPE";
        scene[sceneCount].small_outputs           = "STA";
        scene[sceneCount].large_input             = "BUS";
        scene[sceneCount].large_outputs           = "STB";
        scene[sceneCount].pan                     = false;
    }

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



    digitalWrite(strobePin, HIGH);

    /////////////////////////////////////////////////////
    // set strobePin low to allow data flow
    /////////////////////////////////////////////////////
    digitalWrite(strobePin, LOW);

    // board
    shiftOut(3);

    // driver
    shiftOutDriver(0);

    // relay
    relayOff();
    relayOn();
    relayOff();
    relayOff();
    relayOff();
    relayOff();
    relayOn();
    relayOff();

//    digitalWrite(dataPin, LOW); //TOGGLE_SMALL_FADER_INDEX        = 8;
//    digitalWrite(dataPin, LOW); //TOGGLE_NEXT_SMALL_FADER_INDEX   = 9;
//    digitalWrite(dataPin, LOW); //BUS_TAPE_NEXT_SMALL_FADER_INDEX = 10;
//    digitalWrite(dataPin, LOW); //MIC_LINE_NEXT_SMALL_FADER_INDEX = 11;

    //////////////////////////////////////////////////////
    // pulse clock
    //////////////////////////////////////////////////////
    // delayMicroseconds(10);


}

//--------------------------------------------
// main()
//--------------------------------------------
void loop() {

}

//--------------------------------------------
// TODO : refactor shiftOut* functions to be more modular
//--------------------------------------------
void shiftOut(byte dataOut ) {

    // Serial.println("IN SHIFT OUT");
    for (int i=0; i<=7;i++)  {

        // Serial.println(dataOut);
        if ( dataOut & (1<<i) ) {
            digitalWrite(boardAddressPins[i], HIGH);
        }
        else {
            digitalWrite(boardAddressPins[i], LOW);
        }
    }
}

//--------------------------------------------
// TODO : refactor shiftOut* functions to be more modular
//--------------------------------------------
void shiftOutDriver(byte dataOut ) {

    for (int i=0; i<=3; i++)  {

        if ( dataOut & (1<<i) ) {
            digitalWrite(driverAddressPins[i], HIGH);
        }
        else {
            digitalWrite(driverAddressPins[i], LOW);

        }
    }
}

void relayOn() {
  digitalWrite(clockPin, LOW); 
  digitalWrite(dataPin, LOW);
  delay(1);
  digitalWrite(clockPin, HIGH);
  delay(1); 
  
}
void relayOff() {
  digitalWrite(clockPin, LOW); 
  digitalWrite(dataPin, HIGH);
  delay(1);
  digitalWrite(clockPin, HIGH);
  delay(1); 
  
}
