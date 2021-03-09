import serial
from serial.tools import list_ports

class BLE_Serial:
    
    def __init__(self, serial_port:str = None) -> None:
        """The constructor for the BLE_Serial class, it starts the serial connection at a baud rate 
        of 115200, at the desired port. If no port is passed as an argument, it will print all available ports to the user
        and ask for his input

        Args:
            serial_port (str, optional): The serial port the device is connected to. Defaults to None.
        """
        
        # If no serial port is passed in the constructor, list all available ports and ask the user
        if serial_port == None:
            print("Chose a Serial Port:")

            for port in list_ports.comports():
                print(str(port).replace(" - n/a",""))

            serial_port = input("->")

        # Connect to the serial port with the boundrate of 
        self.serial_connection = serial.Serial(serial_port, baudrate=115200, timeout=1)

    def receive_data(self) -> str:
        """Receive the data from the serial port as a string

        Returns:
            str: The received message as a string
        """
        
        message = self.serial_connection.readline().decode('utf-8')
        return message


if __name__ == '__main__':
    
    serial_com = BLE_Serial()

    while True:
        serial_com.receive_data()