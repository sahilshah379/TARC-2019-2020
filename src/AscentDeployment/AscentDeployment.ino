#include <Arduino_LPS22HB.h>
#include <Arduino_HTS221.h>

#define pin 10

const double gravity = 9.8;
const double mass = 598.28/1000;
const int apogee = 800;
const double parachuteK = 0.405124654;

const double seaLevelPressure = 1021; // https://weather.us/observations/pressure-qnh.html
double prevAlt = 0;
double prevTime = 0;

const int refresh_interval = 3333;
const int open_pulse = 1000;

void setup() {
  pinMode(pin, OUTPUT);

  if (!BARO.begin()) {
      while(1) {
        digitalWrite(pin, HIGH);
        delay(500);
        digitalWrite(pin, LOW);
        delay(500);
      }
  }
  if (!HTS.begin()) {
      while(1) {
        digitalWrite(pin, HIGH);
        delay(500);
        digitalWrite(pin, LOW);
        delay(500);
      }
  }
  prevTime = millis();
}

void loop() {
  double altitude = getAltitude();
  double velocity = getVelocity(altitude);
  if (altitude + getMaxAltitude(velocity) >= apogee/3.281) {
    for (int i = 0; i < 120; i += 1) {
        delayMicroseconds(open_pulse);
        delayMicroseconds(refresh_interval-open_pulse);
    }
    while(1) {}
  }
  delay(100);
}

double getAltitude() {
  double temperature = HTS.readTemperature();
  double pressure = BARO.readPressure()*10;
  double altitude = ((pow((seaLevelPressure/pressure),(1/5.257))-1)*(temperature+273.15))/0.0065;
  return altitude;
}
double getMaxAltitude(double velocity) {
	double vt = sqrt(mass*gravity/parachuteK);
	double yMax = (pow(vt,2)/(2*gravity))*log((pow(velocity,2)+pow(vt,2))/(pow(vt,2)));
	return yMax;
}
double getVelocity(double altitude) {
  double velocity = 1000*(altitude-prevAlt)/((millis()-prevTime));
  prevAlt = altitude;
  prevTime = millis();
  return velocity;
}
