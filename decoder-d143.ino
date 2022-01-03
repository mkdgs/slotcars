/* Carrera DIGITAL 124/132/143 arduino protocol analyser: "The inspector"
 *
 * Dipl.-Ing. Peter Niehues CC BY-NC-SA 3.0
 *
 * This is an example software to decode carrera data protocol. For further information (german only) 
 * visit http://www.wasserstoffe.de/carrera/protocoll-analyser/ 
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/.
 *
 * This license allows you to remix, tweak, and build upon my work non-commercially, as long as 
 * you credit Peter Niehues and license the new creations under the identical terms. 
 *
 */
const int dataPin = 2;                                // the number of the pin to read carrera racetrack data
int wordCount = 0;                                    // counter 

boolean wordChange = false;                           // indicates a new word is ready for further analysing

long currentWord = 0;                                 // assemble Bits to complete data words, used in interrupt routine 
long Word = 0;                                        // save data word for further computing, used in main loop and interrupt
long Words[11];                                       // store 10 datawords in array with counts 1..10

unsigned long intervalMicros = 0;                     // will store the time between two bit changes (microseconds)
unsigned long previousMicros = 0;                     // will store last time state when data signal has changed (microseconds)
unsigned long currentMicros  = 0;                     // will store the current runtime (microseconds)

void setup() {                                        //////
  Serial.begin(115200);                               // initialize serial bus 
  pinMode(dataPin, INPUT);                            // initialize the dataPin as an input
  attachInterrupt(0, manchesterDecode, CHANGE);       // whenever levels on dataPin change, start interrupt routine
}                                                     //////

void loop() {                                         //////
  if( wordChange == true ){                           // only start when interrupt routine has assemled a complete word
    wordChange = false;                               // reset word change indicator
    if( Word > 4000 )                                 // synchronize to longest dataword with 13 bits
      wordCount = 1;                                  // longest word is saved on first position in array
    Words[ wordCount ] = Word;                        // save the actual word in array
    wordCount++;}                                     // increase counter
  if ( wordCount == 11 ){                             // if all 10 words are stored, write them to serial
    for (wordCount=1; wordCount < 11; wordCount++){   //
      Serial.print( wordCount, DEC );                 // print number of dataword
      Serial.print( "\t" );                           // print tab
      Serial.println( Words[ wordCount ], BIN );}     // print dataword in binary format
    Serial.println("---------------------------");    //
    wordCount = 1;                                    // reset counter
    delay( 2000 );}                                   // wait two seconds
}                                                     //////

void manchesterDecode(){                              //////
  currentMicros = micros();                           // get current runtime
  intervalMicros = currentMicros - previousMicros;    // calculate interval
  if (intervalMicros > 75 && intervalMicros < 125){   // is full clock pulse?
    previousMicros = currentMicros;                   // synchronise
    currentWord = currentWord << 1;                   // shift bits left
    if ( digitalRead( dataPin ) == LOW )              // is pin level LOW?
      bitSet( currentWord,0 );                        // received digital 1
    return; }                                         // leave interrupt
  if ( intervalMicros > 6000 ) {                      // is word rate?
    Word = currentWord;                               // save bits for main loop
    currentWord = 0;                                  // reset bits 
    bitSet( currentWord,0 );                          // first bit is always 1
    wordChange = true;                                // indicate a new word 
    previousMicros = currentMicros;                   // synchronise
    return; }                                         // leave interrupt                 
}                                                     //////

/*
 * End of "The Inspector" Carrera Decoding Sketch
 */



 
