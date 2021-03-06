#include "math.h"
//#include "LowPower.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

/************** Air Quality Sensor *************/
/* Air Quality Sensor Pin (ADC0) */
const int sensorAirPin = A0;
/* Ro given value for calculating the C02 ppm */
const int Ro = 1200;

/**************** GPS Sensor ******************/
/* Virtual Serial Port (UART) Baudrate  */
static const uint32_t GPSBaud = 9600;
// The TinyGPS++ object
TinyGPSPlus gps;
/* Float Variables to Store Lat & Lng data */
static float g_latitude, g_longitude;

/************* SigFox Module ******************/
const int WISOL_enable = 7;

/********** Virtual Serial Port ***************/
/* Virtual Serial Port (UART) Pins  */
static const int RXPin = 12, TXPin = 11;
// The serial connection to the GPS device
SoftwareSerial mySerial(RXPin, TXPin);

/********** General  Definitions ***************/
const int blue_button = 6;

/***** Sig Fox Messaging ******/
//*****************************************************
String bufer; // String var to store the payload to send
String bufer_n="\n"; // New line to add into our payload
//*****************************************************

void setup() 
{
  Serial.begin(9600);
  mySerial.begin(GPSBaud);
  pinMode(blue_button, INPUT);
  pinMode(WISOL_enable, OUTPUT);   //enable modulo wisol
}

void loop() 
{
  if (digitalRead(blue_button)==LOW)
  {   
    read_airSensor();
    read_GPS();
//    send_message(bufer); //enviamos nuestro dato por Sigfox
    Serial.println("\n//// Final Bufer Values \\\\");
    Serial.println(bufer);
  }
  
  
//LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, 
//        SPI_OFF, USART0_OFF, TWI_OFF);
}

void read_airSensor() {
  
  //inicializamos el payload cargando el comando para enviar
  //AT$SF= comando para mandar la informacion por sigfox Maximo 12 bytes*/  
  bufer="AT$SF="; 
  
  int adc_MQ = analogRead(sensorAirPin);      //Lemos la salida analógica  del MQ
  float voltaje = adc_MQ * (5.0 / 1023.0); //Convertimos la lectura en un valor de voltaje
  float Rs=1000*((5-voltaje)/voltaje);     //Calculamos Rs con un RL de 1k
  float CO2_ppm_float = 106.5*pow(Rs/Ro,-1.49);  // calculamos la concentración  de CO2 con la ecuación obtenida.
  int CO2_ppm_int = round(CO2_ppm_float);
  
  /**************************************************
  agregamos nuestra variable al payload a enviar*/
  add_int(CO2_ppm_int,true);
}

void read_GPS()
{
  while (mySerial.available() > 0)
  {
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


void add_float(float var1) //funcion para agregar flotantes al payload en formato little-endian
{
  byte* a1 = (byte*) &var1;    //convertimos el dato a bytes
  String str1;
  
//  Serial.println("\n---- Adding Float String to Bufer ----");
//  Serial.println((String)"OG Value = "+var1);

  //agregamos al comando AT$SF= nuestra informacion a enviar
  for(int i=0;i<4;i++)
  {
    str1=String(a1[i], HEX);    //convertimos el valor hex a string 
//    Serial.println((String)"Byte "+i+" = 0x"+str1);
    if(str1.length()<2)
    {
      bufer+=0+str1;    //si no, se agrega un cero
    }
    else
    {
      bufer+=str1;    //si esta completo, se copia tal cual
    }
  }
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
  digitalWrite(WISOL_enable, HIGH);
  delay(1000);
  //Reset del canal para asegurar que manda en la frecuencia correcta
  Serial.print("AT$RC\n"); 
  //************************
  //Enviamos la informacion por sigfox
  Serial.print(bufer);
  delay(3000);
  //deshabilitamos el modulo Sigfox
  digitalWrite(WISOL_enable, LOW);
}
