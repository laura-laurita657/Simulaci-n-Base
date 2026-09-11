#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Configuración de la pantalla LCD (dirección 0x27 según tu Tinkercad)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definición de pines
const int sensorPin = A0;   // Pin analógico del sensor de humedad
const int bombaPin = 8;     // Pin digital para controlar el motor/bomba
const int ledVerde = 5;     // LED indicador de tierra húmeda
const int ledRojo = 6;      // LED indicador de tierra seca

// Umbral de humedad (valor a calibrar)
const int umbralHumedad = 500;

void setup() {
  // Configurar pines como salida PRIMERO
  pinMode(bombaPin, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  
  // Inicializar pantalla LCD
  lcd.init();
  lcd.backlight();
  
  // Iniciar comunicación serial para depuración
  Serial.begin(9600);
  
  // Mensaje inicial en la pantalla
  lcd.setCursor(0, 0);
  lcd.print("Sistema Riego IoT");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Leer el valor del sensor de humedad
  int valorHumedad = analogRead(sensorPin);
  
  // Mostrar valor en el monitor serial
  Serial.print("Valor de humedad: ");
  Serial.println(valorHumedad);
  
  // Mostrar valor en la pantalla LCD
  lcd.setCursor(0, 0);
  lcd.print("Humedad: ");
  lcd.print(valorHumedad);
  lcd.print("   ");
  
  // Lógica de control
  if (valorHumedad > umbralHumedad) {
    // Tierra seca -> Activar bomba y LED rojo
    digitalWrite(bombaPin, HIGH);
    digitalWrite(ledRojo, HIGH);
    digitalWrite(ledVerde, LOW);
    
    lcd.setCursor(0, 1);
    lcd.print("Estado: REGANDO ");
  } else {
    // Tierra húmeda -> Apagar bomba y encender LED verde
    digitalWrite(bombaPin, LOW);
    digitalWrite(ledRojo, LOW);
    digitalWrite(ledVerde, HIGH);
    
    lcd.setCursor(0, 1);
    lcd.print("Estado: OK      ");
  }
  
  delay(1000); // Esperar 1 segundo antes de la siguiente lectura
}