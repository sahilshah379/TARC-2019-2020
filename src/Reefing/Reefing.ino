#include <Arduino_LPS22HB.h>
#include <Arduino_HTS221.h>

const double mass = 630.0/1000; // kg
const double referenceArea = PI*pow((.155*2.54/2),2) - PI*pow((.06*2.54/2),2); // m^2
const double dragCoefficient = 1.5;
const double finalAltitude = 800/3.281; // m
const double endTime = 41.5; // s (40-43)
const double reefingAltitude = 70; // m
const double timeBuffer = 2; // s (because of the limits of the formula for k)

const double seaLevelPressure = 1023; // hPa
const double gravity = 9.80665; // m/s^2
const double gasConstant = 8.314; // J/(K·mol)
const double molarMassDryAir = 0.028964; // kg/mol
const double molarMassWaterVapor = 0.018016; // kg/mol

int progress = 0; // 0 = standby, 1 = ascent, 2 = drogue ascent, 3 = descent free fall, 4 = descent reef, 5 = landing

double startTime;
double initialAltitude;
double airDensity;
double apogee;

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
  
  if (progress == 0 && altitude > 2) {
    startTime = millis();
    progress = 1;
  }

  if (progress == 1) {
    double velocity = 0; // m/s
    if ((finalAltitude-altitude) <= (pow(velocity,2)/(2*(gravity+(k*pow(velocity,2)/mass))))) {
      // separate
      apogee = altitude;
      progress = 2;
    }
  }
  
  if (progress == 2) {
    if ((apogee - altitude) > 3) {
      progress = 3;
    }
  }

  if (progress == 3) {
    // reel in servos
    if (altitude < reefingAltitude) {
      progress = 4;
    }
  }

  if (progress == 4) {
    double timeRemaining = targetTime-((millis()-startTime)/1000);

    if (timeRemaining > 2) {
      double velocity = 0; // m/s
      double k = getAirResistanceCoefficient();
      
      bool increase;
      double heightDifference;
      if (velocity*timeRemaining <= altitude) {
        heightDifference = altitude-(velocity*timeRemaining);
        increase = true;
      } else {
        heightDifference = (velocity*timeRemaining)-altitude;
        increase = false;
      }
  
      kPart = (-2*log(2)*mass*heightDifference+mass*pow(timeRemaining,2)*gravity+mass*timeRemaining*sqrt(-1*gravity*(4*log(2)*heightDifference-pow(timeRemaining,2)*gravity)))/(2*pow(heightDifference,2));
      terminalVelocityPart = sqrt(mass*gravity/kPart);
  
      double terminalVelocityOverall;
      if (increase) {
        terminalVelocityOverall = velocity+terminalVelocityPart;
      } else {
        terminalVelocityOverall = velocity-terminalVelocityPart;
      }
      double k = mass*gravity/pow(terminalVelocityOverall,2);
  
      // set servo to k
    } else {
      progress = 5;
    }
  }
  
  delay(100);
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
  double gamma = log(humidity/100)+((a*temperature)/(b+temperature));
  double dewPoint = (b*gamma)/(a-gamma); // celsius
  double saturationVaporPressure = 6.102*pow(10,(7.5*dewPoint/(dewPoint+237.8))); // hPa
  double vaporPressure = dewPoint*saturationVaporPressure; // hPa
  double dryPressure = pressure-vaporPressure; // hPa
  double airDensity = (((dryPressure*100)*molarMassDryAir)+((vaporPressure*100)*molarMassWaterVapor))/(gasConstant*(temperature+273.15)); // kg/m^3
  return airDensity;
}
double getAltitude(double temperature, double pressure) {
  double temperature = HTS.readTemperature(); // celsius
  double pressure = BARO.readPressure()*10; // hPa
  
  double altitude = log(pressure/seaLevelPressure)*(gasConstant*(temperature+273.15))/(-1*gravity*molarMassDryAir);
  return altitude;
}

// https://weather.us/observations/new-jersey/pressure-qnh/20190324-1400z.html
