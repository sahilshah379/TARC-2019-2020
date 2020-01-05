#include <Arduino_LPS22HB.h> // pressure
#include <Arduino_HTS221.h> // humidity & temperature

const float seaLevelPressure = 1019; // https://weather.us/observations/pressure-qnh.html
float prevAlt = 0;
double prevTime = 0;

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
  float temperature = HTS.readTemperature(); // temperature (celsius)
  float pressure = BARO.readPressure() * 10; // pressure (hPa)
  float altitude = ((pow((seaLevelPressure/pressure),(1/5.257))-1)*(temperature+273.15))/0.0065;
  Serial.print("Altitude: ");
  Serial.println(altitude);
  float velocity = 1000*(altitude-prevAlt)/((millis()-prevTime));
  Serial.print("Velocity: ");
  Serial.println(velocity);
  prevAlt = altitude;
  prevTime = millis();
  Serial.println();
  delay(100);
}
