import board
import digitalio
import time

s, e= 5, 7

LEDs = []

for i in range(s, e+1):
    x = eval("board.D%d"%i)
#    print (x)
    led = digitalio.DigitalInOut(x)
    led.direction = digitalio.Direction.OUTPUT
    LEDs.append(led)

while True:
    for led in LEDs:
        led.value = True
        time.sleep(0.2)
        led.value = False
        time.sleep(0.2)

