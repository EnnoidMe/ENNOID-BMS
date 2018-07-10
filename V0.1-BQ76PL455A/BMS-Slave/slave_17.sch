EESchema Schematic File Version 4
LIBS:slave-cache
EELAYER 26 0
EELAYER END
$Descr User 18731 11828
encoding utf-8
Sheet 1 18
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
L slave-eagle-import:FOXBMS_BMS-MASTER_V1.00_A_FRAME_B_L FRAME13
U 1 0 CD21D0AF0B12B9C6
P 1500 10800
AR Path="/CD21D0AF0B12B9C6" Ref="FRAME13"  Part="1" 
AR Path="/5A91E755/CD21D0AF0B12B9C6" Ref="FRAME13"  Part="1" 
F 0 "FRAME13" H 1500 10800 50  0001 C CNN
F 1 "FOXBMS_BMS-MASTER_V1.00_A_FRAME_B_L" H 1500 10800 50  0001 C CNN
F 2 "" H 1500 10800 50  0001 C CNN
F 3 "" H 1500 10800 50  0001 C CNN
	1    1500 10800
	1    0    0    -1  
$EndComp
$Comp
L slave-eagle-import:FOXBMS_BMS-MASTER_V1.00_A_FRAME_B_L FRAME13
U 2 0 CD21D0AF0B12B9CA
P 13600 10700
AR Path="/CD21D0AF0B12B9CA" Ref="FRAME13"  Part="2" 
AR Path="/5A91E755/CD21D0AF0B12B9CA" Ref="FRAME13"  Part="2" 
F 0 "FRAME13" H 13600 10700 50  0001 C CNN
F 1 "FOXBMS_BMS-MASTER_V1.00_A_FRAME_B_L" H 13600 10700 50  0001 C CNN
F 2 "" H 13600 10700 50  0001 C CNN
F 3 "" H 13600 10700 50  0001 C CNN
	2    13600 10700
	1    0    0    -1  
$EndComp
Text Notes 14730 9850 0    85   ~ 0
Changelog
Text Notes 2300 900  0    170  ~ 0
Version
Text Notes 3800 900  0    170  ~ 0
Date
Text Notes 5900 900  0    170  ~ 0
Editor
Text Notes 8700 900  0    170  ~ 0
Comment
Wire Notes Line
	2100 1100 3700 1100
Wire Notes Line
	5800 1100 8600 1100
Wire Notes Line
	3700 700  5800 700 
Wire Notes Line
	8600 700  17900 700 
Wire Notes Line
	17900 700  17900 1100
Wire Notes Line
	8600 1100 8600 700 
Wire Notes Line
	8600 700  5800 700 
Wire Notes Line
	5800 700  5800 1100
Wire Notes Line
	3700 1100 3700 700 
Wire Notes Line
	3700 700  2100 700 
Wire Notes Line
	2100 700  2100 1100
Wire Notes Line
	2100 1100 2100 1500
Wire Notes Line
	2100 1500 3700 1500
Wire Notes Line
	3700 1500 5800 1500
Wire Notes Line
	8600 1500 17900 1500
Wire Notes Line
	17900 1500 17900 1100
Wire Notes Line
	17900 1100 8600 1100
Wire Notes Line
	8600 1100 8600 1500
Wire Notes Line
	8600 1500 5800 1500
Wire Notes Line
	5800 1500 5800 1100
Wire Notes Line
	5800 1100 3700 1100
Wire Notes Line
	3700 1100 3700 1500
Text Notes 3800 1300 0    127  ~ 0
2016-09-29
Text Notes 5900 1300 0    127  ~ 0
Sebastian Wacker
Text Notes 2300 1300 0    127  ~ 0
2.0.0
Text Notes 8700 1300 0    127  ~ 0
Initial Version
Text Notes 2300 1700 0    127  ~ 0
2.0.1
Text Notes 3800 1700 0    127  ~ 0
2016-11-15
Text Notes 5900 1700 0    127  ~ 0
Sebastian Wacker
Text Notes 8700 1700 0    127  ~ 0
Added missing cooling areas on bottom side, adjusted silk screen
Text Notes 8700 2000 0    127  ~ 0
Enlarged PCB tracks, R201/202/301/302 other package
Wire Notes Line
	5800 2800 5800 1500
