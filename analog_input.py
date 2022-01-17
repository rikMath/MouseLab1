import time
import board
from analogio import AnalogIn

analog_x = AnalogIn(board.GP26)
analog_y = AnalogIn(board.GP27)

previous_x = analog_x.value
previous_y = analog_y.value

divisor = 65536 / 2

class Variations:
    def __init__(self, initial_x, initial_y):
        self.previous_x = initial_x
        self.previous_y = initial_y
        
    def variation_value_x(self, new_x):
        global divisor
        variation = (new_x - self.previous_x) / divisor
        self.previous_x = new_x
        
        return variation

    def variation_value_y(self, new_y):
        global divisor
        variation = (new_y - self.previous_y) / divisor
        self.previous_y = new_y
        
        return variation

variations = Variations(analog_x.value, analog_y.value)

while True:
    print((variations.variation_value_x(analog_x.value), variations.variation_value_y(analog_y.value)))
    time.sleep(1)

