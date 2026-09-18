#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Configuración de la pantalla LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definición de pines
const int SENSOR_PIN = A0;      // Sensor de humedad
const int RED_PIN = 11;         // LED RGB - Rojo
const int GREEN_PIN = 9;        // LED RGB - Verde
const int BLUE_PIN = 10;        // LED RGB - Azul
const int BLINK_LED_PIN = 6;    // LED naranja (parpadea)

// Valores del sensor
const int SENSOR_MIN_VALUE = 0;
const int SENSOR_MAX_VALUE = 876;

// Umbrales de humedad (en porcentaje)
const int THRESHOLD_CRITICAL = 10;   // Sequía crítica
const int THRESHOLD_VERY_DRY = 25;   // Muy seco
const int THRESHOLD_DRY = 45;        // Seco
const int THRESHOLD_GOOD = 65;       // Bueno
const int THRESHOLD_OPTIMAL = 100;   // Óptimo

void setup() {
  // Configurar pines como salida
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(BLINK_LED_PIN, OUTPUT);
  
  // Iniciar comunicación serial
  Serial.begin(9600);
  
  // Inicializar LCD
  lcd.init();
  lcd.backlight();
  
  // Mensaje inicial
  lcd.setCursor(0, 0);
  lcd.print("Plant Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Leer el sensor
  int sensorValue = analogRead(SENSOR_PIN);
  
  // Convertir a porcentaje (0-100%)
  int moisturePercent = map(sensorValue, SENSOR_MIN_VALUE, SENSOR_MAX_VALUE, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);
  
  // Mostrar en monitor serial
  Serial.print("Sensor: ");
  Serial.print(sensorValue);
  Serial.print(" | Humedad: ");
  Serial.print(moisturePercent);
  Serial.println("%");
  
  // === CONTROL DEL LED RGB ===
  if (moisturePercent <= THRESHOLD_VERY_DRY) {
    // Tierra muy seca -> ROJO
    analogWrite(RED_PIN, 255);
    analogWrite(GREEN_PIN, 0);
    analogWrite(BLUE_PIN, 0);
  } else if (moisturePercent <= THRESHOLD_GOOD) {
    // Tierra media -> AZUL
    analogWrite(RED_PIN, 0);
    analogWrite(GREEN_PIN, 0);
    analogWrite(BLUE_PIN, 255);
  } else {
    // Tierra húmeda -> VERDE
    analogWrite(RED_PIN, 0);
    analogWrite(GREEN_PIN, 255);
    analogWrite(BLUE_PIN, 0);
  }
  
  // === LED NARANJA PARPADEANTE ===
  if (moisturePercent <= THRESHOLD_VERY_DRY) {
    // Parpadea más rápido si está más seco
    int blinkDelay = map(moisturePercent, THRESHOLD_CRITICAL, THRESHOLD_VERY_DRY, 100, 500);
    blinkDelay = constrain(blinkDelay, 100, 500);
    
    digitalWrite(BLINK_LED_PIN, HIGH);
    delay(blinkDelay);
    digitalWrite(BLINK_LED_PIN, LOW);
    delay(blinkDelay);
  } else {
    digitalWrite(BLINK_LED_PIN, LOW);
  }
  
  // === MOSTRAR EN LCD ===
  lcd.setCursor(0, 0);
  if (moisturePercent <= THRESHOLD_CRITICAL) {
    lcd.print("CRITICAL DRY    ");
  } else if (moisturePercent <= THRESHOLD_VERY_DRY) {
    lcd.print("Very thirsty    ");
  } else if (moisturePercent <= THRESHOLD_DRY) {
    lcd.print("Water me please ");
  } else if (moisturePercent <= THRESHOLD_GOOD) {
    lcd.print("Moisture good   ");
  } else if (moisturePercent <= THRESHOLD_OPTIMAL) {
    lcd.print("Optimal moisture");
  } else {
    lcd.print("OVERWATERED     ");
  }
  
  lcd.setCursor(0, 1);
  lcd.print(moisturePercent);
  lcd.print("% MOISTURE      ");
  
  delay(300);
}