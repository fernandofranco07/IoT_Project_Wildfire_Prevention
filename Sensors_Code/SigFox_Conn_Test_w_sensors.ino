#include "math.h"
#include "LowPower.h"

const int sensorPin = A0;
const int Ro = 1200;
//Programa de ejemplo para enviar informacion por medio de Sigfox
//representacion en hexadecimal de un entero (1 byte) o de un 1 flotante a 32 bits en little-endian
//Al presionar el boton azul se envia un mensaje
const int boton=6;
const int enable=7;
int randNumber;
//*****************************************************
String bufer; //variable donde guardaremos nuestro payload
String bufer2="\n";   //agregamos un salto de linea al final de nuestro payload
//*****************************************************

void setup() 
{
  Serial.begin(9600);
  pinMode(boton, INPUT);
  pinMode(enable, OUTPUT);   //enable modulo wisol
}

void loop() 
{
	if (digitalRead(boton)==LOW)
	{
		read_airSensor();
	}
	
//LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, 
//			  SPI_OFF, USART0_OFF, TWI_OFF);
}

void read_airSensor() {
  
  //inicializamos el payload cargando el comando para enviar
  //AT$SF= comando para mandar la informacion por sigfox Maximo 12 bytes*/  
  bufer="AT$SF="; 
  
  int adc_MQ = analogRead(sensorPin);      //Lemos la salida analógica  del MQ
  float voltaje = adc_MQ * (5.0 / 1023.0); //Convertimos la lectura en un valor de voltaje
  float Rs=1000*((5-voltaje)/voltaje);     //Calculamos Rs con un RL de 1k
  float CO2_ppm = 106.5*pow(Rs/Ro,-1.49);  // calculamos la concentración  de CO2 con la ecuación obtenida.

  /**************************************************
  agregamos nuestra variable al payload a enviar*/
  add_float(CO2_ppm);
  
  //enviamos nuestro dato por Sigfox
  send_message(bufer);
}

void add_float(float var1) //funcion para agregar flotantes al payload en formato little-endian
{
  byte* a1 = (byte*) &var1;    //convertimos el dato a bytes
  String str1;
  //agregamos al comando AT$SF= nuestra informacion a enviar
  for(int i=0;i<4;i++)
  {
    str1=String(a1[i], HEX);    //convertimos el valor hex a string 
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

void send_message(String payload)
{
  //agregamos el salto de linea "\n"
  bufer+=bufer2;
  //*******************
  //Habilitamos el modulo Sigfox
  digitalWrite(enable, HIGH);
  delay(1000);
  //Reset del canal para asegurar que manda en la frecuencia correcta
  Serial.print("AT$RC\n"); 
  //************************
  //Enviamos la informacion por sigfox
  Serial.print(bufer);
  delay(3000);
  //deshabilitamos el modulo Sigfox
  digitalWrite(enable, LOW);
}
