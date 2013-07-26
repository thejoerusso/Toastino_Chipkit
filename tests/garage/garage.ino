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

int boardAddressPins[] = { 40, 42, 44, 46, 48, 50, 52 };
int driverAddressPins[] = { 32, 34, 36, 38 };

// currently not wired
// int boardAddressPinSeven = 40;

int totalChannels     = 48;
int garageChannel     = 0;
int garageNextChannel = 0;
int garageChannelCount[48];

boolean strobeOn = false;

int garageBoardAddress = 0;
int empty = 0;

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

// setup "window" and driver vars
int relay = 0;
int driverAddress = 0;

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

void setup() {

    Serial.begin(9600);

    // setup scene data
    for (int sceneCount=0; sceneCount < 48; sceneCount++) {

        scene[sceneCount].channel                 = sceneCount;
        scene[sceneCount].name                    = "test channel";
        scene[sceneCount].busses[sceneCount]      = 1;
        scene[sceneCount].small_input             = "MIC";
        scene[sceneCount].small_outputs           = "STA";
        scene[sceneCount].large_input             = "MIC";
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

    digitalWrite(strobePin, HIGH);

    strobeOn = true;

}

//--------------------------------------------
// main()
//--------------------------------------------
void loop() {

    digitalWrite(strobePin, HIGH);

    // select board
    shiftOut(garageBoardAddress, 7, boardAddressPins);

    // shift out driver
    shiftOut(driverAddress, 3, driverAddressPins);
    digitalWrite(strobePin, LOW);
    strobeOn = false;

    while(garageChannel < 48) {

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
                            shiftOut(garageBoardAddress, 7, boardAddressPins);
                        }
                        else {
                            driverAddress = driverAddress + 1;
                        }

                        shiftOut(driverAddress, 3, driverAddressPins);

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
//
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
//
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

void shiftOut(int data, int maxRegister, int pins[] ) {

    for (int i=0; i<=maxRegister;i++)  {

        if ( data & (1<<i) ) {
            digitalWrite(pins[i], HIGH);
        }
        else {
            digitalWrite(pins[i], LOW);
        }
    }
}
