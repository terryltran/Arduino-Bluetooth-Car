#include <SoftwareSerial.h> 
#define rx 10
#define tx 9
SoftwareSerial myBlue(rx, tx); // RX | TX 
int terminal_counter = 0; // Counting how many messages are being sent from the terminal to central
void setup() 
{   
 Serial.begin(38400); 
 myBlue.begin(38400); 
 pinMode(tx, OUTPUT);
 pinMode(rx, INPUT);
} 
void loop() 
{ 
  if(Serial.available() > 0)
  {
     terminal_counter++;
     int state = Serial.readString().toInt();
     if (state == 5)
     {
        Serial.print("Total number of messages sent from Terminal to Central Device: ");
        Serial.println(terminal_counter);
     }
     myBlue.write(state);
  }

  if(myBlue.available() > 0)
  {
    while(myBlue.available())
    {
      Serial.println(myBlue.readString());
    }
  }
}  
