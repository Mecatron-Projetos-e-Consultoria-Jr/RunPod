from flask import Flask
from termcolor import colored
import time
import sys
import os

# Get the argument from the commandline
csv_file = sys.argv[1]

# instantiate the Flask server
app = Flask(__name__)

@app.route('/send_data/data=<data>', methods=['GET'])
def save_data(data):

    global csv_file
    
    # Open the desired file in append mode and add the nex line 'time,data'
    with open(csv_file, 'a') as csv:
        csv.write(str(time.time())+','+data+'\n')

    return 'ok'



def create_file(name:str):
    # check to see if the name of the file passed already existis (to avoid overwriting)
    files_in_directory = os.listdir()
    if name in files_in_directory:
        raise ValueError(colored('FILE ALREADY EXISTS: '+name, 'red'))
    else:
        # os.system('touch '+name)
        with open(name, 'w') as _:
            ...


if __name__ == '__main__':

    # Create a file with the name passed as parameter
    create_file(csv_file)

    # run webserver
    app.run(host='0.0.0.0')