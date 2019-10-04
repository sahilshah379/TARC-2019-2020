#include <Arduino_LPS22HB.h> // pressure
#include <Arduino_HTS221.h> // humidity & temperature

const float gravity = 9.78;
const float mass = 598.28/1000;
const int apogee = 800;
const float parachuteK = 0.405124654;

const float seaLevelPressure = 1019; // https://weather.us/observations/pressure-qnh.html
float prevAlt = 0;
float prevTime = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  if (!BARO.begin()) {
      Serial.println("Failed to initialize pressure sensor!");
      while (1);
  }
  if (!HTS.begin()) {
      Serial.println("Failed to initialize humidity temperature sensor!");
      while (1);
  }
  prevTime = millis();
}

void loop() {
  float altitude = getAltitude();
  float velocity = getVelocity(altitude);
  delay(100);
}

float getAltitude() {
  float temperature = HTS.readTemperature(); // temperature (celsius)
  float pressure = BARO.readPressure() * 10; // pressure (hPa)
  float altitude = ((pow((seaLevelPressure/pressure),(1/5.257))-1)*(temperature+273.15))/0.0065;
  return altitude;
}
float getVelocity(float altitude) {
  float velocity = 1000*(altitude-prevAlt)/((millis()-prevTime));
  prevAlt = altitude;
  prevTime = millis();
  return velocity;
}
