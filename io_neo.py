import time
import board
import neopixel
import digitalio
import analogio
import math

from util import matrix2array, wheel

pixel_pin = board.D6
num_pixels = 5*8
pixels = neopixel.NeoPixel(pixel_pin, num_pixels, brightness=0.1, auto_write=False)

def dotturn(x, y, color):
    pos = 0     
    buttons = {}
    button_keys = {"up":2, "down":4,"left":5, "right":3}
    for direction, pin in button_keys.items():
        xx = eval("board.D%d"%pin)
        #print (x)
        buttons[direction] = digitalio.DigitalInOut(xx)
        buttons[direction].direction = digitalio.Direction.INPUT
        
    while True: 
        pixels.fill((0,0,0))  # clean the screen 
        pixels.show() 
        
        # now read button input 
        if buttons["left"].value == 0:
            print ("left")
            y = (y - 1 +8)%8
        elif buttons["right"].value == 0:
            print ("right")
            y = (y + 1 )%8
        elif buttons["up"].value == 0:
            print ("up")
            x = (x-1+5)%5
        elif buttons["down"].value == 0:
            print ("down")
            x = (x+1)%5;
        
        # set cursor 
        led = matrix2array(x,y)
        pixels[led] = wheel(pos) 
        pixels.show()
            
        time.sleep(0.15)
        pos += 2 # change the color 

def dotturn_joystick(x, y, color):
    pos = 0 
    
    joy_h, joy_v = analogio.AnalogIn(board.A0), \
               analogio.AnalogIn(board.A1)
               
    value_h, value_v = joy_h.value, joy_v.value 
        
    while True: 
        pixels.fill((0,0,0))  # clean the screen 
        pixels.show() 
        
        # now read button input 
        new_value_h, new_value_v = joy_h.value, joy_v.value
        delta_h, delta_v = new_value_h - value_h, new_value_v - value_v 
        
        
        # determining dominant direction 
        if math.fabs(math.fabs(delta_h) - math.fabs(delta_v)) > 30 and \
           math.fabs(delta_h) > 30 and   math.fabs(delta_v) > 30 :
            # denoising
            if math.fabs(delta_h) > math.fabs(delta_v): 
                if delta_h > 0 : # left 
                    print ("left")
                    y = (y - 1 +8)%8
                else : # right 
                    print ("right")
                    y = (y - 1 +8)%8
            else:
                if delta_v > 0 : # up 
                    print ("up")
                    x = (x-1+5)%5
                else: # down 
                    print ("down")
                    x = (x+1)%5;
        
        # set cursor 
        led = matrix2array(x,y)
        pixels[led] = wheel(pos) 
        pixels.show()
            
        time.sleep(0.2)
        pos += 2 # change the color  

RED = (255, 0, 0)
YELLOW = (255, 150, 0)
GREEN = (0, 255, 0)
CYAN = (0, 255, 255)
BLUE = (0, 0, 255)
PURPLE = (180, 0, 255)

#dotturn(3,3, RED)
dotturn_joystick(3,3, RED)