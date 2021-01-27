import matplotlib.pyplot as plt
from scipy.ndimage import gaussian_filter
import json

with open('log_file.csv', 'r') as data_file:
    
    raw_data = data_file.read()
    raw_data = raw_data.split('\n')[:-1]

    time = [float(line.split(',')[0]) for line in raw_data]
    x_acceleration = [float(line.split(',')[1]) for line in raw_data]
    y_acceleration = [float(line.split(',')[2]) for line in raw_data]
    z_velocity = [float(line.split(',')[3]) for line in raw_data]
    
    plt.plot(time,x_acceleration,label='x_acceleration')
    plt.plot(time, gaussian_filter(x_acceleration,sigma=5), label='x_gaussian')
    # plt.plot(time,y_acceleration,label='y_acceleration')
    # plt.plot(time,z_velocity,label='z_velocity')
    plt.legend(loc="upper left")
    plt.show()