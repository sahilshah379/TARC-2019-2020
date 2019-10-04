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
  if (altitude + getMaxAltitude() >= apogee/3.281) {
    Serial.println("Deploying parachute");
    Serial.print("Time: ");
    Serial.println(prevTime/1000);
    Serial.print("Altitude: ");
    Serial.println(altitude);
    Serial.print("Will travel ");
    Serial.print(getMaxAltitude());
    Serial.println(" m after deployment");
    Serial.print("Apogee at ");
    Serial.println(altitude+getMaxAltitude());
    while(1) {
    }
  }
  delay(100);
}

float getAltitude() {
  float temperature = HTS.readTemperature(); // temperature (celsius)
  float pressure = BARO.readPressure() * 10; // pressure (hPa)
  float altitude = ((pow((seaLevelPressure/pressure),(1/5.257))-1)*(temperature+273.15))/0.0065;
  return altitude;
}
float getMaxAltitude() {
	float vt = sqrt(mass*gravity/parachuteK);
  yMax = (pow(vt,2)/(2*gravity))*math.log((pow(velocity,2)+pow(vt,2))/(pow(vt,2)));
	return yMax;
}
float getVelocity(float altitude) {
  float velocity = 1000*(altitude-prevAlt)/((millis()-prevTime));
  prevAlt = altitude;
  prevTime = millis();
  return velocity;
}
