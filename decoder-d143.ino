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

/*****************************************************************************/

/// \file       defines.h
/// \author     Frank Redlich
/// \version    1.05
/// \date       26.12.2020
/// \brief      allgemeine defines
/// \details    Projekt:    Carrera_Decoder<br>
///             Prozessor:  Microchip PIC16F1824 <br>
///             Hardware:   Rev2a

/******************************************************************************/
/*
 neu an 1.01 
 * getrennte Pulslängen für High und Low-Pulse. Durch das Schaltverhalten des
   Transistors ist die Highphase immer kürzer als die Low Phase 

 neu an 1.02 
 * verschiedene Defines für 16 MHz Takt und 32 MHz Takt  
  
 neu an 1.03
 * compilerswitch für Prozessortakt entfernt, jetzt immer 32MHz  
  
 neu an 1.04
 * defines für nicht negierte (standard Schaltung) und negierten Input 
 
 neu an 1.05
 * Formel 1 Bremslicht hat andere Einschaltdauer 
   
 */

#ifndef DEFINES_H
#define	DEFINES_H


#include "version.h"

#ifdef STANDARD_DECODER
    #define  INPUT_INVERTED             // normale Schaltung mit Eingangs-Transistor
    #undef   INPUT_NOT_INVERTED         // Schaltung nach Marc Walter, Ableitung der                                      
                                        // Überspannung durch Schutzdiode des PIC
#endif

#ifdef DIY_DECODER
    #undef  INPUT_INVERTED              // normale Schaltung mit Eingangs-Transistor
    #define INPUT_NOT_INVERTED          // Schaltung nach Marc Walter, Ableitung der                                      
                                        // Überspannung durch Schutzdiode des PIC
#endif





#define   JA                            1
#define   NEIN                          0
#define   OK                            1
#define   NOK                           0

#define   GEDRUECKT                     0
#define   LOSGELASSEN                   1


#define   LICHTSPERR_ZEIT               133     // 133 x 7,5 ms = 1 sec

#ifdef FORMEL_1_BREMSLICHT
    #define   BREMSLICHT_DAUER          10      // 10 x (ca.)100 ms = 1 sec
#else
    #define   BREMSLICHT_DAUER          5       // 5 x (ca.)100 ms = 0,5 sec
#endif

#define   PROGRAMMIER_TIMEOUT           20      // 20 x (ca.)100 ms = 2 sec


#define   MAX_REGLER                    6       // 6 Regler anschließbar

#define   MAX_DATABITS                  20      // max. Pufferlänge für empf. CU Bits

#define   AUTO_1_ADRESSE                0       // 1. Auto
#define   AUTO_2_ADRESSE                1       // 2. Auto
#define   AUTO_3_ADRESSE                2       // 3. Auto
#define   AUTO_4_ADRESSE                3       // 4. Auto
#define   AUTO_5_ADRESSE                4       // 5. Auto
#define   AUTO_6_ADRESSE                5       // 6. Auto 
#define   GHOSTCAR_ADRESSE              6       // 7. Auto = Ghostcar, Adressierung beginnt mit 0
#define   PACECAR_ADRESSE               7       // 8. Auto = Pacecar, Adressierung beginnt mit 0
#define   PACE_UND_GHOSTCAR_DW_ADRESSE  7       // Pace- und Ghostcar haben eine gemeinsame Datenwort Adressse


#define   PRG_MAX_GESCHWINDIGKEIT       0       // Parameter für Programmierdatenworte
#define   PRG_BREMSWIRKUNG              1
#define   PRG_TANK_INHALT               2
#define   PRG_CU_RESET                  3
#define   PRG_NORMALBETRIEB             4
#define   PRG_TANKSTAND_INFO            10

#define   MIN_BREMSWERT                 6       // minimal von der CU vergebener Bremswert (1. LED blinkt)
#define   MAX_BREMSWERT                 15      // maximal von der CU vergebener Bremswert (5. LED dauer an)
#define   DEFAULT_BREMSWERT             10      // mittlerwer Wert als Defaulteinstellung

#define   AKTIVDATENWORT                8       // Länge des Datenwortes in Bits
#define   REGLERDATENWORT               10      // Länge des Datenwortes in Bits
#define   PROGRAMMIERDATENWORT          13      // Länge des Datenwortes in Bits



/*
 Berechnung PWM Werte der IR LEDs für Auto 1 bis 8
 Timertick ist 4µs bzw, 2 µs @ 32 MHz
 PWM Wert = Periode / Timertick - 1
   
 Auto   Frequenz        Periode/µs   Value PWM 16 MHz     Value PWM 32 MHz  
 1      15,625           64             15                  31
 2      7,8125          128             31                  63
 3      5,208333333     192             47                  95
 4      3,90625         256             63                  127  
 5      3,125           320             79                  159
 6      2,604166667     384             95                  191 
 7      2,232142857     448            111                  223
 8      1,953125        512            127                  255
 */

#define   IR_LED_PWM_CAR_1               31 
#define   IR_LED_PWM_CAR_2               63
#define   IR_LED_PWM_CAR_3               95
#define   IR_LED_PWM_CAR_4              127
#define   IR_LED_PWM_CAR_5              159  
#define   IR_LED_PWM_CAR_6              191
#define   IR_LED_PWM_CAR_7              223
#define   IR_LED_PWM_CAR_8              255


#define   PULS_START                    1
#define   PULS_KURZ                     2
#define   PULS_LANG                     3

//-------------------------------------------------- Pulslängen in µs zur Auswertung Manchester Code

#define   PULS_START_MIN                300
#define   PULS_START_MAX                7500

// kurz und lang unterschiedliche Pulsweiten für High und Low
                                        // kurze Pulse normal 50, gemessen High 35, Low 65
#define   HIGH_PULS_KURZ_MIN            25  // alt 25
#define   HIGH_PULS_KURZ_MAX            65  // alt 75
#define   LOW_PULS_KURZ_MIN             35  // alt 25
#define   LOW_PULS_KURZ_MAX             75  // alt 75

                                        // lange Pulse normal 100, gemessen High 90, Low 110
#define   HIGH_PULS_LANG_MIN            75  // alt 75
#define   HIGH_PULS_LANG_MAX            120 // alt 120
#define   LOW_PULS_LANG_MIN             85  // alt 75
#define   LOW_PULS_LANG_MAX             130 // alt 120



/*
// kurz und lang gleiche Pulsweiten für High und Low
#define   HIGH_PULS_KURZ_MIN            25  // alt 25
#define   HIGH_PULS_KURZ_MAX            75  // alt 75
#define   LOW_PULS_KURZ_MIN             25  // alt 25
#define   LOW_PULS_KURZ_MAX             75  // alt 75

                                         // lange Pulse normal 100, gemessen High 90, Low 110
#define   HIGH_PULS_LANG_MIN            75  // alt 75
#define   HIGH_PULS_LANG_MAX            120 // alt 120
#define   LOW_PULS_LANG_MIN             75  // alt 75
#define   LOW_PULS_LANG_MAX             120 // alt 120
*/



#define   PULS_UNBEKANNT                0
#define   PULS_START                    1
#define   PULS_KURZ_HIGH                2
#define   PULS_KURZ_LOW                 3
#define   PULS_LANG_HIGH                4
#define   PULS_LANG_LOW                 5


#endif	/* DEFINES_H */




 
