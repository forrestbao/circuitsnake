#include <Adafruit_NeoPixel.h>

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(5*8, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(100);
  Serial.begin(9600);
  
}

void loop() {
//  drivethru(strip.Color(  50,   0, 0), 150);
//  fadethru(0, 0, 120, 6, 150);
//  dotturn(3, 5, strip.Color(0,0,127));

//  // Some example procedures showing how to display to the pixels:
//  colorWipe(strip.Color(255, 0, 0), 50); // Red
//  colorWipe(strip.Color(0, 255, 0), 50); // Green
//  colorWipe(strip.Color(0, 0, 255), 50); // Blue
//  // Send a theater pixel chase in...
//  theaterChase(strip.Color(127, 127, 127), 50); // White
//  theaterChase(strip.Color(127,   0,   0), 50); // Red
//  theaterChase(strip.Color(  0,   0, 127), 50); // Blue
//
//  rainbow(20);
//  rainbowCycle(20);
//  theaterChaseRainbow(50);
}

int matrix2array(int x, int y){
  // Turn a 2D coordinate to 1D position
  // x is row, 0 to 4, y is column, 0 to 7
  return x*8+y;
  }


void dotturn(int x, int y, uint32_t color){
  // move a dot in 4 directions
  uint8_t led = matrix2array(x, y), dir;
  while (true) {
    strip.setPixelColor(led, color);
    strip.show();
    if (Serial.available() > 0){
      dir = int(Serial.read());
//      Serial.println(dir); // for debug
      switch (dir){
        case 49: // left
          y = (y-1+8)%8;     
          break;
        case 50: // right
          y = (y+1)%8;
          break;
        case 51: // up
          x = (x-1+5)%5;
          break;
        case 52: // down
          x = (x+1)%5;
          break;
      }
//    Serial.println(row); // for debug
//    Serial.println(col); // for debug
    led = matrix2array(x, y);
    strip.clear();
    }
  }
}


void fadethru(int r, uint8_t g, uint8_t b, uint8_t l, int wait){
  // Display l LEDs that fade thru the screen
  for (int8_t col = -l ; col<8; col++){
    for (uint8_t i = 0; i<l; i++){
      if (i+col < 8 && i+col>=0){// when not out of the boundary
        uint8_t led = matrix2array(2, col+i);
        float scale = pow(2, -1*(l-i)/l);
        strip.setPixelColor(led, r*scale, g*scale, b*scale);
      }
    }  
    strip.show();
    delay(wait);
    strip.clear();
  }
}

void drivethru(uint32_t c, uint8_t wait){
  // Display a single LED passing thru one column or row
    for (int r = 0; r<8; r++){
      int led = matrix2array(2, r);
      strip.setPixelColor(led, c);
      strip.show();
      delay(wait);
      strip.clear();
    }
  }

void volumebar(uint32_t c, uint8_t wait) {
  // Display a bar that grows from zero to max 
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
