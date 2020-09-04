#include <SPI.h>
#include <MFRC522.h>//Required for RFID
#include <Wire.h>//Required for analog communication between 2 nanos.
#include <LiquidCrystal_I2C.h>//Required for LCD.
#include <SoftwareSerial.h>//Required for Bluetooth.
#include <NewPing.h>
NewPing sonar(8, 7, 400);
String k = "";
String data = "";
int cm ;
SoftwareSerial ble(3, 2);
#define SS_PIN 10//Pin for RFID.
#define RST_PIN 9//Pin for RFID.
#define IRSensor 8 //Pin for IR Sensor 
#define SoundSensor 7//Pin for Sound Sensor
int mask = 0;
bool getcm();
LiquidCrystal_I2C lcd(0x3F, 16, 2); //LCD
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

void setup()
{
  lcd.begin();  //initialize the lcd
  lcd.backlight();  //open the backlight
  lcd.setCursor ( 0, 0 );            // go to the top left corner
  // write this string on the top row
  lcd.print("Present your card");
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  ble.begin(9600);
}
void loop()
{

  // Look for new cards
  //  while (mask==1){
  //
  //  }
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "71 3A DC 73") //change here the UID of the card/cards that you want to give access
  {
    lcd.clear();
    lcd.print("Authorized access");
    Serial.println();
    delay(3000);
    data = "";
    while (data == "")
    {
      data = ble.readString();
    }
    Serial.println(data);
    if (data == "Thank you for wearing a mask") {
      mask = 1;
      Serial.println("done");
      lcd.clear();
      lcd.print("Thank you! for");
      lcd.setCursor(0, 1);
      lcd.print("wearing a mask");
      delay(3000);
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Sanitize");
      lcd.setCursor(0, 1);
      lcd.print("your hands");
      cm = sonar.ping_cm();
      while (cm > 15) {
        cm = sonar.ping_cm();
        Serial.println("CM:"+cm);
      }
      if (cm < 16 )
      {
        Wire.beginTransmission(0x40);
        Wire.write("A");
        Wire.endTransmission(0x40);
        Serial.println("Detected hand");
      }
    }

    else {
      mask = 0;
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Please wear");
      lcd.setCursor(0, 1);
      lcd.print("a mask");
    }
  }


  else   {
    Serial.println(" Access denied");
    delay(3000);
  }

  delay(1000);


}
