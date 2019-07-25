import math

m = 650/1000 # mass
g = 9.80665 # gravity


def main():
	t = 32
	h = 245
	vi = 0
	k = (-2*math.log(2)*m*h+m*pow(t,2)*g+m*t*math.sqrt(-g*(4*math.log(2)*h-pow(t,2)*g)))/(2*pow(h,2))
	print('k: %s' %k)

if __name__ == '__main__':
	main()

# https://keisan.casio.com/exec/system/1231475371
# http://www.ambrsoft.com/Physics/FreeFall/FreeFallWairResistance.htm