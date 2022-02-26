#include <SoftwareSerial.h>

#define rx 2
#define tx 3
SoftwareSerial myBlue(2, 3); // RX, TX

String state = "0";
// Sensor Pins
const int photocellPin = A0;
const int tempPin = A1;
const int trigPin = 5;
const int echoPin = 4;
const int LEDpinOne = 9;
const int LEDpinTwo = 8;
const int horn = 6;
// Counters
int peripheral_counter = 0; // From Central to Peripheral
int peripheral_counter_two = 0; // From Peripheral to Central
int tempSequence = 0;
int lightSequence = 0;
int sonicSequence = 0;
// Car movement variable
char t;
// Made global
void temp();
void photocell();
void ultrasonic();

void setup() {
  Serial.begin(38400); // Default communication rate of the Serial monitor
  myBlue.begin(38400); // Default communication rate of the Bluetooth module
  // Car Motors
  pinMode(13, OUTPUT);  
  pinMode(12, OUTPUT); 
  pinMode(11, OUTPUT); 
  pinMode(10, OUTPUT); 
   
  pinMode(horn, OUTPUT);// buzzer turns on
  pinMode(LEDpinOne, OUTPUT);
  pinMode(LEDpinTwo, OUTPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(tx, OUTPUT);
  pinMode(rx, INPUT);
}
void loop() {
  if (myBlue.available() > 0) { // Checks whether data is coming
    state = myBlue.read(); // Reads the data from the serial port
    peripheral_counter++; // Add one count for central call to peripheral
    if (state == "1") // Temp reading on demand
    {
      myBlue.write("On-Demand Temperature Reading:\n");
      temp();
      myBlue.write("------------------------------------------");
      state = "0";
    }

    if (state == "2") // Photocell reading on demand
    {
      myBlue.write("On-Demand Photocell Reading:\n");
      photocell();
      myBlue.write("------------------------------------------");
      state = "0";
    }

    if (state == "3") // Ultrasonic reading on demand
    {
      myBlue.write("On-Demand Ultrasonic Reading:");
      ultrasonic();
      myBlue.write("------------------------------------------");
      state = "0";
    }
    /*
     * NOTE: This was our periodical data code, but it did not work for some reason
    if (state == "4") // Periodical sensor readings given a user input
    {
      myBlue.println("Type a sampling period: ");
      while (state == "4")
      {
        if (myBlue.available() > 0) {
          int sampling_period = myBlue.readString().toInt();
          myBlue.println(sampling_period);
          while (state == "4")
          {
            myBlue.print("Periodic Temperature Reading: \n");
            temp();

            myBlue.print("\nPeriodic Photocell Reading:");
            photocell();

            myBlue.print("\nPeriodic Ultrasonic Reading:");
            ultrasonic();
            myBlue.print("------------------------------------------");
            delay(sampling_period);
            if(myBlue.available() > 0){
              state = myBlue.read();
            }
           }
          }
        }
      }
      */
    if (state == "5") // Print total call numbers
    {
      peripheral_counter_two++;
      myBlue.print("Total number of messages sent from Central to Peripheral Device: ");
      myBlue.println(peripheral_counter);
      myBlue.print("Total number of messages sent from Peripheral to Central Device: ");
      myBlue.println(peripheral_counter_two);
      myBlue.write("------------------------------------------");
      state = "0";
    }
  }


  if (Serial.available()) {
    t = Serial.read();


    if (t == 'F') {          //move forward (all motors rotate in forward direction)
      forward();
    }

    else if (t == 'B') {    //move reverse (all motors rotate in reverse direction)
      backward();
    }

    else if (t == 'L') {    //turn right (left side motors rotate in forward direction, right side motors doesn't rotate)
      left();
    }

    else if (t == 'R') {    //turn left (right side motors rotate in forward direction, left side motors doesn't rotate)
      right();
    }
    
    else if (t == 'V') {
      tone(horn,494);
      delay(1000);
      noTone(horn);
    }
    
    else if (t == 'S') {    //STOP (all motors stop)
      stop_move();
    }

  }
}
void temp()
{
  //getting the voltage reading from the temperature sensor
  tempSequence++;
  int reading = analogRead(tempPin);

  float voltage = reading * 5.0;
  voltage /= 1024.0;
  float temperatureC = (voltage - 0.5) * 100 ;
  // now convert to Fahrenheit
  float temperatureF = (temperatureC * (9.0 / 5.0)) + 32.0;
  myBlue.print("Temperature Sequence Number: "); myBlue.println(tempSequence);
  myBlue.print(temperatureF); myBlue.write(" degrees F\n");
  peripheral_counter_two++;
}

void photocell()
{
  int photocell_reading = analogRead(photocellPin);  // reads from the analog pin (a0)
  lightSequence++;
  peripheral_counter_two++;
  myBlue.print("Photocell Sequence Number: "); myBlue.println(lightSequence);
  myBlue.print("Analog reading = ");
  myBlue.print(photocell_reading);     // the raw analog reading

  // We'll have a few threshholds, qualitatively determined by lux value
  if (photocell_reading < 10)
  {
    myBlue.println(" - Dark");
    digitalWrite(LEDpinOne, HIGH);
    digitalWrite(LEDpinTwo, HIGH);
    myBlue.println("Headlights are on.");
  }
  else if (photocell_reading < 200)
  {
    myBlue.println(" - Dim");
    digitalWrite(LEDpinOne, HIGH);
    digitalWrite(LEDpinTwo, HIGH);
    myBlue.println("Headlights are on.");
  }
  else if (photocell_reading < 500)
  {
    myBlue.println(" - Light");
    digitalWrite(LEDpinOne, LOW);
    digitalWrite(LEDpinTwo, LOW);
    myBlue.println("Headlights are off.");
  }
  else if (photocell_reading < 800)
  {
    myBlue.println(" - Bright");
    digitalWrite(LEDpinOne, LOW);
    digitalWrite(LEDpinTwo, LOW);
    myBlue.println("Headlights are off.");
  }
  else
  {
    myBlue.println(" - Very bright");
    digitalWrite(LEDpinOne, LOW);
    digitalWrite(LEDpinTwo, LOW);
    myBlue.println("Headlights are off.");
  }
}

void ultrasonic()
{
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  delay(distance);
  
  sonicSequence++;
  peripheral_counter_two++;
  myBlue.print("Ultrasonic Sequence Number: "); myBlue.println(sonicSequence);
  myBlue.print(distance);
  myBlue.println(" cm");
  
  if (distance <= 200 || distance >= 0) {
    if (distance < 11)
    {
      myBlue.println("Object is right in front of you!");
    }
    else if (distance < 26)
    {
      myBlue.println("Object is close...");
    }
    else if (distance < 51)
    {
      myBlue.println("Object is getting closer...");
    }
    else
    {
      myBlue.println("An object is in view...");
    }
  }
}

void forward()
{
  digitalWrite(12, HIGH);
  digitalWrite(11, HIGH);
}

void backward()
{
  digitalWrite(13, HIGH);
  digitalWrite(10, HIGH);
}

void left()
{
  digitalWrite(10, HIGH);
}

void right()
{
  digitalWrite(13, HIGH);
}

void stop_move()
{
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
}
