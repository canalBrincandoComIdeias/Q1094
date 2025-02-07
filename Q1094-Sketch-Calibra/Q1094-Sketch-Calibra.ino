#include "HX711.h"

//Pinos do módulo HX711
#define pinCELULA_DOUT 2
#define pinCELULA_SCK 4

HX711 balanca;

void setup() {
  Serial.begin(9600);
  balanca.begin(pinCELULA_DOUT, pinCELULA_SCK);
}

void loop() {

  if (balanca.is_ready()) {

    balanca.set_scale();
    Serial.println("Tara... retire qualquer peso da balanca.");
    for (int nL=0; nL < 5; nL++) {
        Serial.print(".");
        delay(1000); 
    }

    balanca.tare();
    Serial.println("Tara calculada...");
    Serial.println("Coloque um peso conhecido na balanca...");
    for (int nL=0; nL < 5; nL++) {
        Serial.print(".");
        delay(1000); 
    }

    long leitura = balanca.get_units(10);
    Serial.print("Resultado - Leitura de Calibracao: ");
    Serial.println(leitura);
    delay(5000);
    for (int nL=0; nL < 5; nL++) {
        Serial.print(".");
        delay(1000); 
    }
  } else {
    Serial.println("HX711 nao encontrado.");
  }
  delay(1000);
}
