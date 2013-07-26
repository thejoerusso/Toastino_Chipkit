/*********************************
*
* test to cycle through channels
*
*********************************/

//----------------
// configure pins
//----------------
int dataPin      = 12;
int clockPin     = 13;
int strobePin    = 11;

int maxAddressPin = 53;

int channelAddressPins[] = { 40, 42, 44, 46, 48, 50, 52 };
int driverAddressPins[]  = { 32, 34, 36 }; // 38 should be here whenever parking garage needs to work

// currently not wired
// int boardAddressPinSeven = 40;

int totalChannels = 48;
int delaytime = 10;

//----------------
// setup arduino
//----------------
void setup() {

    pinMode(dataPin  , OUTPUT);
    pinMode(clockPin , OUTPUT);
    pinMode(strobePin, OUTPUT);

    // set initial state
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin , HIGH);
    digitalWrite(strobePin , LOW);

    // initialize pins 32-52 and set to low
    for (int addressPin = 32;addressPin<maxAddressPin;addressPin++) {
             pinMode(addressPin, OUTPUT);
        digitalWrite(addressPin, LOW);
    }

    // set channel address pins
    for (int channelAddressPin =
0;channelAddressPin<(sizeof(channelAddressPins)/sizeof(int));channelAddressPin++)
{
             pinMode(channelAddressPin, OUTPUT);
        digitalWrite(channelAddressPin, LOW);
    }

    // set driver address pins
    for (int driverAddressPin =
0;driverAddressPin<(sizeof(driverAddressPins)/sizeof(int));driverAddressPin++)
{
             pinMode(driverAddressPin, OUTPUT);
        digitalWrite(driverAddressPin, LOW);
    }

    digitalWrite(strobePin, LOW);

    delay(10);

}

//----------------------------
// main()
//----------------------------
void loop() {

    // cycle through channels
    for (int channel = 0; channel < 48; channel++) {

        // set strobePin low to allow data flow
        digitalWrite(strobePin, HIGH);

        // shift data out
        shiftOut(channel);

        delay(delaytime);

        // cycle through driver addresse
        for (int driverAddress=0; driverAddress < 7; driverAddress++) {


           shiftOutDriver(driverAddress);

           //delayMicroseconds(500);

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

void shiftOut(byte dataOut ) {

    //
    for (int i=0; i<=7;i++)  {

        if ( dataOut & (1<<i) ) {
            digitalWrite(channelAddressPins[i], HIGH);
        }
        else {
            digitalWrite(channelAddressPins[i], LOW);
        }
    }
}

void shiftOutDriver(byte dataOut ) {

    //
    for (int i=0; i<=3; i++)  {

        if ( dataOut & (1<<i) ) {
            digitalWrite(driverAddressPins[i], HIGH);
        }
        else {
            digitalWrite(driverAddressPins[i], LOW);
        }
    }
}

