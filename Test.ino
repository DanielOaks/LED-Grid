//**************************************************************//
//  Name    : shiftOutCode, Cube_Test                            //
//  Author  : Danneh                                             //
//  Date    : 14 Nov, 2011                                       //
//  Version : 1.0                                                //
//  Notes   : Code to control an LED grid using a 74HC595        //
//          : Shift Register, runs through several patterns      //
//**************************************************************//
#include <TrueRandom.h>

int latchPin = 8;  //Pin connected to ST_CP of 74HC595
int clockPin = 12; //Pin connected to SH_CP of 74HC595
int dataPin = 11;  //Pin connected to DS    of 74HC595

#define CubeSize (4)
int cube[CubeSize][CubeSize];

unsigned long millis_last_updated = 0; //millisecond last updated
int currentPattern = 0; //pattern number
#define TotalPatterns (2)
int newPattern = 0; //whether or not we switched patterns last tick

int millis_between_patterns = 5000;

int delay_ticks = 0;

void setup() {
  pinMode(latchPin, OUTPUT);
  cubeClear();
  cubeShiftOut();
  millis_last_updated = millis();
}

void loop() {
  progress();
  cubeShiftOut();
  
  delay(delay_ticks);
  
  unsigned long current_millis = millis();
  if (current_millis - millis_last_updated > millis_between_patterns) {
    currentPattern++;
    if (currentPattern >= TotalPatterns) {
      currentPattern = 0;
    }
    //cubeClear();
    //cubeShiftOut();
    millis_last_updated = current_millis;
  }
}

void progress() {
  switch (currentPattern) {
    case 0: //*** matrix pattern vertical
      delay_ticks = TrueRandom.random(0, 222);
      for (int y = CubeSize-1; y > 0 ; y--) {
        for (int x = 0; x < CubeSize; x++) {
          if (cube[x][y-1] == 1) {
            cube[x][y] = 1;
          }
          else {
            cube[x][y] = 0;
          }
        }
      }
      for (int x = 0; x < CubeSize; x++) {
        if (TrueRandom.random(1,10) == 7) {
          cube[x][0] = 1;
        }
        else {
          cube[x][0] = 0;
        }
      }
      break;
      
    case 1: //*** matrix pattern horisontal
      delay_ticks = TrueRandom.random(0, 222);
      for (int x = CubeSize-1; x > 0 ; x--) {
        for (int y = 0; y < CubeSize; y++) {
          if (cube[x-1][y] == 1) {
            cube[x][y] = 1;
          }
          else {
            cube[x][y] = 0;
          }
        }
      }
      for (int y = 0; y < CubeSize; y++) {
        if (TrueRandom.random(1,10) == 7) {
          cube[0][y] = 1;
        }
        else {
          cube[0][y] = 0;
        }
      }
      break;
    
    default: //*** test pattern, just incase
      delay_ticks = 0;
      for (int x = 0; x < CubeSize; x++) {
        for (int y = 0; y < CubeSize; y++) {
          cube[x][y] = 1;
        }
      }
  }
}

void cubeClear() {
  for (int i = 0; i <= (CubeSize * CubeSize); i++) {
    bitShiftOut(dataPin, clockPin, 0);
  }
  for (int x = 0; x < CubeSize; x++) {
    for (int y = 0; y < CubeSize; y++) {
      cube[x][y] = 0;
    }
  }
}

void cubeShiftOut() {
  digitalWrite(latchPin, 0);
  for (int y = CubeSize-1; y >= 0; y--) {
    for (int x = CubeSize-1; x >= 0; x--) {
      bitShiftOut(dataPin, clockPin, cube[x][y]);
    }
  }
  digitalWrite(latchPin, 1);
}

void bitShiftOut(int myDataPin, int myClockPin, int myDataOut) {
  //setup
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);
  
  //write bit
  digitalWrite(myDataPin, myDataOut);
  digitalWrite(myClockPin, 1);
  digitalWrite(myDataPin, 0);
  
  //stop shifting
  digitalWrite(myClockPin, 0);
}
