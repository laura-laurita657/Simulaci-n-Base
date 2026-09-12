#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Configuración de la pantalla LCD (dirección 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definición de pines
const int sensorPin = A0;   // Sensor de humedad
const int bombaPin = 8;     // Motor/bomba
const int ledVerde = 5;     // LED verde (húmedo)
const int ledRojo = 6;      // LED rojo (seco)
const int ledAmarillo = 7;  // LED amarillo (regando) - NUEVO
const int buzzerPin = 9;    // Zumbador (alerta crítica) - NUEVO

// Umbrales de humedad
const int umbralHumedad = 500;   // Umbral normal
const int umbralCritico = 750;   // Umbral de sequía crítica - NUEVO

void setup() {
  // Configurar pines como salida
  pinMode(bombaPin, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(ledAmarillo, OUTPUT);   // NUEVO
  pinMode(buzzerPin, OUTPUT);     // NUEVO
  
  // Inicializar pantalla LCD
  lcd.init();
  lcd.backlight();
  
  // Iniciar comunicación serial
  Serial.begin(9600);
  
  // Mensaje inicial
  lcd.setCursor(0, 0);
  lcd.print("Riego Inteligente");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Leer el valor del sensor
  int valorHumedad = analogRead(sensorPin);
  
  // Mostrar en monitor serial
  Serial.print("Humedad: ");
  Serial.println(valorHumedad);
  
  // Mostrar en LCD
  lcd.setCursor(0, 0);
  lcd.print("Humedad: ");
  lcd.print(valorHumedad);
  lcd.print("   ");
  
  // === LÓGICA DE CONTROL ===
  
  // Caso 1: Sequía crítica (alerta máxima)
  if (valorHumedad > umbralCritico) {
    digitalWrite(bombaPin, HIGH);      // Bomba encendida
    digitalWrite(ledRojo, HIGH);       // LED rojo encendido
    digitalWrite(ledVerde, LOW);       // LED verde apagado
    digitalWrite(ledAmarillo, HIGH);   // LED amarillo encendido
    tone(buzzerPin, 1000);             // Zumbador suena
    
    lcd.setCursor(0, 1);
    lcd.print("!ALERTA! Sequia ");
  }
  // Caso 2: Tierra seca (necesita riego, pero no crítico)
  else if (valorHumedad > umbralHumedad) {
    digitalWrite(bombaPin, HIGH);      // Bomba encendida
    digitalWrite(ledRojo, HIGH);       // LED rojo encendido
    digitalWrite(ledVerde, LOW);       // LED verde apagado
    digitalWrite(ledAmarillo, HIGH);   // LED amarillo encendido
    noTone(buzzerPin);                 // Zumbador apagado
    
    lcd.setCursor(0, 1);
    lcd.print("Estado: REGANDO ");
  }
  // Caso 3: Tierra húmeda (todo OK)
  else {
    digitalWrite(bombaPin, LOW);       // Bomba apagada
    digitalWrite(ledRojo, LOW);        // LED rojo apagado
    digitalWrite(ledVerde, HIGH);      // LED verde encendido
    digitalWrite(ledAmarillo, LOW);    // LED amarillo apagado
    noTone(buzzerPin);                 // Zumbador apagado
    
    lcd.setCursor(0, 1);
    lcd.print("Estado: OK      ");
  }
  
  delay(1000);
}