Wire Notes Line
	8600 1500 8600 2800
Wire Notes Line
	8600 2800 5800 2800
Wire Notes Line
	8600 2800 17900 2800
Wire Notes Line
	3700 1500 3700 2800
Wire Notes Line
	3700 2800 5800 2800
Wire Notes Line
	2100 1500 2100 2800
Wire Notes Line
	2100 2800 3700 2800
Wire Notes Line
	17900 2800 17900 1500
Text Notes 8700 2300 0    127  ~ 0
Enlarged T201/301 cooling area
Text Notes 8700 2600 0    127  ~ 0
Replaced PCF8574 with PCA8574 (400 kHz I2C)
Text Notes 2300 8000 0    170  ~ 0
Device
Text Notes 6400 8000 0    170  ~ 0
I2C Address (without R/W bit)
Wire Notes Line
	11300 7800 6200 7800
Wire Notes Line
	6200 7800 2100 7800
Wire Notes Line
	2100 7800 2100 8200
Wire Notes Line
	6200 7800 6200 8200
Wire Notes Line
	11300 7800 11300 8200
Wire Notes Line
	11300 8200 6200 8200
Text Notes 2300 8400 0    127  ~ 0
MUX Temp. Sens. 0-7
Wire Notes Line
	6200 8200 2100 8200
Wire Notes Line
	11300 8600 6200 8600
Text Notes 6400 8400 0    127  ~ 0
0x4C
Wire Notes Line
	6200 8600 2100 8600
Wire Notes Line
	6200 8200 6200 8600
Wire Notes Line
	2100 8200 2100 8600
Wire Notes Line
	11300 8200 11300 8600
Text Notes 6400 8800 0    127  ~ 0
0x4D
Text Notes 6400 9200 0    127  ~ 0
0x4E
Wire Notes Line
	6200 9000 2100 9000
Wire Notes Line
	11300 9000 6200 9000
Wire Notes Line
	6200 8600 6200 9000
Wire Notes Line
	11300 8600 11300 9000
Wire Notes Line
	11300 9000 11300 9400
Wire Notes Line
	6200 9000 6200 9400
Wire Notes Line
	11300 9400 6200 9400
Wire Notes Line
	6200 9400 6200 9800
Wire Notes Line
	11300 9400 11300 9800
Wire Notes Line
	11300 9800 6200 9800
Wire Notes Line
	2100 8600 2100 9000
Wire Notes Line
	2100 9000 2100 9400
Wire Notes Line
	2100 9400 2100 9800
Wire Notes Line
	6200 9400 2100 9400
Wire Notes Line
	6200 9800 2100 9800
Wire Notes Line
	2100 9800 2100 10200
Wire Notes Line
	6200 10200 2100 10200
Wire Notes Line
	6200 9800 6200 10200
Wire Notes Line
	11300 10200 6200 10200
Wire Notes Line
	11300 9800 11300 10200
Text Notes 2300 8800 0    127  ~ 0
MUX Analog In 0-7
Text Notes 2300 9200 0    127  ~ 0
MUX Analog In 8-15
Text Notes 2300 9600 0    127  ~ 0
Port Expander
Text Notes 2300 10000 0    127  ~ 0
EEPROM
Text Notes 6400 9600 0    127  ~ 0
0x20
Text Notes 6400 10000 0    127  ~ 0
0x50 (see manual for MSB addressing)
Text Notes 6400 10400 0    127  ~ 0
0x48
Wire Notes Line
	11300 10600 6200 10600
Wire Notes Line
	6200 10200 6200 10600
Wire Notes Line
	11300 10200 11300 10600
Wire Notes Line
	6200 10600 2100 10600
Wire Notes Line
	2100 10200 2100 10600
