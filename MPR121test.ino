/*  These sensors use I2C communicate, at least 2 pins are required to interface*/
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIXEL_PIN    9  // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 241 // Number of NeoPixels

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRBW + NEO_KHZ800);// This declares the name of the led strip

int     mode     = 0;    // Different modes happen when the button is pushed
//////////////////////////////////////////////////////////////END OF BUTTON DECLARATIONS////////////////////////////////////////////////

#include <Wire.h>
#include "Adafruit_MPR121.h" //downloaded from Tools>Manage Libraries

#ifndef _BV
#define _BV(bit) (4 << (bit))
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();/////Set up for touch sensor, same and Pin Input declaration

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;
boolean newState;
 int releasedWaitingForTouch = 0;

void setup() {
  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.show();  // Initialize all pixels to 'off'
  Serial.begin(9600);

//  cap.setThresholds(12, 6); //use begin(); to get it out
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) { ///cap.begin(0x5A);
    ////   if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");
}

void loop() {
  // Get the currently touched pads
  currtouched = cap.touched();
  strip.setBrightness(255);
  uint8_t padnumber = 4;

  for (uint8_t i = 0; i < 12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" touched");
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" released");
    }
  }
  if  ((currtouched & _BV(padnumber)) && !(lasttouched & _BV(padnumber)) )
  {
    // Short delay to debounce button.
    delay(20);///was 20

      if (++mode > 9) mode = 0; // Advance to next mode, wrap around after #8
      switch (mode) {          // Start the new animation...
        case 0:
          /// newState = HIGH;
          colorWipe(strip.Color(  255,   255,   255), 5);    // White
          break;

        case 1:
          colorWipe(strip.Color(255,   0,   0), 5); // Red
                          break;
        case 2:
          colorWipe(strip.Color(255,   85,   0), 5); // Orange
          break;
        case 3:
          colorWipe(strip.Color(255,   165,   0), 5); // Yellow
          break;
        case 4:
          colorWipe(strip.Color(0,   255,   0), 5); // Green
          break;
        case 5:
          colorWipe(strip.Color(0,   170,   85), 5); // BlueGreen
          break;
        case 6:
          colorWipe(strip.Color(  0,   0,   255), 5); // Blue
          break;
        case 7:
          colorWipe(strip.Color(  85,   0,   170), 5); // Purple
          break;
        case 8:
          colorWipe(strip.Color(  170,   0,   85), 5); // Magenta
          break;
        case 9:
        boolean nottouched = true;
          rainbow(1);
          
          while (nottouched == true){
             
          for (long firstPixelHue = 0; firstPixelHue < 1 * 65536; firstPixelHue += 350) {   ///750 was 256 and 3
        for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
        int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());///L equals the value passed above by pixel hue
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));     
       }
      strip.show(); // Update strip with new contents
      delay(10);  // Pause for a moment
      
          if (cap.touched() & (1<<4) ) 
        { Serial.println("FARRT");
        nottouched = false;
       break;
        }
        
                   }
                   
    }
         

    
      
      }
    }


  // reset our state
  lasttouched = currtouched;

  // comment out "return;" to look like "//return;" get detailed data from the sensor!//////////////////////////////SERIAL MONITOR PRINTOUT CODE/////////////////////////
  return;

  // debugging info, what
  Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap.touched(), HEX);
  Serial.print("Filt: ");
  for (uint8_t i = 0; i < 12; i++) {
    Serial.print(cap.filteredData(i)); Serial.print("\t");
  }
  Serial.println();
  Serial.print("Base: ");
  for (uint8_t i = 0; i < 12; i++) {
    Serial.print(cap.baselineData(i)); Serial.print("\t");
  }
  Serial.println();

  // put a delay so it isn't overwhelming
  delay(100);
}


// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 3 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 3*65536. Adding 256 to firstPixelHue each time
  // means we'll make 3*65536/256 = 768 passes through this outer loop:
      
    for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 750) {   ///750 was 256 and 3
        for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
        // Offset pixel hue by an amount to make one full revolution of the
        // color wheel (range of 65536) along the length of the strip
        // (strip.numPixels() steps):
        int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());///L equals the value passed above by pixel hue
        // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
        // optionally add saturation and value (brightness) (each 0 to 255).
        // Here we're using just the single-argument hue variant. The result
        // is passed through strip.gamma32() to provide 'truer' colors
        // before assigning to each pixel:
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));     
       }
       
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
                   } }
       
   
