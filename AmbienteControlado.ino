#include "DHT.h"
#include <math.h> // Necesario para usar floorf()

#define DHTPIN 15       // Pin del DHT11 en GPIO15
#define DHTTYPE DHT11   // Tipo de sensor DHT11

const int SOIL_MOISTURE_PIN = 13;  // Pin del sensor de humedad de suelo en GPIO13

#define RELAY_FOCO_PIN  26         // Rele para el foco (controla temperatura)
#define RELAY_HUMIDIFIER_PIN 27    // Rele para el humidificador (controla humedad)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("Control independiente de humedad y temperatura"));

  dht.begin();
  pinMode(SOIL_MOISTURE_PIN, INPUT);

  pinMode(RELAY_FOCO_PIN, OUTPUT);
  pinMode(RELAY_HUMIDIFIER_PIN, OUTPUT);

  // Inicialmente apagar actuadores
  digitalWrite(RELAY_FOCO_PIN, LOW);          
  digitalWrite(RELAY_HUMIDIFIER_PIN, LOW);
}

void loop() {
  delay(5000);  // Intervalo entre lecturas

  // ---------------- Leer sensores ----------------
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float rawValue = analogRead(SOIL_MOISTURE_PIN);
  float moisturePercent = ((4095.0 - rawValue) * 100.0) / 4095.0;

  // Truncar valores a 2 decimales sin redondear usando floats
  h = floorf(h * 100) / 100.0;
  t = floorf(t * 100) / 100.0;
  

  // ---------------- Control actuador humidificador ----------------
  if (h < 83.0) {
    digitalWrite(RELAY_HUMIDIFIER_PIN, HIGH);     // Encender humidificador
  } else {
    digitalWrite(RELAY_HUMIDIFIER_PIN, LOW);      // Apagar humidificador
  }

  // ---------------- Control actuador foco ----------------
  if (t < 25.0) {
    digitalWrite(RELAY_FOCO_PIN, HIGH);           // Encender foco
  } else {
    digitalWrite(RELAY_FOCO_PIN, LOW);            // Apagar foco
  }

  // ---------------- Imprimir ----------------
  Serial.print(F("Humedad ambiente: "));
  Serial.print(h, 2);
  Serial.print(F("% | Temp: "));
  Serial.print(t, 2);
  Serial.print(F("°C | Humedad suelo: "));
  Serial.print(moisturePercent, 2);
  Serial.println(F("%"));
}
