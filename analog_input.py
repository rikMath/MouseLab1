import time
import board
from analogio import AnalogIn

analog_x = AnalogIn(board.GP26)
analog_y = AnalogIn(board.GP27)

def get_voltage(pin):
    return (pin.value * 3.3) / 65536

while True:
    print((get_voltage(analog_x), get_voltage(analog_y)))
    time.sleep(0.1)
