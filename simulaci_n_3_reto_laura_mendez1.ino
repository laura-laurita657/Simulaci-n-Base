#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pines del sensor y LEDs
const int SENSOR_PIN = A0;
const int RED_PIN = 11;
const int GREEN_PIN = 9;
const int BLUE_PIN = 10;
const int BLINK_LED_PIN = 6;

// Pines nuevos del reto
const int BUZZER_PIN = 8;   // Zumbador
const int MOTOR_PIN = 7;    // Motor (bomba)

// Valores del sensor
const int SENSOR_MIN_VALUE = 0;
const int SENSOR_MAX_VALUE = 876;

// Umbrales de humedad (%)
const int THRESHOLD_CRITICAL = 10;
const int THRESHOLD_VERY_DRY = 25;
const int THRESHOLD_DRY = 45;
const int THRESHOLD_GOOD = 65;
const int THRESHOLD_OPTIMAL = 100;

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(BLINK_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Riego Inteligente");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");
  delay(2000);
  lcd.clear();
}

void loop() {
  int sensorValue = analogRead(SENSOR_PIN);
  int moisturePercent = map(sensorValue, SENSOR_MIN_VALUE, SENSOR_MAX_VALUE, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);

  Serial.print("Sensor: ");
  Serial.print(sensorValue);
  Serial.print(" | Humedad: ");
  Serial.print(moisturePercent);
  Serial.println("%");

  // === LED RGB ===
  if (moisturePercent <= THRESHOLD_VERY_DRY) {
    analogWrite(RED_PIN, 255);
    analogWrite(GREEN_PIN, 0);
    analogWrite(BLUE_PIN, 0);
  } else if (moisturePercent <= THRESHOLD_GOOD) {
    analogWrite(RED_PIN, 0);
    analogWrite(GREEN_PIN, 0);
    analogWrite(BLUE_PIN, 255);
  } else {
    analogWrite(RED_PIN, 0);
    analogWrite(GREEN_PIN, 255);
    analogWrite(BLUE_PIN, 0);
  }

  // === LED NARANJA PARPADEANTE ===
  if (moisturePercent <= THRESHOLD_VERY_DRY) {
    int blinkDelay = map(moisturePercent, THRESHOLD_CRITICAL, THRESHOLD_VERY_DRY, 100, 500);
    blinkDelay = constrain(blinkDelay, 100, 500);
    digitalWrite(BLINK_LED_PIN, HIGH);
    delay(blinkDelay);
    digitalWrite(BLINK_LED_PIN, LOW);
    delay(blinkDelay);
  } else {
    digitalWrite(BLINK_LED_PIN, LOW);
  }

  // === ZUMBADOR EN SEQUÍA CRÍTICA ===
  if (moisturePercent <= THRESHOLD_CRITICAL) {
    tone(BUZZER_PIN, 1000);
  } else {
    noTone(BUZZER_PIN);
  }

  // === MOTOR (BOMBA) CUANDO NECESITA RIEGO ===
  if (moisturePercent <= THRESHOLD_VERY_DRY) {
    digitalWrite(MOTOR_PIN, HIGH);
  } else {
    digitalWrite(MOTOR_PIN, LOW);
  }

  // === LCD ===
  lcd.setCursor(0, 0);
  if (moisturePercent <= THRESHOLD_CRITICAL) {
    lcd.print("CRITICAL DRY    ");
  } else if (moisturePercent <= THRESHOLD_VERY_DRY) {
    lcd.print("Very thirsty    ");
  } else if (moisturePercent <= THRESHOLD_DRY) {
    lcd.print("Water me please ");
  } else if (moisturePercent <= THRESHOLD_GOOD) {
    lcd.print("Moisture good   ");
  } else {
    lcd.print("Optimal moisture");
  }

  lcd.setCursor(0, 1);
  lcd.print(moisturePercent);
  lcd.print("% MOISTURE      ");

  delay(300);
}