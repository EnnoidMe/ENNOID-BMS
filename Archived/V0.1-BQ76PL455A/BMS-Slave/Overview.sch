EESchema Schematic File Version 4
LIBS:slave-cache
EELAYER 26 0
EELAYER END
$Descr User 16157 12428
encoding utf-8
Sheet 2 18
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L slave-eagle-import:FOXBMS_BMS-MASTER_V1.00_A_FRAME_B_L FRAME4
U 1 0 C64DA869B9984423
P 1500 11400
AR Path="/C64DA869B9984423" Ref="FRAME4"  Part="1" 
AR Path="/5A91C4EF/C64DA869B9984423" Ref="FRAME4"  Part="1" 
F 0 "FRAME4" H 1500 11400 50  0001 C CNN
F 1 "FOXBMS_BMS-MASTER_V1.00_A_FRAME_B_L" H 1500 11400 50  0001 C CNN
F 2 "" H 1500 11400 50  0001 C CNN
F 3 "" H 1500 11400 50  0001 C CNN
	1    1500 11400
	1    0    0    -1  
$EndComp
$Comp
L slave-eagle-import:FOXBMS_BMS-MASTER_V1.00_A_FRAME_B_L FRAME4
U 2 0 C64DA869B998442F
P 13600 11300
AR Path="/C64DA869B998442F" Ref="FRAME4"  Part="2" 
AR Path="/5A91C4EF/C64DA869B998442F" Ref="FRAME4"  Part="2" 
F 0 "FRAME4" H 13600 11300 50  0001 C CNN
F 1 "FOXBMS_BMS-MASTER_V1.00_A_FRAME_B_L" H 13600 11300 50  0001 C CNN
F 2 "" H 13600 11300 50  0001 C CNN
F 3 "" H 13600 11300 50  0001 C CNN
	2    13600 11300
	1    0    0    -1  
$EndComp
Text Notes 4700 3300 0    150  ~ 0
LTC6804-1\nLTC6811-1\nPrimary\n\nincluding \npower supply
Wire Notes Line
	4300 5900 6400 5900
Wire Notes Line
	6400 5900 6400 700 
Wire Notes Line
	6400 700  4300 700 
Text Notes 8000 6800 0    150  ~ 0
temperature measurement circuit:\n8 T-sensors via connector
Wire Notes Line
	12100 6300 7800 6300
Wire Notes Line
	7800 6300 7800 6900
Wire Notes Line
	7800 6900 12100 6900
Wire Notes Line
	12100 6900 12100 6300
Text Notes 8000 7900 0    150  ~ 0
16 digital or analog inputs via pin headers\n8 digital inputs or outputs via pin headers
Wire Notes Line
	13900 7300 7800 7300
Wire Notes Line
	7800 7300 7800 8100
Wire Notes Line
	7800 8100 13900 8100
Wire Notes Line
	13900 8100 13900 7300
Text Notes 7900 8800 0    150  ~ 0
global balancing feedback
Wire Notes Line
	11200 8500 7800 8500
Wire Notes Line
	7800 8500 7800 8900
Wire Notes Line
	7800 8900 11200 8900
Wire Notes Line
	11200 8900 11200 8500
Text Notes 8000 9600 0    150  ~ 0
communication circuit
Wire Notes Line
	10800 9300 7800 9300
Wire Notes Line
	7800 9300 7800 9700
Wire Notes Line
	7800 9700 10800 9700
Wire Notes Line
	10800 9700 10800 9300
Text Notes 8000 10300 0    150  ~ 0
cell voltage measurement circuit via fuses
Wire Notes Line
	13400 10000 7800 10000
Wire Notes Line
	7800 10000 7800 10400
Wire Notes Line
	7800 10400 13400 10400
Wire Notes Line
	13400 10400 13400 10000
Text Notes 4700 9000 0    150  ~ 0
LTC6804-1\nLTC6811-1\nSecondary\n\nincluding \npower supply
Wire Notes Line
	4300 11300 6400 11300
Wire Notes Line
	6400 11300 6400 6300
Wire Notes Line
	6400 6300 4300 6300
Wire Notes Line
	6400 6600 7800 6600
Wire Notes Line
	6400 7700 7800 7700
Wire Notes Line
	6400 8700 7800 8700
Wire Notes Line
	6400 9500 7800 9500
Wire Notes Line
	6400 10200 7800 10200
