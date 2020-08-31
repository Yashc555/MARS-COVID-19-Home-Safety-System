#include <SPI.h>//Required for RFID
#include <MFRC522.h>//Required for RFID
#include <Wire.h>//Required for analog communication between 2 nanos.
#include <LiquidCrystal_I2C.h>//Required for LCD.
#include <SoftwareSerial.h>//Required for Bluetooth.
long duration, cm;
String data = "";
SoftwareSerial ble(3, 2);//Initialised RX and TX pins of HM-10
#define SS_PIN 10//Pin declared for RFID.
#define RST_PIN 9//Pin declared for RFID.
#define trigPin 8 //Pin declared for Ultrasonic sensor 
#define echoPin 7//Pin declared for Ultrasonic sensor 
int mask = 0;//Used to store the value of mask Detection 
LiquidCrystal_I2C lcd(0x3F, 16, 2); //LCD address intialised
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
bool getID();//Initialised Function for gettting the id of the card placed to the reader
void setup()
{
Wire.begin(0x40);//Initialised the communication between the two nanos
lcd.begin();  //initialize the lcd
lcd.backlight();//open the backlight
lcd.setCursor ( 0, 0 ); //resets the cursor
  
lcd.print("SAY-'Detect Mask'");//prints SAY-'Detect Mask so that the user says detect msk which is a button on app which will detect the mask
mfrc522.PCD_Init();// Initialised MFRC522            
SPI.begin();// Initialised SPI
pinMode(echoPin,INPUT);//Used for transmitting Radio waves
pinMode(trigPin,OUTPUT);//Used for detection of Radio waves
ble.begin(9600);// begin bluetooth serial port communication
}
/*********************************************************/
void loop()
{
  if ( data != "") //If data is blank it will read the ble again until it is blank
  {
    lcd.clear();//To reset the LCD
    lcd.setCursor ( 0, 0 );            

    if (data == "Thank you for wearing a mask") //If the data is equal to Thank you for wearing a mask that means the person has worn a mask
    {
      mask = 1;
      lcd.clear();
      lcd.print("Thank you! for");
      lcd.setCursor(0, 1);
      lcd.print("wearing a mask");


      data = "";//data is now reseted

      delay(3000);
      


      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("put your card");// intructing the user to the to scan the rfid to the reader
      lcd.setCursor(0, 1);
      lcd.print("to the reader");

      


      while (!getID())
      {}


      lcd.clear();
      lcd.print("Card Reading");//This line is there incase the RFID takes time to read the card 
      String content = "";//as the Rfid gives data in bytes we concat it into a string for that the content variable is used
      byte letter;
      for (byte i = 0; i < mfrc522.uid.size; i++)
      {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
      }

      content.toUpperCase();
      if (content.substring(1) == "71 3A DC 73") //This is the address of the card that we are using

      {
        if (mask == 1) //This is if is used to recheck that that the person has worn a mask
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Authorized");//here it prints that the the uid is correct and the person is Authorized 
          lcd.setCursor(0, 1);
          lcd.print("access");
          
          
          
          mask = 0;//the mask variable resets here for the next run
          delay(3000);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Sanitze your");// here it asks the user to sanitize hands before entering the place
          lcd.setCursor(0, 1);
          lcd.print("hands");
          //here the distance is measured and if the distance is less than equal to 20 the it will dispense sanitizer
          pinMode(trigPin, OUTPUT);
          while(true){
            digitalWrite(trigPin, LOW);
          delayMicroseconds(2);
          digitalWrite(trigPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin, LOW);
        
          pinMode(echoPin, INPUT);
          duration = pulseIn(echoPin, HIGH);
          cm = microsecondsToCentimeters(duration);//This function is made by us to measure the disance 
        
        
          
          delay(100);
          Serial.print(cm);
        
          if (cm >= 20) //Here the comparision is done
          { 
            Wire.beginTransmission(0x40);
            Wire.write("A");//Here the data is Transmitted to the 
            Wire.endTransmission(0x40);
            lcd.clear();
            lcd.print("Please Enter");
            
          }
          
          }
         
        }

      }
      else {
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Acess Denied");

      }
      //      Wire.beginTransmission(0x40);
      //      Wire.write("Authorized Access");
      //      Wire.endTransmission(0x40);


    }
    else {
      mask = 0;
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Please wear");
      lcd.setCursor(0, 1);
      lcd.print("a mask");
      data = "";
    }






    //    int n = data.length();
    //    Serial.print(n);
    //    int i = 0;
    //    char char_array[n + 1];
    //    strcpy(char_array, data.c_str());
    //    for (i = 0; i < n; i++) {
    //      ble.write(char_array[i]);
    //    }
  }

  else {
    data = ble.readString();


  }
}
bool getID()
{
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return false;
  }
  
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return false;
    //mask = 0;
  }

  return true;

}
long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