Text Notes 2300 10400 0    127  ~ 0
Board Temp. Sens.
Text Notes 2300 3000 0    127  ~ 0
2.0.2
Text Notes 3800 3000 0    127  ~ 0
2016-11-24
Text Notes 5900 3000 0    127  ~ 0
Sebastian Wacker
Text Notes 8700 3000 0    127  ~ 0
Replaced LTC1380 MUXs with ADG728 (400 kHz I2C)
Text Notes 8700 3300 0    127  ~ 0
Adjusted connection of 100 ohm resistors for V+ / V_REG supply 
Text Notes 8700 3600 0    127  ~ 0
Reduced value of I2C pullups to 1k2
Wire Notes Line
	13400 7800 11300 7800
Text Notes 11500 8000 0    170  ~ 0
I2C clock
Wire Notes Line
	13400 7800 13400 8200
Wire Notes Line
	13400 8200 11300 8200
Wire Notes Line
	13400 8200 13400 8600
Wire Notes Line
	13400 8600 13400 9000
Wire Notes Line
	13400 8600 11300 8600
Wire Notes Line
	13400 9000 11300 9000
Wire Notes Line
	13400 9400 11300 9400
Wire Notes Line
	13400 9800 11300 9800
Wire Notes Line
	13400 10200 11300 10200
Wire Notes Line
	13400 10600 11300 10600
Wire Notes Line
	13400 9000 13400 9400
Wire Notes Line
	13400 9400 13400 9800
Wire Notes Line
	13400 9800 13400 10200
Wire Notes Line
	13400 10200 13400 10600
Text Notes 11500 8400 0    127  ~ 0
400 kHz (max.)
Wire Notes Line
	8600 3800 5800 3800
Wire Notes Line
	8600 3800 17900 3800
Wire Notes Line
	3700 3800 5800 3800
Wire Notes Line
	2100 3800 3700 3800
Text Notes 2300 4000 0    127  ~ 0
2.0.3
Text Notes 3800 4000 0    127  ~ 0
2017-04-13
Text Notes 5900 4000 0    127  ~ 0
Sebastian Wacker
Text Notes 8700 4000 0    127  ~ 0
Fixed isoSPI transformer CMC issue
Wire Notes Line
	5800 4200 5800 3800
Wire Notes Line
	8600 3800 8600 4200
Wire Notes Line
	3700 3800 3700 4200
Wire Notes Line
	2100 3800 2100 4200
Wire Notes Line
	17900 4200 17900 3800
Text Notes 11500 8800 0    127  ~ 0
400 kHz (max.)
Text Notes 11500 9200 0    127  ~ 0
400 kHz (max.)
Text Notes 11500 9600 0    127  ~ 0
400 kHz (max.)
Text Notes 11500 10000 0    127  ~ 0
1 MHz (max.)
Text Notes 11500 10400 0    127  ~ 0
400 kHz (max.)
Wire Notes Line
	5800 3800 5800 2800
Wire Notes Line
	8600 2800 8600 3800
Wire Notes Line
	8600 4200 5800 4200
Wire Notes Line
	8600 4200 17900 4200
Wire Notes Line
	3700 2800 3700 3800
Wire Notes Line
	3700 4200 5800 4200
Wire Notes Line
	2100 2800 2100 3800
Wire Notes Line
	2100 4200 3700 4200
Wire Notes Line
	17900 3800 17900 2800
Text Notes 2300 4400 0    127  ~ 0
2.1.0
Text Notes 3800 4400 0    127  ~ 0
2017-09-04
Text Notes 5900 4400 0    127  ~ 0
Sebastian Wacker
Text Notes 8700 4700 0    127  ~ 0
Replaced I2C EEPROM 24AA02UID with M24M02-DR (ECC)
Text Notes 8700 5000 0    127  ~ 0
Switched connection of balancing resistors from serial to parallel
Wire Notes Line
	5800 5200 5800 4200
Wire Notes Line
	8600 4200 8600 5200
Wire Notes Line
	3700 4200 3700 5200
Wire Notes Line
	2100 4200 2100 5200
Wire Notes Line
	17900 5200 17900 4200
Text Notes 8700 4400 0    127  ~ 0
Added 8-24 V isolated external power supply
Wire Notes Line
	8600 5200 5800 5200
Wire Notes Line
	8600 5200 17900 5200
Wire Notes Line
	3700 5200 5800 5200
Wire Notes Line
	2100 5200 3700 5200
$EndSCHEMATC
