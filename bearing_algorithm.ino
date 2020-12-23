#include <math.h>
#include <SPI.h>
#include <SoftwareSerial.h> 
#include "TinyGPS++.h"

TinyGPSPlus gps;
SoftwareSerial SoftSerial(2, 3); 

double lat1;
double lat2;
double lon1;
double lon2;
double latR1;
double latR2;
double lonR1;
double lonR2;
double dlon;
double dlat;
double a;
double e;
double d;
double R = 6371.00;
double toDegrees = 57.295779;
char sb[10];
/*-----------------------------Setup---------------------------------*/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SoftSerial.begin(9600);
  //Get Destination GPS Coordinates
  Serial.println("\nInput Destination Latitude: ");
  while(Serial.available() == 0){}
  Serial.readBytes(sb, 10);
  lat2 = atof(sb);
  Serial.println("Input Destination Longitude: \n");
  while(Serial.available() == 0){}
  Serial.readBytes(sb, 10);
  lon2 = atof(sb);
  String Dest = "GPS Destination Location: " + String(lat2, 6) + ", " + String(lon2, 6);
  Serial.print(Dest);
  Serial.println();

}


/*-----------------------------Main Loop---------------------------------*/
void loop() {
  //Get Starting GPS Coordinates
    while (SoftSerial.available() > 0)
  gps.encode(SoftSerial.read());
    if (gps.location.isUpdated()){
      // put your main code here, to run repeatedly:
      lat1 = (String(gps.location.lat(),5)).toFloat();
      lon1 = (String(gps.location.lng(),5)).toFloat();

      String Start = "GPS Starting Location: " + String(lat1, 6) + ", " + String(lon1, 6);
      Serial.print(Start);
    

    
      calcDist(); //Call the distance and bearing calculation function
      
      delay(5000);
    }
}

/*-----------------------------Distance & Bearing Calculator---------------------------------*/
void calcDist(){ //This is a haversine based distance calculation formula
  //This portion converts the current and destination GPS coords from decDegrees to Radians
  lonR1 = lon1*(PI/180);
  lonR2 = lon2*(PI/180);
  latR1 = lat1*(PI/180);
  latR2 = lat2*(PI/180);
 
  //This portion calculates the differences for the Radian latitudes and longitudes and saves them to variables
  dlon = lonR2 - lonR1;
  dlat = latR2 - latR1;
 
  //This portion is the Haversine Formula for distance between two points. Returned value is in KM
  a = (sq(sin(dlat/2))) + cos(latR1) * cos(latR2) * (sq(sin(dlon/2)));
  e = 2 * atan2(sqrt(a), sqrt(1-a)) ;
  d = R * e;

  Serial.println();
  Serial.print("Distance to destination(KM): ");
  //Serial.println(a);
  //Serial.println(e);
  Serial.println(d, 6);
  Serial.println();
  if (d < 1/10){
    Serial.println("Destination Reached");
  }
 
  //This portion is the Haversine Formula for required bearing between current location and destination. Returned value is in Degrees
  double x = cos(latR2)*sin(lonR2-lonR1); //calculate x
 
  Serial.print("X = ");
  Serial.println(x, 6);

  double y = cos(latR1)*sin(latR2)-sin(latR1)*cos(latR2)*cos(lonR2-lonR1); //calculate y

  Serial.print("Y = ");
  Serial.println(y, 6);
  float brRad = atan2(x, y); //return atan2 result for bearing. Result at this point is in Radians

  Serial.print("atan2(x, y) (Radians) = ");
  Serial.println(brRad, 6);

  float reqBear = toDegrees*brRad;
  Serial.print("Bearing: ");
  Serial.println(reqBear, 4);
}
