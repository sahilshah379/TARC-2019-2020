import math
import csv

flight_data = [] # need to input time, altitude, y velocity, flight angle, and k

gravity = 9.78 # m/s^2
mass = 598.28/1000 # g to kg
apogee = 800

def main():
	readCSV()

	for i in range(len(flight_data)):
		t = flight_data[i]['Time'] # time
		y = flight_data[i]['Altitude Feet']/3.281 # altitude
		vy = flight_data[i]['y-Velocity Feet / Sec']/3.281 # y component of velocity
		theta = (abs(flight_data[i]['Flight angle Deg.']-180))*(math.pi/180) # theta
		v = vy/math.sin(theta) # velocity magnitude

		k = parachuteK()
		vt = math.sqrt((mass*gravity)/k)
		yMax = getMaxAltitude(vt,vy)

		yTotal = (y+yMax)*3.281

		if yTotal >= apogee:
			print('Index: %s' %i)
			print('Altitude: %s' %round(y*3.281,3))
			print('Apogee: %s' %round(yTotal,3))
			print('Time: %s' %round(t,3))
			break

def parachuteK():
	altitude = 10
	v = flight_data[[i for i in range(len(flight_data)) if abs(flight_data[i]['Altitude Feet']-altitude) < 1][0]]['y-Velocity Feet / Sec']/3.281
	k = (mass*gravity)/pow(v,2)
	return k

def getMaxAltitude(vt,v0):
	yMax = (math.pow(vt,2)/(2*gravity))*math.log((pow(v0,2)+pow(vt,2))/(pow(vt,2)))
	maxAltitude = yMax
	return maxAltitude

def readCSV():
	with open('TARC-2019-2020.csv', 'r') as file:
		fileData = file.read()
	fileData = fileData.replace(' ,',',')
	with open('TARC-2019-2020.csv', 'w') as file:
		fileData = file.write(fileData)
	with open('TARC-2019-2020.csv') as file:
		read = csv.DictReader(file, delimiter=',')
		for row in read:
			raw_data = dict(row)
			data = {k : float(v.replace(' ','')) for k,v in raw_data.items()}
			flight_data.append(data)

if __name__ == '__main__':
	main()