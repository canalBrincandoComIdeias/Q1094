#include "HX711.h"

//Pinos do módulo HX711
#define pinCELULA_DOUT 2
#define pinCELULA_SCK 4

HX711 balanca;

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando a balanca");

  balanca.begin(pinCELULA_DOUT, pinCELULA_SCK);

  Serial.println("Teste inicial dos comandos:");
  Serial.print("read: \t\t");
  Serial.println(balanca.read());  //mostra uma leitura direta do ADC (conversor analógico digital)

  Serial.print("read_average(20): \t\t");
  Serial.println(balanca.read_average(20));  //mostra a media das 20 leituras do ADC

  Serial.print("get_value(5): \t\t");
  Serial.println(balanca.get_value(5));  //mostra a media das 5 leituras do ADC menos o peso da tara (ainda nao configurada)

  Serial.print("get units(5): \t\t");
  Serial.println(balanca.get_units(5), 1);  //mostra a media das 5 leituras do ADC menos o peso da tara (ainda nao configurada) dividido
                                            //pelo indice de calibracao (ainda nao configurado)

  //COLOQUE AQUI O FATOR DE CALIBRACAO CALCULADO  (FATOR DE CALIBRACAO = LEITURA DE CALIBRACAO / PESO CONHECIDO NA CALIBRACAO)
  balanca.set_scale(110.63);   //22127 / 200 = 110.63

  balanca.tare();  // zera a balanca
  Serial.println("Apos configuracao da calibracao e tara:");

  Serial.print("read: \t\t");
  Serial.println(balanca.read());  //mostra uma leitura direta do ADC

  Serial.print("read_average(20): \t\t");
  Serial.println(balanca.read_average(20));  //mostra a media das 20 leituras do ADC

  Serial.print("get value: \t\t");
  Serial.println(balanca.get_value(5));  //mostra a media das 5 leituras do ADC menos o peso da tara

  Serial.print("get units: \t\t");
  Serial.println(balanca.get_units(5), 1);  //mostra a media das 5 leituras do ADC menos o peso da tara, dividido
                                            //pelo indice de calibracao

  Serial.println("Leituras:");
}

void loop() {
  Serial.print("uma leitura:\t");
  Serial.print(balanca.get_units(), 1);

  Serial.print("\t| media:\t");
  Serial.println(balanca.get_units(10), 5);

  delay(5000);
}
