/*
20:58:48.605 -> 1100000100010
20:58:48.638 -> 1111011000
20:58:48.638 -> 10000000
20:58:48.638 -> 1000100000
20:58:48.638 -> 1100000000
20:58:48.671 -> 1001100000
20:58:48.671 -> 1101000000
20:58:48.671 -> 1010100000
20:58:48.671 -> 10000000
20:58:48.671 -> 1011000000
 */

const int dataPin = 2; 

boolean wordChange = false;
long Word = 0;
long currentWord = 0;                                                                        
unsigned long intervalMicros = 0;                     
unsigned long previousMicros = 0;                     
unsigned long currentMicros  = 0; 


// bit utility
#define GETMASK(index, size) ((((size_t)1 << (size)) - 1) << (index))
#define READFROM(data, index, size) (((data) & GETMASK((index), (size))) >> (index))
#define WRITETO(data, index, size, value) ((data) = (((data) & (~GETMASK((index), (size)))) | (((value) << (index)) & (GETMASK((index), (size))))))
#define FIELD(data, name, index, size) \
  inline decltype(data) name() const { return READFROM(data, index, size); } \
  inline void set_##name(decltype(data) value) { WRITETO(data, index, size, value); }

struct CarWord {
  long bitData;
  long one = READFROM(bitData, 10-4, 4); // from the end
  FIELD(bitData, one, 0, 1)
  FIELD(bitData, aspeed, 2, 3)
};



void setup() {                                        
  Serial.begin(115200);                                
  pinMode(dataPin, INPUT);                            
  attachInterrupt(0, manchesterDecode, CHANGE);
  Serial.println("test 1");               
}                                       
                                           
void loop() {                                         
  if( wordChange == true ){                           
    wordChange = false;                               

    if (0b10000000000 <= Word && 0b1111111111111 >= Word ) {
      //Serial.print( Word, BIN );
      //Serial.println(" Mot de données programation ");
    }
    else if (0b1000000000 <= Word && 0b1111111111 >= Word ) {
      Serial.println( Word, BIN );
      
      

      

      //CarWord car = { Word};
      //Serial.println(car.aspeed);
      // bit depart 
      Serial.println(READFROM(Word, 1, 3),BIN); // 3 Adresse du contrôleur (0-5). MSB
      Serial.println(READFROM(Word, 3, 1),BIN); // 1 Bouton de changement de voie. Devient 0 lorsque la touche est enfoncée.
      Serial.println(READFROM(Word, 4, 4),BIN); // 4 Vitesse (0-15) MSB

      
      //Serial.println(READFROM(Word, 1, 3));
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
  if (intervalMicros > 75 && intervalMicros < 125){   
    previousMicros = currentMicros;                   
    currentWord = currentWord << 1;                   
    if ( digitalRead( dataPin ) == LOW )              
      bitSet( currentWord,0 );                        
    return; }                                         
  if ( intervalMicros > 6000 ) {                      
    Word = currentWord;                               
    currentWord = 0;                                  
    bitSet( currentWord,0 );                          
    wordChange = true;                                 
    previousMicros = currentMicros;                   
    return; }                                                          
}              
