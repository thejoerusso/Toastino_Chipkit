/**************************************
*
* test to cycle through coffin/garage
*
**************************************/

//--------------------------------------------
// configure pins
// TODO : NEED TO REALLY SET THESE PINS UP
//--------------------------------------------
int dataPin       = 12;
int clockPin      = 13;
int strobePin     = 11;
int parkingPin    = 23;

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

boolean strobeOn       = false;

int coffinBusCount     = 0;

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

    // setup scene data
    for (int sceneCount=0; sceneCount < 48; sceneCount++) {

        scene[sceneCount].channel                 = sceneCount;
        scene[sceneCount].name                    = "test channel";

        // set busses 1 & 2 on channels 1-4

        if (sceneCount < 4) {
            scene[sceneCount].busses[28]      = true;
            scene[sceneCount].busses[40]      = true;
        }
        else {
            for (int s=0;s<48;s++) {
                if (s != 28 || s != 40) {
                    scene[sceneCount].busses[s] = false;
                }
            }
        }

        scene[sceneCount].small_input             = "MIC";
        scene[sceneCount].small_output            = "STB";
        scene[sceneCount].large_input             = "MIC";
        scene[sceneCount].large_output            = "STA";
        scene[sceneCount].pan                     = false;
    }

    // setup pin modes
    pinMode(dataPin    , OUTPUT);
    pinMode(clockPin   , OUTPUT);
    pinMode(strobePin  , OUTPUT);
    pinMode(parkingPin , OUTPUT);

    pinMode(52 , OUTPUT);
    digitalWrite(52, LOW);

    // set initial state
    digitalWrite(clockPin   , LOW);
    digitalWrite(dataPin    , HIGH);
    digitalWrite(strobePin  , LOW);
    digitalWrite(parkingPin , HIGH);

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

    //setGarage();

    //delay(1000);

    setCoffin();

    delay(30000);
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

                        //Serial.println(scene[channel].busses[coffinBusCount]);
                        // Serial.print("Turning on bus ");
                        // Serial.println(coffinBusCount);
                        // Serial.print(" for channel ");
                        // Serial.println(channel);
                        relayOn();
                    }
                    else {
                        relayOff();
                    }
                }
                else {

                    if (relayAddress == LARGE_A_OUTPUT) {
                        if (scene[channel].large_output == "STA") {
                            Serial.println("large_output STA");
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    if (relayAddress == LARGE_B_OUTPUT) {
                        if (scene[channel].large_output == "STB") {
                            Serial.println("large_output STB");
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    if (relayAddress == LARGE_C_OUTPUT) {
                        if (scene[channel].large_output == "STC") {
                            Serial.println("large_output STC");
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    if (relayAddress == SMALL_A_OUTPUT) {
                        if (scene[channel].small_output == "STA") {
                            Serial.println("small_output STA");
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    if (relayAddress == SMALL_B_OUTPUT) {
                        if (scene[channel].small_output == "STB") {
                            Serial.println("small_output STB");
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    if (relayAddress == SMALL_C_OUTPUT) {
                        if (scene[channel].small_output == "STC") {
                            Serial.println("small_output STC");
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    if (relayAddress == PAN_OUTPUT) {
                        if (scene[channel].small_output == "PAN") {
                            Serial.println("small_output PAN");
                            relayOn();
                        }
                        else {
                            relayOff();
                        }
                    }

                    if (relayAddress == MATRIX_OUTPUT) {
                        if (scene[channel].small_output == "MTX") {
                            Serial.println("small_output MTX");
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
