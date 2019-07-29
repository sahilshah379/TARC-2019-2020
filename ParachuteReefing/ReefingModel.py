import math
import random

tUpper = 43 # upper time bound
tLower = 40 # lower time bound
tTarget = 41.5 # target landing time in the time range
tBuffer1 = 2 # time stopeed before tLower (because of the limits of the formula for k)
tBuffer2 = 1.15 # Time length less than tLower (to decrease the final velocity of the rocket)
m = 635/1000 # mass
g = 9.80665 # gravity
ascentTime = 6 # ground to apogee
ascentDifference = math.ceil(ascentTime)-ascentTime
time = tLower-math.ceil(ascentTime)-tBuffer2 # apogee to ground including buffers
height = 245 # apogee
deltaTime = 1 # time between each update

# 0.418983747 = 0.456036731*1.225*0.5*1.5

def main():
	t = time # time remaining
	h = height # altitude
	vi = 0
	k = 0
	kPrev = 0
	increase = True
	print('(t,h,vi,k): (%s,%s,%s,%s)' %(round(t+tBuffer2+ascentDifference,3),h,vi,k))
	while (t > tBuffer1):
		h = h # use altimeter data
		
		if vi*t <= h:
			h1 = h-(vi*t)
			increase = True
		else:
			h1 = (vi*t)-h
			increase = False

		kPart = (-2*math.log(2)*m*h1+m*pow(t,2)*g+m*t*math.sqrt(-g*(4*math.log(2)*h1-pow(t,2)*g)))/(2*pow(h1,2))
		termVelPart = math.sqrt(m*g/kPart)

		termVelOverall = None
		if (increase):
			termVelOverall = vi+termVelPart
		else:
			termVelOverall = vi-termVelPart

		k = m*g/pow(termVelOverall,2) # change the servo to match this k value
		if (increase):
			vi += math.sqrt(m*g/kPart)*math.tanh(math.sqrt(g*kPart/m)*deltaTime)
		else:
			vi -= math.sqrt(m*g/kPart)*math.tanh(math.sqrt(g*kPart/m)*deltaTime)
		
		# beginning of model specific calculations
		if (abs(h1) > 0.01):
			hPart = None
			try:
				hPart = (m/kPart)*math.log(math.cosh(math.sqrt(g*kPart/m)*deltaTime))
			except:
				hPart = 0
			if (increase):
				deltaH = (vi*deltaTime)+hPart
			else:
				deltaH = (vi*deltaTime)-hPart
			# deltaH *= (random.randint(95,105)/100)
			h -= deltaH
		# end of model specific calculations

		t -= deltaTime # delay 1 second
		print('(t,h,vi,k): (%s,%s,%s,%s)' %(round(t+tBuffer2+ascentDifference,3),h,vi,k))

	# final k value
	h = h # use altimeter data
	t += ((tTarget-tLower)+tBuffer2+ascentDifference) # include the target time and the buffer time
	h1Final = (vi*t)-h
	kPartFinal = (-2*math.log(2)*m*h1Final+m*pow(t,2)*g+m*t*math.sqrt(-g*(4*math.log(2)*h1Final-pow(t,2)*g)))/(2*pow(h1Final,2))
	termVelPartFinal = math.sqrt(m*g/kPartFinal)
	termVelOverallFinal = vi-termVelPartFinal
	k = m*g/pow(termVelOverallFinal,2) # change the servo to match this k value
	vi -= math.sqrt(m*g/kPartFinal)*math.tanh(math.sqrt(g*kPartFinal/m)*t)
	h = 0
	print('(t,h,vi,k): (%s,%s,%s,%s)' %(tLower-tTarget,h,vi,k))
if __name__ == '__main__':
	main()

# https://keisan.casio.com/exec/system/1231475371
# http://www.ambrsoft.com/Physics/FreeFall/FreeFallWairResistance.htm