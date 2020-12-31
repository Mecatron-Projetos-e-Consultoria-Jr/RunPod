from flask import Flask
import json
import time
import os

# instantiate the Flask server
app = Flask(__name__)

@app.route('/send_data/x_acce=<x_acceleration>/y_acce=<y_acceleration>/z_vel=<z_velocity>', methods=['GET'])
def save_data(x_acceleration,y_acceleration,z_velocity):
    
    log_to_file(x_acceleration+','+y_acceleration+','+z_velocity)


def log_to_file(data:str):

    # try to append to the file, if there is an error, most probably there is no file with the name 
    try:
        with open('log_file.csv','a') as log_file:
            log_file.write(data)
    except:
        with open('log_file.csv','w') as log_file:
            log_file.write(data)

if __name__ == '__main__':

    # run webserver
    app.run(host='0.0.0.0')