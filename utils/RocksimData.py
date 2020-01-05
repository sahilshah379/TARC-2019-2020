import csv

flight_data = []

def main():
	readCSV()
	altitude = flight_data[[i for i in range(len(flight_data)) if flight_data[i]['Time'] == 0.105][0]]['Altitude Feet']
	print(altitude)

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