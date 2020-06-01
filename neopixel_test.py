#import neopixel_test.py

import time
import board
import neopixel
import digitalio

pixel_pin = board.D6
num_pixels = 5*8

pixels = neopixel.NeoPixel(pixel_pin, num_pixels, brightness=0.1, auto_write=False)


def color_chase(color, wait):
    for i in range(num_pixels):
        pixels[i] = color
        time.sleep(wait)
        pixels.show()
    time.sleep(0.5)


def rainbow_cycle(wait):
    for j in range(255):
        for i in range(num_pixels):
            rc_index = (i * 256 // num_pixels) + j
            pixels[i] = wheel(rc_index & 255)
        pixels.show()
        time.sleep(wait)



def fadethru(r,g,b, l):
    for col in range(-l, 8):
#        print ("new round")
        pixels.fill((0,0,0))
        pixels.show()
        for i in range(l):
            if i+col <8 and i+col >= 0 :
                led = matrix2array(2, col+i)
                print (led)
                scale = 2.54**(-1*(l-i)/l)
                pixels[led] = wheel(led*10) # (int(r*scale), int(g*scale), int(b*scale))
        pixels.show()
        time.sleep(0.2)

def redgreenalter():
    pixels[0] = RED
    pixels[1] = GREEN
    pixels.show()
    time.sleep(1)
    pixels[1] = RED
    pixels[0] = GREEN
    pixels.show()
    time.sleep(1)

RED = (255, 0, 0)
YELLOW = (255, 150, 0)
GREEN = (0, 255, 0)
CYAN = (0, 255, 255)
BLUE = (0, 0, 255)
PURPLE = (180, 0, 255)

while True:
    fadethru(0,12,0, 4)
#    pixels.fill(RED)
#
#    # Increase or decrease to change the speed of the solid color change.
#    time.sleep(1)
#    pixels.fill(GREEN)
#    pixels.show()
#    time.sleep(1)
#    pixels.fill(BLUE)
#    pixels.show()
#    time.sleep(1)

#    color_chase(RED, 0.1)  # Increase the number to slow down the color chase
#    color_chase(YELLOW, 0.1)
#    color_chase(GREEN, 0.1)
#    color_chase(CYAN, 0.1)
#    color_chase(BLUE, 0.1)
#   color_chase(PURPLE, 0.1)

#    rainbow_cycle(0)  # Increase the number to slow down the rainbow