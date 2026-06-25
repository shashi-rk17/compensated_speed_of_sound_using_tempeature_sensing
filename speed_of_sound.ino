#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Ultrasonic pins
#define TRIG 6
#define ECHO 7

// Measurement LED
#define MEASURE_LED 3

// LM35 analog pin
#define LM35_PIN A0

// Fixed distance = 17 cm = 0.17 m
float d = 0.17;

void setup() {

  Serial.begin(9600);

  // Ultrasonic
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // LED
  pinMode(MEASURE_LED, OUTPUT);

  // LCD
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("System Ready");

  delay(2000);
  lcd.clear();
}

void loop() {

  // Read LM35 temperature
  int sensorValue = analogRead(LM35_PIN);

  float voltage = sensorValue * (5.0 / 1023.0);

  // LM35: 10mV per degree C
  float temp = voltage * 100.0;

  // Measurement LED ON
  digitalWrite(MEASURE_LED, HIGH);

  // Trigger ultrasonic pulse
  digitalWrite(TRIG, LOW);
  delayMicroseconds(4);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG, LOW);

  // Read echo duration
  long duration = pulseIn(ECHO, HIGH, 30000);

  // Measurement LED OFF
  digitalWrite(MEASURE_LED, LOW);

  // No echo detected
  if (duration == 0) {

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("No Echo");

    lcd.setCursor(0, 1);
    lcd.print("Check Sensor");

    Serial.println("No echo received");

    delay(1000);
    return;
  }

  // Convert microseconds to seconds
  float delta_t = duration / 1000000.0;

  // Measured speed of sound
  float v_measured = (2 * d) / delta_t;

  // Theoretical speed of sound
  float v_theory = 331.0 + (0.6 * temp);

  // LCD Display
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Vm:");
  lcd.print(v_measured, 0);

  lcd.setCursor(9, 0);
  lcd.print("Vt:");
  lcd.print(v_theory, 0);

  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(temp, 1);
  lcd.print((char)223); // degree symbol
  lcd.print("C");

  // Serial Monitor output
  Serial.print("Temp: ");
  Serial.print(temp);

  Serial.print(" C | Vm: ");
  Serial.print(v_measured);

  Serial.print(" | Vt: ");
  Serial.println(v_theory);

  delay(1000);
}
