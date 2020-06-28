#include <Adafruit_NeoPixel.h>
#include <Arduino_APDS9960.h>

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
  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor!");
  }  
}

void loop() {
    snaketurn_gesture(3,4, 3, strip.Color(0,0,127));  
//  // Some example procedures showing how to display to the pixels:
//  colorWipe(strip.Color(255, 0, 0), 50); // Red
//  colorWipe(strip.Color(0, 255, 0), 50); // Green
//  colorWipe(strip.Color(0, 0, 255), 50); // Blue
}

int matrix2array(int x, int y){
  // Turn a 2D coordinate to 1D position
  // x is row, 0 to 4, y is column, 0 to 7
  return x*8+y;
  }

void drawsnake(uint8_t snake[][2], uint8_t head_ptr, uint8_t tail_ptr){

  strip.clear(); 

  int led, x, y; 
  uint32_t head_color = strip.Color(127,0,0), tail_color = strip.Color(0,0,127), color;
  Serial.print("Light these LEDs: ");
  for (int i=tail_ptr; i < head_ptr; i++){
    x = snake[i][0]; y = snake[i][1];
    led = matrix2array(x,y);
    if (i == head_ptr) color = head_color;
    else color=tail_color; 
    strip.setPixelColor(led, color);
    Serial.print(led);
    Serial.print(" ");
  }
  Serial.println();

  strip.show(); 
}

void printsnake(uint8_t snake[][2], uint8_t head_ptr, uint8_t tail_ptr){
  
  Serial.print("Print Snake:");
  for (int i=tail_ptr; i<head_ptr; i++){
    Serial.print("(");
    Serial.print(snake[i][0]);
    Serial.print(", ");
    Serial.print(snake[i][1]);
    Serial.print(") -> ");
  }
  Serial.println();

}

void snaketurn_gesture(uint8_t head_x, uint8_t head_y, uint8_t len, uint32_t color){
  // turn a snake around using 4 hand gestures 
  // 1, 2, 3, and 4 for moving left, right, up, and down.
  // head_x, head_y: coordinate of snake head
  int dir; // length of the snake, direction of the snake moves to
  uint8_t snake[][2]  = { }; 
  uint8_t head_ptr = len, tail_ptr = 0; // pointer to head and tail in the snake array. only advances. 


  // 1. Initialize the snake 
  for (int i = 0; i<len; i++){
    snake[i][0] = head_x;
    snake[i][1] = head_y-len+i+1; // FIXME initial snake has incorrect tail (x,y-(len-1)), (x,y-(len-1)+1),..., (x,y-(len-1)+(len-1))
    // it erroratically became the 2nd last from tail side
    }

  while (true){
    delay(100);
    // Update snake head per user input

     if (APDS.gestureAvailable()) {
       // a gesture was detected, read and print to serial monitor
        int gesture = APDS.readGesture();
        Serial.println(gesture);
    
        printsnake(snake, head_ptr, tail_ptr);
        drawsnake(snake, head_ptr, tail_ptr); 

        switch (gesture){
            // TODO: add direction check, does not allow snake to grow backward
            case 1: // left
              head_y -= 1; 
              break;
            case 0: // right
              head_y += 1; 
              break;
            case 2: // up
              head_x -= 1;
              break;
            case 3: // down
              head_x += 1; 
              break;
        }

      // Add new snake head into snake
      snake[head_ptr][0] = head_x; snake[head_ptr][1] = head_y; 
  
      // Shift head and tail pointer in snake
      head_ptr += 1; tail_ptr += 1; 
     } // end if

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
