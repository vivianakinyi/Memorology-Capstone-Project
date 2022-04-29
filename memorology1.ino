#include <CapacitiveSensor.h>
#include "pitches.h"

CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired

// You may need to adjust this value to work for your particular sensor.
// Use the capacitive sensing test sketch to get a sense of the range of values
// coming from your capacitive sensor.
int speakerPin = 7;
int threshold = 100;
// int noteDuration = 10; // ms
// note durations: 4 = quarter note, 8 = eighth note, etc.:
//int noteDuration[] = {
//
//  4, 8, 8, 4, 4, 4, 4, 4
//};
int toneVal;


// Play happy birthday melody change this to make the song slower or faster
int tempo = 140;

// piezzo pin
int buzzer = 7;

// LED logic
// ----------------------------------------------------
// Red LED, connected to digital pin 9
const int redPin   = 9;
// Green LED, connected to digital pin 10
const int greenPin = 10;
// Blue LED,  connected to digital pin 11
const int bluePin  = 11;

// Variables to store the values to send to the pins.
// Initial values are Red full, Green and Blue off.
int redVal   = 255; 
int greenVal = 1;   
int blueVal  = 1;

// Tracks how many 'steps' into our fade we are.
unsigned i = 0;
// The number of ms to wait between steps in the fade. Smaller values will result
// In a faster fade.
const int wait = 5;
// Determines if debug information should be printed to the serial monitor.
const bool DEBUG = false;
// ----------------------------------------------------

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  // Happy Birthday
  // Score available at https://musescore.com/user/8221/scores/26906

  NOTE_C4,4, NOTE_C4,8, 
  NOTE_D4,-4, NOTE_C4,-4, NOTE_F4,-4,
  NOTE_E4,-2, NOTE_C4,4, NOTE_C4,8, 
  NOTE_D4,-4, NOTE_C4,-4, NOTE_G4,-4,
  NOTE_F4,-2, NOTE_C4,4, NOTE_C4,8,

  NOTE_C5,-4, NOTE_A4,-4, NOTE_F4,-4, 
  NOTE_E4,-4, NOTE_D4,-4, NOTE_AS4,4, NOTE_AS4,8,
  NOTE_A4,-4, NOTE_F4,-4, NOTE_G4,-4,
  NOTE_F4,-2,
 
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;


void setup()                    
{
  cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
  // pinMode(LED_BUILTIN, OUTPUT);
  pinMode(redPin,   OUTPUT);
  pinMode(greenPin, OUTPUT);   
  pinMode(bluePin,  OUTPUT); 
  
  pinMode(speakerPin, OUTPUT);
  Serial.begin(9600);

}

void loop()                    
{
  long capVal =  cs_4_2.capacitiveSensor(30);
  
  Serial.println(capVal);
  
  if ( capVal > threshold ) {
//    tempo = capVal;
//    noteDuration = capVal; 

    // digitalWrite(LED_BUILTIN, HIGH);
    
    buzzerFn(capVal);
    ledBlink();
    
  } else {
    toneVal = capVal; 
    
    //digitalWrite(LED_BUILTIN, LOW);
    noTone(buzzer);
    ledBlink();
  }

}
void buzzerFn(int capVal) {
  Serial.println("I have been passed along");
  Serial.println(capVal);
  
  // play happy birthday song 
  // iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    if ( capVal )

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);
    
    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
  
}
void ledBlink() {
  
  i += 1;
  if (i < 255) {
    redVal   -= 1; // Red down
    greenVal += 1; // Green up
    blueVal   = 1; // Blue low
  } else if (i < 509) {
    redVal    = 1; // Red low
    greenVal -= 1; // Green down
    blueVal  += 1; // Blue up
  } else if (i < 763) {
    redVal  += 1; // Red up
    greenVal = 1; // Green low
    blueVal -= 1; // Blue down
  } else {
    i = 0;
  }  

  analogWrite(redPin,   redVal);
  analogWrite(greenPin, greenVal); 
  analogWrite(bluePin,  blueVal);

}

//void my_delay(int milsec){
//  int c = 1;
//  for ( int i = 0; i < milsec; i++ ) {
//        c = c * (-1); Serial.print("delay running\n");
//  }
//}
