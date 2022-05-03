/*

Example for ESP_8_BIT color composite video generator library on ESP32.
Connect GPIO25 to signal line, usually the center of composite video plug.

GFX Hello World

This demonstrates using the ESP_8_BIT_GFX class, which inherits from the
Adafruit GFX base class to deliver an easy to use graphics API. Draws two
rectangles that cycle around the border of the screen. The amount of corners
cut off from these rectangle show the amount of overscan on a particular
screen. In the middle of two rectangles are a bit of text drawn using
Adafruit GFX print() API.

Copyright (c) Roger Cheng

MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include <ESP_8_BIT_GFX.h>

#define DEBUG_LED  2

// A list of 8-bit color values that work well in a cycle.
uint8_t colorCycle[] = {
  0xFF, // White
  0xFE, // Lowering blue
  0xFD,
  0xFC, // No blue
  0xFD, // Raising blue
  0xFE,
  0xFF, // White
  0xF3, // Lowering green
  0xE7,
  0xE3, // No green
  0xE7, // Raising green
  0xF3,
  0xFF, // White
  0x9F, // Lowering red
  0x5F,
  0x1F, // No red
  0x5F, // Raising red
  0x9F,
  0xFF
};

// Create an instance of the graphics library
ESP_8_BIT_GFX videoOut(false /* = NTSC */, 8 /* = RGB332 color */);


const int numMolecules = 245;
const int xRes = 256;
const int yRes = 240;


uint8_t positions[numMolecules];  // holds the intensity values for each lamp
int byteCount = 0;

bool stringStart = false;
bool stringComplete = false;  // whether the string is complete


void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(DEBUG_LED,OUTPUT);
  digitalWrite(DEBUG_LED,LOW);

  
  // Initial setup of graphics library
  videoOut.begin();
}

void loop() {
  digitalWrite(DEBUG_LED,LOW);

  
  // Wait for the next frame to minimize chance of visible tearing
  videoOut.waitForFrame();

//  // Get the current time and calculate a scaling factor
//  unsigned long time = millis();
//  float partial_second = (float)(time % 1000)/1000.0;
//
//  // Use time scaling factor to calculate coordinates and colors
//  uint8_t movingX = (uint8_t)(255*partial_second);
//  uint8_t invertX = 255-movingX;
//  uint8_t movingY = (uint8_t)(239*partial_second);
//  uint8_t invertY = 239-movingY;
//
//  uint8_t cycle = colorCycle[(uint8_t)(17*partial_second)];
//  uint8_t invertC = 0xFF-cycle;

  // Clear screen
  videoOut.fillScreen(0xFC);

  // only run if a full command string is avaliable and its not reading a new one atm
  if (stringComplete && !stringStart) {

    digitalWrite(DEBUG_LED,HIGH);
//    Serial.println("yes");

    for (int i = 0; i < 3; i+=2) {

      // map the intensities to suitable dimmer values
      float mappedX = map(positions[i], 0., 125., 0., xRes);
      float mappedY = map(positions[i+1], 0., 125., 0., yRes);
//      float dimVal = constrain((float)intensities[i], 0, 1000);
//      dimVal = map(dimVal, 0., 125., dimMIN, dimMAX);

      // draw a circle on the respective position
      videoOut.drawCircle(mappedX, mappedY, 20, 0xFF);
      videoOut.fillCircle(mappedX, mappedY, 20, 0xFF);

    }



    stringComplete = false;

  }


  videoOut.drawCircle(100, 100, 3, 0xE3);
  videoOut.fillCircle(100, 100, 3, 0xE3);

  

//  // Draw one rectangle
//  videoOut.drawLine(movingX, 0,       255,     movingY, cycle);
//  videoOut.drawLine(255,     movingY, invertX, 239,     cycle);
//  videoOut.drawLine(invertX, 239,     0,       invertY, cycle);
//  videoOut.drawLine(0,       invertY, movingX, 0,       cycle);
//
//  // Draw a rectangle with inverted position and color
//  videoOut.drawLine(invertX, 0,       255,     invertY, invertC);
//  videoOut.drawLine(255,     invertY, movingX, 239,     invertC);
//  videoOut.drawLine(movingX, 239,     0,       movingY, invertC);
//  videoOut.drawLine(0,       movingY, invertX, 0,       invertC);
//
//  // Draw text in the middle of the screen
//  videoOut.setCursor(25, 80);
//  videoOut.setTextColor(invertC);
//  videoOut.setTextSize(2);
//  videoOut.setTextWrap(false);
//  videoOut.print("Adafruit GFX API");
//  videoOut.setCursor(110, 120);
//  videoOut.setTextColor(0xFF);
//  videoOut.print("on");
//  videoOut.setCursor(30, 160);
//  videoOut.setTextColor(cycle);
//  videoOut.print("ESP_8_BIT video");

  if (Serial.available()) {
    serialEvent();
  }
}



void serialEvent() {

  
//  delay(100);
//  digitalWrite(DEBUG_LED,LOW);

  while (Serial.available()) {
    // get the new byte:
//    uint8_t inChar = (uint8_t)Serial.read();
    char inChar = (char)Serial.read();


    // only check for serial data if its not processing the previous one in the loop function
    if (!stringComplete) {


      // if a starting byte has been detected read the upcoming bytes
      if (stringStart) {


        //  if the incoming byte is 254 -> means string ended
        if (inChar == 126) {

          // check if a full string is read (52 bytes) and set the stringComplete flag
          // if not, the string is damaged therefore discard it and search for a new one
          if (byteCount == 3) {
            stringComplete = true;
          }

          stringStart = false;
          byteCount = 0;

        } else {

          // add the incoming byte as integer value to the intensities array
//          positions[byteCount] = (int)inChar;
          positions[byteCount] = inChar;
          byteCount++;

        }

      }

      // if the incoming byte is 255 -> means new string starting
      // hence set the stringStart flag
      if (inChar == 127) {
        stringStart = true;
      }

    }

  }

}
