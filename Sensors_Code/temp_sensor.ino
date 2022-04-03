//Programa de ejemplo utilizando el sensor de temperatura LM35
//En este caso el dato en entero es transformado a su 
//representacion en hexadecimal en 1 flotante a 32 bits en little-endian

#include <math.h>

const int boton=6;
const int sensorPin = A1;
//*****************************************************
String bufer; //variable donde guardaremos nuestro payload
String bufer2="\n";   //agregamos un salto de linea al final de nuestro payload
//*****************************************************
void setup() 
{
  Serial.begin(9600);
  pinMode(boton, INPUT);
  pinMode(7, OUTPUT);   //enable modulo wisol
}

void leer_temperatura()
{
  int sensorVal=analogRead(sensorPin);
  Serial.print("Grados Cº: ");
  float temp=(5.0*sensorVal*100.0)/1024.0;
  Serial.println(temp);
  
  //-----------------------------------------------------
  //AT$SF= comando para mandar la informacion por sigfox
  //Maximo 12 bytes
  bufer="AT$SF=";
  //-----------------------------------------------------
  //agregamos nuestro valor de la temperatura al payload a enviar
  int temp_aux = round(temp);
  
  add_int(temp_aux,false);
  
  Serial.println(temp_aux);
  Serial.println(bufer);
  //enviamos nuestro dato por Sigfox
  send_message(bufer);
}

void loop() 
{
  if (digitalRead(boton)==LOW)
  {
    leer_temperatura();
    delay(1000);
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
      Serial.println((String)"Byte 0 = 0x"+str2);
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
  bufer+=bufer2;
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
