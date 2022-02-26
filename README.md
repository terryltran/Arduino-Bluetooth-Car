# Arduino-Bluetooth-Car

Created by Terry Tran and Raphael Juco

The "Final_Bluetooth_Master" file is the code for the master Arduino (Arduino Mega2560) that takes in user input via the Serial monitor
and sends commands to the slave Arduino (Arduino Uno). If there is data that is sent from the slave to the master
via Bluetooth, the master Arduino takes that data and prints it out on the Serial monitor. This is done using the SoftwareSerial library.
The Bluetooth devices used are three HC-05 Bluetooth modules (two paired to each other for the master and slave communication, and one 
paired to an Android phone). 

The "Final_Bluetooth_Slave" file is the code for the Arduino Uno that takes commands from the Arduino Mega (via Bluetooth), and does all
car movement, data collecting, and sending data back to the Arduino Mega (either on-demand or periodically). The data is collect from 
sensors/actuators connected onto the car (which is all on the Arduino Uno). The car moves through using the code for the car movement 
and an Android phone app. 


