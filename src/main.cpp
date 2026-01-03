// FINÁLNÍ DIAGNOSTIKA - PWM test
#include <ESP32Servo.h>

Servo myServo;
const int servoPin = 18;
const int ledPin = 2;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n=================================");
  Serial.println("   FINÁLNÍ SERVO DIAGNOSTIKA");
  Serial.println("=================================\n");
  
  pinMode(ledPin, OUTPUT);
  
  // Test připojení
  Serial.println("1. Připojuji servo knihovnou ESP32Servo...");
  myServo.attach(servoPin, 544, 2400);
  delay(500);
  Serial.println("   ✓ Servo attached na GPIO 18");
  
  // Test pozic s detailním výpisem
  Serial.println("\n2. Test pozic:");
  Serial.println("   Pokud vidíte tento text, ESP32 FUNGUJE!");
  Serial.println("   Pokud se servo NEHÝBE:");
  Serial.println("   - Zkontrolujte červený kabel → VIN pin (ne 3V3!)");
  Serial.println("   - Zkontrolujte, že všechny kabely jsou pevně zapojené");
  Serial.println("   - Servo může být vadné\n");
  
  Serial.println("--- Začínám test pohybu ---\n");
}

void loop() {
  // Test 0°
  digitalWrite(ledPin, HIGH);
  Serial.println(">>> Posílám příkaz: 0° (minimum)");
  Serial.println("    Servo by se mělo otočit VLEVO");
  myServo.write(0);
  delay(3000);
  digitalWrite(ledPin, LOW);
  
  // Test 90°
  Serial.println(">>> Posílám příkaz: 90° (střed)");
  Serial.println("    Servo by mělo být UPROSTŘED");
  myServo.write(90);
  delay(3000);
  
  // Test 180°
  digitalWrite(ledPin, HIGH);
  Serial.println(">>> Posílám příkaz: 180° (maximum)");
  Serial.println("    Servo by se mělo otočit VPRAVO");
  myServo.write(180);
  delay(3000);
  digitalWrite(ledPin, LOW);
  
  Serial.println("\n--- Cyklus dokončen, opakuji ---\n");
  delay(1000);
}
