// scrolltext demo for Adafruit RGBmatrixPanel library.
// Demonstrates double-buffered animation on our 16x32 RGB LED matrix:
// http://www.adafruit.com/products/420

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include <RGBmatrixPanel.h>
#include <SoftwareSerial.h>

// Most of the signal pins are configurable, but the CLK pin has some
// special constraints.  On 8-bit AVR boards it must be on PORTB...
// Pin 8 works on the Arduino Uno & compatibles (e.g. Adafruit Metro),
// Pin 11 works on the Arduino Mega.  On 32-bit SAMD boards it must be
// on the same PORT as the RGB data pins (D2-D7)...
// Pin 8 works on the Adafruit Metro M0 or Arduino Zero,
// Pin A4 works on the Adafruit Metro M4 (if using the Adafruit RGB
// Matrix Shield, cut trace between CLK pads and run a wire to A4).

#define CLK  8   // USE THIS ON ARDUINO UNO, ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
//#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2

// Last parameter = 'true' enables double-buffering, for flicker-free,
// buttery smooth animation.  Note that NOTHING WILL SHOW ON THE DISPLAY
// until the first call to swapBuffers().  This is normal.
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
// Double-buffered mode consumes nearly all the RAM available on the
// Arduino Uno -- only a handful of free bytes remain.  Even the
// following string needs to go in PROGMEM:

SoftwareSerial wifiSerial(11, 12); // RX, TX

// Similar to F(), but for PROGMEM string pointers rather than literals

char    content[255] = "waiting...";
int     contentMarker = 10;
int16_t textX         = matrix.width();
int16_t textMin       = contentMarker * -12;
int16_t hue           = 0;

void setup() {
  matrix.begin();
  matrix.setTextWrap(false); // Allow text to run off right edge
  matrix.setTextSize(2);
  wifiSerial.begin(1200);
  Serial.begin(9600);
  Serial.println("waiting...");
}

bool capture = false;

void loop() {
  // Read data from serial
  if (wifiSerial.available() > 0) {
    // Serial.print("incoming data...");
    // Serial.println(wifiSerial.available());
    char c = wifiSerial.read();
    if (byte(c) == 61) { // Equals sign
        contentMarker=0;
        for (int i=0; i<255; i++) {
          content[i] = ' ';
        }
        capture = true;
        Serial.println("resetting content");
    } else if (isPrintable(c)) {
        content[contentMarker++] = c;
        textMin = contentMarker * -12;
        Serial.print(c);
        Serial.print(" ");
        Serial.println(byte(c));
    }
  }

  // Clear background
  matrix.fillScreen(0);

  // Draw big scrolly text on top
  matrix.setTextColor(matrix.ColorHSV(hue, 255, 255, true));
  matrix.setCursor(textX, 1);
  matrix.print(content);

  // Move text left (w/wrap), increase hue
  --textX;
  if (textX < textMin) {
    textX = matrix.width();
  }
  hue += 7;
  if(hue >= 1536) hue -= 1536;

  delay(60);
  // Update display
  matrix.swapBuffers(false);
}
