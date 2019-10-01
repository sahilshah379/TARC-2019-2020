import math

gravity = 9.78 # m/s^2
mass = 576.95/1000 # g to kg
apogee = 1021.74

def main():
	y = 600.07/3.281 # ft to m (altitude)
	vy = 178.568/3.281 # ft/s to m/s (yVelocity)
	D = 2.10378 # N (drag)
	theta = (abs(108.915-180))*(math.pi/180) # normalize then to radians
	v = vy/math.sin(theta) # velocity

	k = D/pow(v,2)
	# k = (mass*gravity)/pow(13.559,2)
	vt = math.sqrt((mass*gravity)/k)
	yMax = getMaxAltitude(vt,vy)

	yTotal = (y+yMax)*3.281
	print('Total: %s ft' %round(yTotal,3))
	print('Deviance: %s ft' %round(yTotal-apogee,3))

def getMaxAltitude(vt,v0):
	yMax = (math.pow(vt,2)/(2*gravity))*math.log((pow(v0,2)+pow(vt,2))/(pow(vt,2)))
	maxAltitude = yMax
	return maxAltitude

if __name__ == '__main__':
	main()