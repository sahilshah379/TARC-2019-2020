#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <math.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define SEALEVELPRESSURE_HPA (1023) //https://weather.us/observations/new-jersey/pressure-qnh/20190324-1400z.html

const double mass = 370.0/1000; // kg
const double parachuteArea = PI*pow((.155*2.54/2),2) - PI*pow((.06*2.54/2),2); // m^2
const double dragCoefficient = 1.5;
const double finalAltitude = 800/3.2808; // m
const double endTime = 40; // s  -  40 - 43

const double gravity = 9.80665;
const double e = 2.71828;
const double gasConstant = 287.058;

double start;
double initialAltitude;

void setup() {
  pinMode(13,HIGH);    
  initialAltitude = getAltitude(); // m
  start = millis();
}

void loop() {
  // deploy parachute already during ascent?
  
  double currentAltitude = getAltitude(); // m
  double altitude = currentAltitude - initialAltitude; // m

  double temperature = getTemperature(); // *C
  double pressure = getPressure(); // Pa
  double now = millis();
  double timePassed = (now-start)/1000;

  double timeParachute = descentTime(altitude, pressure, temperature);
  if ((timePassed + timeParachute) > endTime) {
    // turn servo to decrease string length
  } else if ((timePassed + timeParachute) < endTime) {
    // turn servo to increase string length
  }
  delay(10);
}

double descentTime(double altitude, double pressure, double temperature) {
  double airDensity = pressure/(gasConstant*(temperature+273.15)); // kg/m^3  -  around 1.225
  double airResistance = ((airDensity*parachuteArea)/2)*dragCoefficient; // kg/m  -  around 0.24 for air
  double descentTime = sqrt(mass/(gravity*airResistance))*acosh(pow(e,((altitude*airResistance)/mass))); // s
  return descentTime; // time to reach the ground if the parachute is open
}

double acosh(double x) {
  return log(x+sqrt(pow(x,2)-1));
}

//http://www.ambrsoft.com/Physics/FreeFall/FreeFallWairResistance.htm
//https://keisan.casio.com/exec/system/1231475371
//http://www.rocketmime.com/rockets/descent.html
//https://www.omnicalculator.com/physics/free-fall-air-resistance
//https://www.omnicalculator.com/physics/drag-equation
//https://www.omnicalculator.com/physics/air-density
//https://www.researchgate.net/publication/283105108_Reversible_Control_Line_Reefing_System_for_Circular_Parachutes/link/5899ed6f92851c8bb6820d48/download
//https://apps.dtic.mil/dtic/tr/fulltext/u2/a036497.pdf
