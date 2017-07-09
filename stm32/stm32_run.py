import time
import sys
import RPi.GPIO as GPIO

io_1 = 12

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(io_1, GPIO.OUT)

argv1 = sys.argv[1]
if argv1 == 'on':
    GPIO.output(io_1, 1)
    print 'stm32 turn on'
elif argv1 == 'off':
    GPIO.output(io_1, 0)
    print 'stm32 turn off'
else:
    print 'invalid args'
#GPIO.cleanup()
