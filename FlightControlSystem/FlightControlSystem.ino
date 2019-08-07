#include <Arduino_LPS22HB.h>
#include <Arduino_HTS221.h>
#include <math.h>

const double mass = 630.0/1000; // kg
const double referenceArea = PI*pow((.155*2.54/2),2) - PI*pow((.06*2.54/2),2); // m^2
const double dragCoefficient = 1.5;
const double finalAltitude = 800/3.281; // m
const double endTime = 40; // s (40-43)

const double seaLevelPressure = 1023; // hPa
const double gravity = 9.80665; // m/s^2
const double gasConstant = 8.314; // J/(K·mol)
const double molarMassDryAir = 0.028964; // kg/mol
const double molarMassWaterVapor = 0.018016; // kg/mol


double startTime;
double initialAltitude;
double airDensity;

void setup() {
  if (!BARO.begin()) {
    Serial.println("Failed to initialize pressure sensor!");
    while (1);
  }
  if (!HTS.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
  }
  airDensity = getAirDensity();
  initialAltitude = getAltitude();
}

void loop() {
  double altitude = getAltitude()-initialAltitude; // m
  double k = getAirResistanceCoefficient();
  double velocity = 0; // m/s
  if ((finalAltitude-altitude) <= (math.pow(velocity,2)/(2*(gravity+(k*math.pow(velocity,2)/mass))))) {
    // separate
  }
  delay(10);
}

double getAirResistanceCoefficient() {
  double airResistanceCoefficient = 0.5*dragCoefficient*referenceArea*airDensity;
  return airResistanceCoefficient;
}
double getAirDensity(double temperature, double pressure, double humidity) {
  double temperature = HTS.readTemperature(); // celsius
  double pressure = BARO.readPressure()*10; // hPa
  double humidity = HTS.readHumidity(); // %
  
  double a = 17.62; double b = 243.12; // dew point constants
  double gamma = math.log(humidity/100)+((a*temperature)/(b+temperature));
  double dewPoint = (b*gamma)/(a-gamma); // celsius
  double saturationVaporPressure = 6.102*math.pow(10,(7.5*dewPoint/(dewPoint+237.8))); // hPa
  double vaporPressure = dewPoint*saturationVaporPressure; // hPa
  double dryPressure = pressure-vaporPressure; // hPa
  double airDensity = (((dryPressure*100)*molarMassDryAir)+((vaporPressure*100)*molarMassWaterVapor))/(gasConstant*(temperature+273.15)); // kg/m^3
  return airDensity;
}
double getAltitude(double temperature, double pressure) {
  double temperature = HTS.readTemperature(); // celsius
  double pressure = BARO.readPressure()*10; // hPa
  
  double altitude = math.log(pressure/seaLevelPressure)*(gasConstant*(temperature+273.15))/(-1*gravity*molarMassDryAir);
  return altitude;
}

// https://weather.us/observations/new-jersey/pressure-qnh/20190324-1400z.html
