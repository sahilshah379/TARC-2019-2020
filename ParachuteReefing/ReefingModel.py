import math
import random

m = 650/1000 # mass
g = 9.80665 # gravity
time = 32
height = 245
deltaTime = 1 # time between each update

def main():
	t = time # time remaining
	h = height # altitude
	vi = 0
	k = 0
	kPrev = 0
	increase = True
	print('(t,h,vi,k): (%s,%s,%s,%s)' %(t,h,vi,k))
	while (t > 2):
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
			deltaH *= (random.randint(95,105)/100)
			h -= deltaH
		# end of model specific calculations

		t -= deltaTime
		print('(t,h,vi,k): (%s,%s,%s,%s)' %(t,h,vi,k))


if __name__ == '__main__':
	main()

# https://keisan.casio.com/exec/system/1231475371
# http://www.ambrsoft.com/Physics/FreeFall/FreeFallWairResistance.htm