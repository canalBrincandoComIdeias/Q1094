#include <LiquidCrystal_I2C.h>
#include "HX711.h"
#include <EEPROM.h>

//Pinos do módulo HX711
#define pinCELULA_DOUT 2
#define pinCELULA_SCK 4

//Pinos dos Botoes
#define pinBotCalib 12
#define pinBotTara 13

#define pesoCalibra 200
#define enderecoCalibra 49

LiquidCrystal_I2C lcd(0x25, 16, 2);  //0x3F //0x25
HX711 balanca;

double leitura;
char texto[9];
bool limpaDisplay = false;
float fatorCalibra = 1;  //349.62;
unsigned long delayTara;

void setup() {
  pinMode(pinBotCalib, INPUT);
  pinMode(pinBotTara, INPUT);

  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Iniciando...");

  Serial.begin(9600);
  Serial.println("Iniciando a balanca");

#ifdef ARDUINO_ARCH_ESP32
  if (!EEPROM.begin(512)) {
    Serial.println("Erro ao iniciar EEPROM");
    Serial.println("Reiniciando...");
    delay(1000);
    ESP.restart();
  }
#endif

  //Leitura da calibracao armazenada na EEPROM
  EEPROM.get(enderecoCalibra, fatorCalibra);

  Serial.print("Fator:\t");
  Serial.print(fatorCalibra);

  balanca.begin(pinCELULA_DOUT, pinCELULA_SCK);
  balanca.set_scale(fatorCalibra);
  balanca.tare();  // zera a balanca
  Serial.println("Tara e calibracao configurados");

  lcd.clear();
  lcd.print("Balanca Pronta!");
  delay(1000);
  lcd.clear();
  lcd.setCursor(13, 0);
  lcd.print("GR.");
}

void loop() {

  leitura = balanca.get_units(5);
  delay(20);
  //Serial.print("Leitura:\t");
  //Serial.println(leitura, 2);

  if (leitura < 0) leitura = 0;

  dtostrf(leitura, 12, 0, texto);
  lcd.setCursor(0, 0);
  lcd.print(texto);

  //Comando Tara
  if (digitalRead(pinBotTara)) {
    balanca.tare();
    lcd.setCursor(12, 1);
    lcd.print("tara");
    delayTara = millis();
    limpaDisplay = true;
  }
  if (limpaDisplay) {
    if ((millis() - delayTara) > 1000) {
      lcd.setCursor(12, 1);
      lcd.print("    ");
      limpaDisplay = false;
    }
  }

  //Comando Calibra
  if (digitalRead(pinBotCalib)) {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Retire qualquer");
    lcd.setCursor(3, 1);
    lcd.print("peso");
    for (int nL = 0; nL < 5; nL++) {
      lcd.print(".");
      delay(1000);
    }
    balanca.set_scale();
    delay(5000);
    balanca.tare();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Coloque ");
    lcd.print(pesoCalibra);
    lcd.print(" GR.");

    lcd.setCursor(5, 1);
    for (int nL = 0; nL < 5; nL++) {
      lcd.print(".");
      delay(1000);
    }
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Calibrando...");

    leitura = balanca.get_units(200);
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Balanca");
    lcd.setCursor(3, 1);
    lcd.print("Calibrada!");

    fatorCalibra = leitura / pesoCalibra;
    EEPROM.put(enderecoCalibra, fatorCalibra);
#ifdef ARDUINO_ARCH_ESP32
    EEPROM.commit();  //Para ESP (comentar para Arduino)
#endif

    balanca.set_scale(fatorCalibra);
    delay(1000);
    lcd.clear();
    lcd.setCursor(13, 0);
    lcd.print("GR.");
  }
}
