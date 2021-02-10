/*********************************************************
  This is a library for the MPR121 12-channel Capacitive touch sensor

  Designed specifically to work with the MPR121 Breakout in the Adafruit shop
  ----> https://www.adafruit.com/products/

  These sensors use I2C communicate, at least 2 pins are required
  to interface

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
**********************************************************/
///////////////////////////////////////////////////////////////////////BUTTON CYCLER DECLARATIONS/////////////////////////////////////////////////
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Digital IO pin connected to the button. This will be driven with a
// pull-up resistor so the switch pulls the pin to ground momentarily.
// On a high -> low transition the button press logic will execute.
#define BUTTON_PIN   2

#define PIXEL_PIN    6  // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 288  // Number of NeoPixels

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRBW + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

boolean oldState = HIGH;
int     mode     = 0;    // Currently-active animation mode, 0-9
//////////////////////////////////////////////////////////////END OF BUTTON CYCLER DECLARATIONS////////////////////////////////////////////////



#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();/////SET UP INITIALIZING STUFF, SAME AS DECLARING PINS FOR OUTPUTS

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

void setup() {
  ////////////////////////////////BUTTON CYCLER SETUP///////////////////////////////////////////////////////////////////////////////////

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.show();  // Initialize all pixels to 'off'

  ////////////////////////////////BUTTON CYCLER SETUP///////////////////////////////////////////////////////////////////////////////////

  Serial.begin(9600);

  while (!Serial) { // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }

  Serial.println("Adafruit MPR121 Capacitive Touch sensor test");

  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");
}

void loop() {
  ///////////////////////////////////////////////////BUTTON LOOP CODE Variables//////////////////////////////////////////////////
  // Get current button state.
  ////////////////////////// boolean newState = digitalRead(BUTTON_PIN); COMMENTED OUT TO GIVE newState variable to touch function
  ///////////////////////////////////////////////////END BUTTON LOOP CODE Variables//////////////////////////////////////////////////
  boolean newState;



  // Get the currently touched pads
  //////////// currtouched = cap.touched();   changed to give newState to the currtouched variable
  newState = cap.touched();



  if ((newState == LOW) && (oldState == HIGH)) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.


    //    newState = digitalRead(BUTTON_PIN);
    newState = currtouched;


 ////////////////   if (newState == LOW) {     // Yes, still low
    if (newState == LOW) {     // Yes, still low


      
      if (++mode > 9) mode = 0; // Advance to next mode, wrap around after #8
      switch (mode) {          // Start the new animation...
        case 0:
          //colorWipe(strip.Color(  0,   0,   0), 5);    // Black/off
           strip.fill( strip.Color(0, 0, 0, 0) , 0, 288            );
          strip.show();
          break;
        case 1:

          //    colorWipe(strip.Color(255,   0,   0), 5);    // Red
          //   strip.clear();
          strip.fill( strip.Color(255, 0, 0, 0) , 0, 288            );
          //    strip.Color(255,   0,   0);    // Red
          strip.show();
          //delay(50);
          break;
        case 2:
          strip.fill( strip.Color(0, 255, 0, 0) , 0, 288            );
          strip.show();

          // colorWipe(strip.Color(  0, 255,   0), 5);    // Green
          break;
        case 3:
          strip.fill( strip.Color(0, 0, 255, 0) , 0, 288           );
          strip.show();
          //          colorWipe(strip.Color(  0,   0, 255), 5);    // Blue
          break;

        case 4:
          strip.fill( strip.Color(255, 255, 0, 0) , 0, 288            ); //yellow
          strip.show();
          break;
        case 5:
          strip.fill( strip.Color(0, 255, 255, 0) , 0, 288            ); //aqua
          strip.show();
          break;
        case 6:
          strip.fill( strip.Color(255, 0, 255, 0) , 0, 288            ); //magenta
          strip.show();
          break;
        case 7:
          strip.fill( strip.Color(255, 255, 255, 0) , 0, 288            ); //white
          strip.show();
          break;
        case 8:
          strip.fill( strip.Color(255, 255, 255, 0) , 0, 288            ); //white
          strip.show();
          break;
          case 9:
          rainbow(5);
          break;

/*

        case 9:
          theaterChase(strip.Color(127, 127, 127), 50); // White
          break;
        case 10:
          theaterChase(strip.Color(127,   0,   0), 50); // Red
          break;
        case 11:
          theaterChase(strip.Color(  0,   0, 127), 50); // Blue
          break;
        case 12:
          rainbow(5);
          break;
        case 13:
          theaterChaseRainbow(10);
          break;
*/

          
      }
    }
  }

  // Set the last-read button state to the old state.
  oldState = newState;



  for (uint8_t i = 0; i < 12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" touched");
      theaterChase(strip.Color(127, 127, 127), 5); // White
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" released");
      colorWipe(strip.Color(  0,   0,   0), 5);    // Black/off



    }
  }

  // reset our state
  lasttouched = currtouched;



  // comment out this line for detailed data from the sensor!//////////////////////////////SERIAL MONITOR PRINTOUT CODE/////////////////////////
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

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for (int a = 0; a < 10; a++) { // Repeat 10 times...
    for (int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for (int c = b; c < strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 3 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 3*65536. Adding 256 to firstPixelHue each time
  // means we'll make 3*65536/256 = 768 passes through this outer loop:
  for (long firstPixelHue = 0; firstPixelHue < 3 * 65536; firstPixelHue += 256) {
    for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for (int a = 0; a < 30; a++) { // Repeat 30 times...
    for (int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for (int c = b; c < strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
