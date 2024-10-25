
#include <Arduino.h>
#include <TM1637Display.h>
#include <string>
#include <iostream>

#define CLK 5
#define DIO 6

TM1637Display display(CLK, DIO);

void updateScreen(int rawValues){
  //clears, converts int to uint8_t, and displays the current values
  uint8_t segmentCode[] = {display.encodeDigit(extractDigit(3,rawValues)), display.encodeDigit(extractDigit(2,rawValues)),
  display.encodeDigit(extractDigit(1,rawValues)),display.encodeDigit(extractDigit(0,rawValues))};  
  display.setBrightness(0x0f);
  display.setSegments(segmentCode);
}

int extractDigit(int desiredPlace, int rawValues){
  int first = rawValues/1000;
  int second = rawValues/100 - (first*1000);
  int third = rawValues/10 - (second*100) - (first*1000);
  int last = rawValues % 10;
  if (desiredPlace == 0) {return first;}
  if (desiredPlace == 1) {return second;}
  if (desiredPlace == 2) {return third;}
  if (desiredPlace == 3) {return last;}
}

void setup() {
  // initialize buttons as digital inputs:
  int inputPins [7] = {13,12,11,10,9,8,7}; //ordered from 1 to 7 <----- sync
   for (int i=0; i < 7; i++){pinMode(inputPins[i], INPUT);}
}

void loop() {

  int rawValues = 0000;
  int multiplier = 1;
  int inputPins [7] = {13,12,11,10,9,8,7}; //ordered from 1 to 7   <----- sync

    display.clear();
  
  // Display blank values !! displays values BACKWARDS !!
  uint8_t segmentCode[] = {display.encodeDigit(extractDigit(3,rawValues)), display.encodeDigit(extractDigit(2,rawValues)),
  display.encodeDigit(extractDigit(1,rawValues)),display.encodeDigit(extractDigit(0,rawValues))};
  display.setBrightness(0x0f);
  display.setSegments(segmentCode);

for (bool clear = false; clear != true;){ //  <-- main loop
  // check the state of all the buttons
  for (int i = 0; i < 7; i++){
   int stateOfButton = digitalRead( (int) inputPins[i] );
   if (stateOfButton == HIGH){
     //chech to see if the screen is already full
      if (extractDigit(3, rawValues) == 0){
        rawValues += (int)inputPins[i]*multiplier;
        multiplier *= 10;
        updateScreen(rawValues);
      }
      else {clear = true;}
    }
  }
}// <-- end not-cleared loop

}// <-- end main function

