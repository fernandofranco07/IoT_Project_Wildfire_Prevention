#include "math.h"

const int sensorPin = A0;
const int Ro = 1200;
float min_rs = 50000;
void setup() {
  Serial.begin(9600);
}

void loop() {
  int adc_MQ = analogRead(sensorPin); //Lemos la salida analógica  del MQ
  float voltaje = adc_MQ * (5.0 / 1023.0); //Convertimos la lectura en un valor de voltaje
  float Rs=1000*((5-voltaje)/voltaje);  //Calculamos Rs con un RL de 1k

  if (Rs < min_rs)
  {
    min_rs = Rs;
  }  
  double CO2_ppm = 106.5*pow(Rs/Ro,-1.49); // calculamos la concentración  de alcohol con la ecuación obtenida.
  //-------Enviamos los valores por el puerto serial------------
  Serial.print("\n");
  Serial.print("Valor ADC:");
  Serial.print(adc_MQ);
  Serial.print("\n");
  Serial.print("Voltaje:");
  Serial.print(voltaje);
  Serial.print("\n");
  Serial.print("Rs:");
  Serial.print(Rs);
  Serial.print("\n");

  Serial.print("Rs Minimo:");
  Serial.print(min_rs);
  Serial.print("\n");
  Serial.print("CO2: ");
  Serial.print(CO2_ppm);
  Serial.println("ppm");
  delay(1000);

}
