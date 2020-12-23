    /////////////////////////////////////////////////////////////////
   //             Easy Arduino Button             v1.02           //
  //       Get the latest version of the code here:              //
 //           http://educ8s.tv/arduino-gps-tracker              //
/////////////////////////////////////////////////////////////////

#include <SD.h>
#include <SPI.h>
#include <SoftwareSerial.h> 
#include "TinyGPS++.h"

int ledPin = 9;
int programStatus = 0;

File file;
String longtitude;
String latitude;
String dayString;
String monthString;
int id = 0;

TinyGPSPlus gps;
SoftwareSerial SoftSerial(2, 3); 

void setup()
{
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  SoftSerial.begin(9600);
  Serial.begin(9600);
  programStatus = initializeSD();
  delay(3000); 
  openFileToWrite("log.csv");
  writeHeaderToFile();
}

void loop()
{
  String entry;
  String hourString;  
  String minuteString;
  String secondsString;
  while (SoftSerial.available() > 0)
  gps.encode(SoftSerial.read());
  
    if (gps.location.isUpdated())
    {
      programStatus = 2;       
      blinkQuickly();
      latitude = String(gps.location.lat(),5);
      longtitude = String(gps.location.lng(),5);
      dayString = String(gps.date.day());
      monthString = String(gps.date.month());
      hourString = String(gps.time.hour());
      minuteString = String(gps.time.minute());
      if(gps.time.hour()<10)
      {
        hourString = "0"+String(gps.time.hour());
      }
      else
      {
       hourString = String(gps.time.hour());
      }
      
      if(gps.time.second()<10)
      {
        secondsString = "0"+String(gps.time.second());
      }
      else
      {
       secondsString = String(gps.time.second());
      }
      
      id++;
      entry = String(id)+","+dayString+"/"+monthString+","+hourString+":"+minuteString+":"+secondsString+","+latitude+","+longtitude;
      writeEntryToFile(entry);
      delay(5000);
  }
  else if(programStatus == 1)
  {
    digitalWrite(ledPin, HIGH);
    delay(300);
    digitalWrite(ledPin,LOW);
    delay(300);
  }
}
int initializeSD()
{
  Serial.println("Initializing SD card...");
  pinMode(10, OUTPUT);

  if (SD.begin(4))
  {
    Serial.println("SD card is ready to use.");
    return 1;
  } else
  {
    Serial.println("SD card initialization failed");
    return 0;
  }
}

int openFileToWrite(char filename[])
{
  file = SD.open(filename, FILE_WRITE);

  if (file)
  {
    Serial.println("opened file");

    return 1;
  } else
  {
    return 0;
  }
}

int writeToFile(String text)
{
  if (file)
  {
    file.println(text);
    return 1;
  } else
  {
    return 0;
  }
}

void closeFile()
{
  if (file)
  {
    file.close();
  }
}
void writeEntryToFile(String entry)
{
  openFileToWrite("log.txt");
  writeToFile(entry);
  Serial.println(entry);
  file.close();

}

void blinkQuickly()
{
   digitalWrite(ledPin, HIGH);
   delay(100);
   digitalWrite(ledPin,LOW);
   delay(100);
}

void writeHeaderToFile()
{
  Serial.println("writing file");
  writeEntryToFile("Id,Date,Time,Latitude,Longitude");
}
