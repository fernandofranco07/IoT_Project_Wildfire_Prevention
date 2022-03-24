#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 12, TXPin = 11;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial mySerial(RXPin, TXPin);

void setup(){
  Serial.begin(9600);
  mySerial.begin(GPSBaud);
}

void loop(){
  // This sketch displays information every time a new sentence is correctly encoded.
  while (mySerial.available() > 0){
    gps.encode(mySerial.read());
    if (gps.location.isUpdated()){
      Serial.print("\n");
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 6);
      Serial.print("\n");
      Serial.print("Longitude= "); 
      Serial.print(gps.location.lng(), 6);
      Serial.print("\n");
    }
  }
}
