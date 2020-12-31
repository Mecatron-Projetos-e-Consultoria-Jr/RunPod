import numpy as np

class vector():

    def __init__(self, size):

        self.size = size
        self.data = np.zeros(size)
        self.last_entry_index = 0
    
    def push(self, data_point):

        # If the array is not yet full, just add to the next index
        if self.last_entry_index < self.size:
            self.data[self.last_entry_index] = data_point
            self.last_entry_index+=1
            
        else:
            self.data = np.roll(self.data,1)
            self.data[-1] = data_point 