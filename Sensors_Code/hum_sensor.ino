// Example testing sketch for various DHT humidity/temperature sensors

#include "DHT.h"
#include <math.h>
#define DHTPIN A2 // what digital pin we're connected to
// Uncomment whatever type you're using!
//#define DHTTYPE DHT11 // DHT 11
#define DHTTYPE DHT11 // DHT 11 (AM2302), AM2321
// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors. This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

/***** Sig Fox Messaging ******/
//*****************************************************
String bufer; // String var to store the payload to send
String bufer_n="\n"; // New line to add into our payload
//*****************************************************

void setup() {
   Serial.begin(9600);
   Serial.println("DHT11 test!");
   dht.begin();
}

void read_send_hum (){
   float h = dht.readHumidity();
   
   // Check if any reads failed and exit early (to try again)   
   if (isnan(h)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
   }
  
   bufer="AT$SF=";
   Serial.print ("Humidity (%): ");
   Serial.print (h);
   Serial.print("\n");
   
   int hum_aux = round(h);
   add_int(hum_aux, false);

   send_message(bufer);
}

void loop() {
   delay(2000); // Wait a few seconds between measurements
   read_send_hum();
}

void add_int(int var2, bool is_Air_sensor)    //funcion para agregar enteros al payload (hasta 255)
{
  byte* a2 = (byte*) &var2; //convertimos el dato a bytes
  String str2;

//  Serial.println("\n---- Adding Int String to Bufer ----");
//  Serial.println((String)"OG Value = "+var2);
  if (is_Air_sensor)
  {
     for(int i=0;i<2;i++)
     {
        str2=String(a2[i], HEX);    //convertimos el valor hex a string 
//        Serial.println((String)"Byte "+i+" = 0x"+str2);
        if(str2.length()<2)
        {
          bufer+=0+str2;    //si no, se agrega un cero
        }
        else
        {
          bufer+=str2;    //si esta completo, se copia tal cual
        }
     }
  }
  else
  {
      str2=String(a2[0], HEX);  //convertimos el valor hex a string 
//     Serial.println((String)"Byte 0 = 0x"+str2);
      //verificamos si nuestro byte esta completo
      if(str2.length()<2)
      {
        bufer+=0+str2;    //si no, se agrega un cero
      }
      else
      {
        bufer+=str2;     //si esta completo, se copia tal cual
      }      
  }  
}


void send_message(String payload)
{
  //agregamos el salto de linea "\n"
  bufer+=bufer_n;
  //*******************
  //Habilitamos el modulo Sigfox
  digitalWrite(7, HIGH);
  delay(1000);
  //Reset del canal para asegurar que manda en la frecuencia correcta
  Serial.print("AT$RC\n"); 
  //************************
  //Enviamos la informacion por sigfox
  Serial.print(bufer);
  delay(3000);
  //deshabilitamos el modulo Sigfox
  digitalWrite(7, LOW);
}
