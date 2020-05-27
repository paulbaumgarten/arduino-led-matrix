# arduino-led-matrix
Using an 16x32 LED matrix for a digital sign

Currently based on the example text scroller code provided by Adafruit.

Intention is to add a wifi module so the text can be set via a remote application.

## Requirements

In your Arduino IDE / Manage Libraries, install the following:

* Adafruit GFX Library by Adafruit
* RGB maxtrix Panel by Adafruit

## Wiring

Looking at the rear of the matrix, 6there is a ribbon cable connector set of pins (2 columns of 9 pins). This was the wiring my matrix required:

* Left 0 to Arduino GND
* Left 1 to none
* Left 2 to none
* Left 3 to Arduino A1
* Left 4 to Arduino D2
* Left 5 to Arduino D3
* Left 6 to Arduino D4
* Left 7 to Arduino D5
* Left 8 to Arduino D6
* Right 0 to Arduino D8
* Right 1 to Arduino D10
* Right 2 to Arduino D9
* Right 3 to Arduino A2
* Right 4 to Arduino A0
* Right 5
* Right 6
* Right 7 to Arduino D7

