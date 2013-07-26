
enum enDigitalPins {
    encoderPinA     = 2,
    encoderPinB     = 3,
    encoderpressPin = 12,
    interruptPin    = 18,
    acknowledgePin  = 10,
    enablePin       = 8
};

char* extra_routings[] = { "LG MIC", "LG LINE", "LG BUS", "LG TAPE", "LG STA", "LG STB", "LG STC", "LG PAN", "LG MTX", "SM MIC", "SM LINE", "SM BUS", "SM TAPE", "SM STA", "SM STB", "SM STC", "SM PAN", "SM MTX" }; 

int routings    = 0;
int maxroutings = 48;

static void _resetPins() {
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

void _lowlevel_ReadEncoder(int &rotate, int& press) {
  
    rotate = (digitalRead(encoderPinB) * 2) + digitalRead(encoderPinA);
    press  = digitalRead(encoderpressPin);
}


void ReadEncoder() {
  
    int position, press;
    int isForward = 0;
    
    _resetPins();
    
    Serial.println("Reading Encoder"); 
    digitalWrite(enablePin, LOW);  
    
    _lowlevel_ReadEncoder(position, press);
    
    while (!Serial.available()) {
      
        int position2, press2;
        
        do {
          
            _lowlevel_ReadEncoder(position2, press2);
          
            if (digitalRead(interruptPin) == LOW  && digitalRead(enablePin) == LOW)  {  
              
                digitalWrite(enablePin, HIGH);
                Serial.println("encoder off");
                delay(100);
                return;
            }                            
        } while ((position2 == position) && (press2 == press));
        
        
        if (position2 != position) {
          
            int isFwd = ((position == 0) && (position2 == 1)) ||
                        ((position == 1) && (position2 == 3)) ||
                        ((position == 3) && (position2 == 2)) ||
                        ((position == 2) && (position2 == 0));
                        
            isFwd ? routings++ : routings--;
            
            if (routings == 0) {
                routings = 64;
            }
            
            if (routings == 65) {
                routings = 1;  
            }
            
            if (routings < 49) {
                Serial.print("BUS: ");
                Serial.println(routings);    
            }
            if (routings > 48) {
                int i = routings - 48;
                Serial.println(extra_routings[i]);
            }            
        }

        // capture encoder presses          
        if (press2 != press) {
            if (press) {
                Serial.println("Setting routing! ");
            }
        }
        
        position = position2;
        press    = press2;   
    }
}


void setup() {
  
    // configure the pins
    _resetPins();
  
    // init serial communication
    Serial.begin(9600); 
}


void loop() {
  
    delay(200);
  
    if (digitalRead(interruptPin) == LOW && digitalRead(acknowledgePin) == LOW)  { 
        delay(300);
        ReadEncoder();                
    }       
}
