#include <Wire.h>//Required for transmision between 2 NANOs
#define motor1A 7//Initializing Motor(For Opening Handle)
#define motor1B 8//Initializing Motor(For Opening Handle)
#define motor2A 4//Initializing Motor(For Opening Door)
#define motor2B 2//Initializing Motor(For Opening Door)
#define motor3A 12//Initializing Motor(For Pumping Sanitizer)
#define motor3B 13//Initializing Motor(For Pumping Sanitizer)

void setup()
{
  Wire.begin(0x40);//Beginnig Transmision between NANOs
  Wire.onReceive(receiveEvent);//When Something is recieved
  Serial.begin(9600);
  pinMode(motor1A, OUTPUT);//Declaring motor as OUTPUT
  pinMode(motor1B, OUTPUT);//Declaring motor as OUTPUT
  pinMode(motor2A, OUTPUT);//Declaring motor as OUTPUT
  pinMode(motor2B, OUTPUT);//Declaring motor as OUTPUT
  pinMode(motor3A, OUTPUT);//Declaring motor as OUTPUT
  pinMode(motor3B, OUTPUT);//Declaring motor as OUTPUT

}
void loop()
{
  //There is nothing required in the void loop as this code is for the slave which dose not have to make any decesions
}
void receiveEvent(int howMany)
{
  while (0 < Wire.available() )
  {

    char cRecievedData = Wire.read();//Assign the string recieved to cRecievedData

    Serial.print(cRecievedData);
    //if (cRecievedData == "Authorized Access")//Check if the recieved string is "Authorized Access"Authorized Access
    if (cRecievedData == 'A')
    {

      digitalWrite(motor1A, HIGH);//Running motor for opening the handle
      digitalWrite(motor1B, LOW);//Running motor for opening the handle
      delay(1000);
      digitalWrite(motor1A, LOW);//Running motor for opening the handle
      digitalWrite(motor1B, LOW);
      delay(100);
      digitalWrite(motor2A, HIGH);//Running the motor to open the door
      digitalWrite(motor2B, LOW);//Running the motor to open the door
      delay(20000);
      digitalWrite(motor2A, LOW);//Running the motor to open the door
      digitalWrite(motor2B, LOW);
      delay(10000);
      digitalWrite(motor2A, LOW);//Running the motor in the opposite direction for closing the door
      digitalWrite(motor2B, HIGH);//Running the motor in the opposite direction for closing the door
      delay(20000);
      digitalWrite(motor2A, LOW);//Running the motor to open the door
      digitalWrite(motor2B, LOW);
      delay(100);
      digitalWrite(motor1A, LOW);//Running the motor in the opposite direction for closing the door handle
      digitalWrite(motor1B, HIGH);//Running the motor in the opposite direction for closing the door handle
      delay(1000);
      digitalWrite(motor1A, LOW);//Running the motor in the opposite direction for closing the door handle
      digitalWrite(motor1B, LOW);

    }
    else if (cRecievedData == "S")//Check if the recieved string is "Santize your hand"
    {
      digitalWrite(motor3A, HIGH);//Running the pump to pump sanitizer
      digitalWrite(motor3B, LOW);//Running the pump to pump sanitizer
      delay(1000);

      digitalWrite(motor3A, LOW);//Running the pump to pump sanitizer
      digitalWrite(motor3B, LOW);
    }
  }
}
