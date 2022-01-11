/*
 * sample of byte word 
 * 20:58:48.605 -> 1100000100010
 * 20:58:48.638 -> 1111011000
 * 20:58:48.638 -> 10000000
 * 20:58:48.638 -> 1000100000
 * 20:58:48.638 -> 1100000000
 * 20:58:48.671 -> 1001100000
 * 20:58:48.671 -> 1101000000
 * 20:58:48.671 -> 1010100000
 * 20:58:48.671 -> 10000000
 * 20:58:48.671 -> 1011000000
 */
#include <math.h>

// bit utility
#define getBitMask(index, size) ((((size_t)1 << (size)) - 1) << (index))
#define getBit(data, index, size) (((data) & getBitMask((index), (size))) >> (index))

struct Hardw { 
  // card wavgate UNO
  unsigned short int freq = 32; // MHZ
  unsigned short int timerTick = 2; // MHZ
  
  unsigned short int pinTrack = 2; // MHZ  
  unsigned short int pinIrLed = 3; // Car id ir pulse  TIMER OC0A // doc p97
  unsigned short int pinLight = 4; // 
  unsigned short int pinRearLight = 5; //
  unsigned short int pinNeon = 6; // why not ? 
   
  // car
  unsigned short int carID = 0; // current car ID
  /*
     IR LED
     Timertick 4µs, 2 µs @ 32 MHz
     PWM = Period / Timertick - 1       
     Auto   Frequenz kHz      Period/µs   Value PWM 16 MHz     Value PWM 32 MHz  
     1      15,625           64             15                  31
     2      7,8125          128             31                  63
     3      5,208333333     192             47                  95
     4      3,90625         256             63                  127  
     5      3,125           320             79                  159
     6      2,604166667     384             95                  191 
     7      2,232142857     448            111                  223
     8      1,953125        512            127                  255
     */
  const float carIrPeriode[8] {
    64, // car id 0
    128, // car id 1 
    192, // car id 2 
    256, // car id 3 
    320, // car id 4 
    384, // car id 5 
    448, // car id Ghostcar 
    512, // car id Pacecare 
  };
  const unsigned int carIrFreq[8] {
    (round(15.625 * 1000)), // car id 0 in kHz
    (round(7.8125 * 1000)), // car id 1 
    (round(5.208333333 * 1000)), // car id 2 
    (round(3.90625 * 1000)), // car id 3 
    (round(3.125 * 1000)), // car id 4 
    (round(2.604166667 * 1000)), // car id 5 
    (round(2.232142857 * 1000)), // car id Ghostcar 
    (round(1.953125 * 1000)), // car id Pacecare 
  };

};

const Hardw hardware;
struct CarWord {
  long bitData;
  long id; // car track ID
  long changeLane; // button request change Lane
  long speed; // speed 1 - 7
  long fuel;  // not used on digital 143
     
  void parseCarWord()  {
     id = getBit(bitData, 10-4, 3); 
     changeLane = getBit(bitData, 5, 1);
     speed = getBit(bitData, 1, 4);
     fuel = getBit(bitData, 0, 1);
  }   
} parseCarWord;

// not used on digital 143
struct ghostCarWord {
  long bitData;  
  long id = getBit(bitData, 10-4, 3); // 111 
  long kfr = getBit(bitData, 10-4, 3); // kfr ?  
  long run = getBit(bitData, 10-4, 3); // ? 
  long fr = getBit(bitData, 10-4, 3); // kfr inverted
  long bt = getBit(bitData, 10-4, 3); // pace car button
  long fuel = getBit(bitData, 0, 1); 
};

// command actionned
// 10000000
struct playerWord {
  long bitData;
  long p0 = getBit(bitData, 6, 1); 
  long p1 = getBit(bitData, 5, 1); 
  long p2 = getBit(bitData, 4, 1); 
  long p3 = getBit(bitData, 3, 1); 
  long p4 = getBit(bitData, 2, 1); 
  long p5 = getBit(bitData, 1, 1); 
  long act = getBit(bitData, 0, 1);  
};

void setup() {                                        
  Serial.begin(115200);                                
  pinMode(hardware.pinTrack, INPUT);                            
  attachInterrupt(0, manchesterDecode, CHANGE);
  
  pinMode(hardware.pinIrLed, OUTPUT);
  noInterrupts();
  //const fpwm = "COM04=2";
  TCCR1A = 0;
  TCCR1B = 0b00001100;
  TIMSK1 = 0b00000010;
  TCNT1 = 0;  
  OCR1A = 31250; 
  interrupts();
  
  Serial.println("SETUP END ----");               
}                                       

boolean wordChange = false;
long Word = 0;
long currentWord = 0;                                                                        
unsigned long intervalMicros = 0;                     
unsigned long previousMicros = 0;                     
unsigned long currentMicros  = 0;
                                           
void loop() {                                         
  if( wordChange == true ){                           
    wordChange = false;                               

    if (0b10000000000 <= Word && 0b1111111111111 >= Word ) {
      //Serial.print( Word, BIN );
      //Serial.println(" Mot de données programation ");
    }
    else if (0b1000000000 <= Word && 0b1111111111 >= Word ) {
      Serial.println( Word, BIN );     
      CarWord car = {Word};           
      //Serial.println(car.aspeed);
      // Serial.println(getBit(Word, 1, 3),BIN); // 3 Adresse du contrôleur (0-5). MSB
      driveCar(car);
      Serial.println(" CAR WORD");
    }    
    else if (0b10000000 <= Word && 0b11111111 >= Word ) {
      //Serial.print( Word, BIN );
      //Serial.println(" Mot de données actif ");
    }
    else {
     // Serial.print( Word, BIN );
     // Serial.println(" UNKNOW WORD");
    }    
  }                          
}                                        

void manchesterDecode(){                              
  currentMicros = micros();                           
  intervalMicros = currentMicros - previousMicros;   
  //Serial.println("test");
  if (intervalMicros > 75 && intervalMicros < 125) {   
    previousMicros = currentMicros;                   
    currentWord = currentWord << 1;                   
    if ( digitalRead( hardware.pinTrack ) == LOW ) {          
      bitSet( currentWord,0 );                        
      return;
    }
  }                                         
  if ( intervalMicros > 6000 ) {                      
    Word = currentWord;                               
    currentWord = 0;                                  
    bitSet( currentWord,0 );                          
    wordChange = true;                                 
    previousMicros = currentMicros;                   
    return;
  }                                                          
}            

void driveCar(CarWord car) {
  if ( car.id == hardware.carID ) {
    timerIrWavgat(car.id);
 }
}

void timerIrWavgat(unsigned long car_ID) {
  // hardware.carIrPeriode
  // hardware.carIrFreq
  // hardware.pinIrLed
 tone(hardware.pinIrLed, hardware.carIrFreq[car_ID]);
}