Text Notes 6500 3100 0    100  ~ 0
control / read
Text Notes 6500 8600 0    100  ~ 0
read
Text Notes 6500 9400 0    100  ~ 0
isoSPI
Text Notes 6500 10100 0    100  ~ 0
read
Text Notes 6500 7600 0    100  ~ 0
read/ write
Text Notes 6500 6500 0    100  ~ 0
read
Text Notes 8000 1300 0    150  ~ 0
temperature measurement circuit:\n8 T-sensors via connector
Wire Notes Line
	12200 800  7800 800 
Wire Notes Line
	7800 800  7800 1400
Wire Notes Line
	7800 1400 12200 1400
Wire Notes Line
	12200 1400 12200 800 
Text Notes 8000 2300 0    150  ~ 0
16 digital or analog inputs via pin headers\n8 digital inputs or outputs via pin headers
Wire Notes Line
	13600 1700 7800 1700
Wire Notes Line
	7800 1700 7800 2500
Wire Notes Line
	7800 2500 13600 2500
Wire Notes Line
	13600 2500 13600 1700
Text Notes 8000 3400 0    150  ~ 0
passive balancing circuit\nglobal balancing feedback
Wire Notes Line
	11300 2900 7800 2900
Wire Notes Line
	7800 2900 7800 3500
Wire Notes Line
	7800 3500 11300 3500
Wire Notes Line
	11300 3500 11300 2900
Text Notes 8000 4200 0    150  ~ 0
communication circuit
Wire Notes Line
	10900 3900 7800 3900
Wire Notes Line
	7800 3900 7800 4300
Wire Notes Line
	7800 4300 10900 4300
Wire Notes Line
	10900 4300 10900 3900
Text Notes 8000 4900 0    150  ~ 0
cell voltage measurement circuit via fuses
Wire Notes Line
	13400 4600 7800 4600
Wire Notes Line
	7800 4600 7800 5000
Wire Notes Line
	7800 5000 13400 5000
Wire Notes Line
	13400 5000 13400 4600
Wire Notes Line
	6400 1100 7800 1100
Wire Notes Line
	6400 2100 7800 2100
Wire Notes Line
	6400 3200 7800 3200
Wire Notes Line
	6400 4100 7800 4100
Wire Notes Line
	6400 4800 7800 4800
Text Notes 6500 1000 0    100  ~ 0
read
Text Notes 6500 2000 0    100  ~ 0
read / write
Text Notes 6500 4000 0    100  ~ 0
isoSPI
Text Notes 6500 4700 0    100  ~ 0
read
Text Notes 2000 6100 0    150  ~ 0
battery \ncell\nsupply\n11 - 60 V\nvia fuses
Wire Notes Line
	3600 3200 4300 3200
Wire Notes Line
	3600 8800 4300 8800
Wire Notes Line
	3200 6100 3600 6100
Text Notes 14800 10400 0    85   ~ 0
Overview
Text Notes 8000 5700 0    150  ~ 0
board temperature sensor\nEEPROM with unique ID
Wire Notes Line
	11300 5200 7800 5200
Wire Notes Line
	7800 5200 7800 5800
Wire Notes Line
	7800 5800 11300 5800
Wire Notes Line
	11300 5800 11300 5200
Wire Notes Line
	6400 5500 7800 5500
Text Notes 6500 5400 0    100  ~ 0
read / write
Text Notes 8000 11100 0    150  ~ 0
board temperature sensor\nEEPROM with unique ID
Wire Notes Line
	11300 10600 7800 10600
Wire Notes Line
	7800 10600 7800 11200
Wire Notes Line
	7800 11200 11300 11200
Wire Notes Line
	11300 11200 11300 10600
Wire Notes Line
	6400 10900 7800 10900
Text Notes 6500 10800 0    100  ~ 0
read
Text Notes 2000 7600 0    150  ~ 0
optional\next. DC\nsupply\n8 - 24 V
Wire Notes Line
	3800 6700 4300 6700
Wire Notes Line
	3800 5500 4300 5500
Wire Notes Line
	4300 700  4300 5900
Wire Notes Line
	4300 6300 4300 11300
Wire Notes Line
	3600 3200 3600 8800
Wire Notes Line
	3800 5500 3800 7600
Wire Notes Line
	3200 7600 3800 7600
$EndSCHEMATC
