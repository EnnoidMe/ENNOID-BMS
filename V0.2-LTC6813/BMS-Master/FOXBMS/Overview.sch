EESchema Schematic File Version 4
EELAYER 26 0
EELAYER END
$Descr User 17931 12130
encoding utf-8
Sheet 1 23
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
L master-eagle-import:FRAME_B_L FRAME4
U 1 0 C64DA869B9984423
P 900 11100
AR Path="/C64DA869B9984423" Ref="FRAME4"  Part="1" 
AR Path="/5A953E9D/C64DA869B9984423" Ref="FRAME4"  Part="1" 
F 0 "FRAME4" H 900 11100 50  0001 C CNN
F 1 "FRAME_B_L" H 900 11100 50  0001 C CNN
F 2 "" H 900 11100 50  0001 C CNN
F 3 "" H 900 11100 50  0001 C CNN
	1    900  11100
	1    0    0    -1  
$EndComp
$Comp
L master-eagle-import:FRAME_B_L FRAME4
U 2 0 C64DA869B998442F
P 13000 11000
AR Path="/C64DA869B998442F" Ref="FRAME4"  Part="2" 
AR Path="/5A953E9D/C64DA869B998442F" Ref="FRAME4"  Part="2" 
F 0 "FRAME4" H 13000 11000 50  0001 C CNN
F 1 "FRAME_B_L" H 13000 11000 50  0001 C CNN
F 2 "" H 13000 11000 50  0001 C CNN
F 3 "" H 13000 11000 50  0001 C CNN
	2    13000 11000
	1    0    0    -1  
$EndComp
Text Notes 14120 10150 0    85   ~ 0
Overview
Wire Notes Line
	1900 3700 3900 3700
Wire Notes Line
	3900 3700 3900 4100
Wire Notes Line
	3900 4100 3900 4700
Wire Notes Line
	3900 4700 3900 6100
Wire Notes Line
	3900 6100 3200 6100
Wire Notes Line
	3200 6100 1900 6100
Wire Notes Line
	1900 6100 1900 3700
Text Notes 2300 4700 0    85   ~ 0
MCU_1\nSecondary\nSTM32F429 \nTQFP-100
Wire Notes Line
	8500 2800 10400 2800
Wire Notes Line
	10400 2800 10400 3200
Wire Notes Line
	10400 3200 10400 3700
Wire Notes Line
	10400 3700 10400 4100
Wire Notes Line
	10400 4100 10400 4600
Wire Notes Line
	10400 4600 10400 5000
Wire Notes Line
	10400 5000 10400 5500
Wire Notes Line
	10400 5500 10400 6100
Wire Notes Line
	10400 6100 9300 6100
Wire Notes Line
	9300 6100 8500 6100
Wire Notes Line
	8500 6100 8500 5600
Wire Notes Line
	8500 5600 8500 4100
Wire Notes Line
	8500 4100 8500 2800
Wire Notes Line
	1600 2200 3200 2200
Wire Notes Line
	3200 2200 5200 2200
Wire Notes Line
	5200 2200 5200 9200
Wire Notes Line
	5200 9200 1600 9200
Wire Notes Line
	1600 9200 1600 2200
Text Notes 1700 2600 0    85   ~ 0
Isolated Supply_1 Secondary\n(+3.3V_1, +5.0V_1, GND_1)
Wire Notes Line
	7900 2200 9400 2200
Wire Notes Line
	9400 2200 17100 2200
Wire Notes Line
	17100 2200 17100 9200
Wire Notes Line
	17100 9200 7900 9200
Wire Notes Line
	7900 9200 7900 2200
Text Notes 8000 2600 0    85   ~ 0
Isolated Supply_0 Primary\n(+3.3V_0, +5.0V_0, GND_0)
Text Notes 5400 4000 0    59   ~ 0
Isolated MCU to MCU Communication (SPI)
Wire Notes Line
	9400 1700 9400 2200
Wire Notes Line
	6700 1700 3200 1700
Wire Notes Line
	3200 1700 3200 2200
Text Notes 5700 800  0    85   ~ 0
External Supply 12V to 24V
Wire Notes Line
	3900 4100 8500 4100
Wire Notes Line
	1900 6700 3200 6700
Wire Notes Line
	3200 6700 4000 6700
Wire Notes Line
	4000 6700 4000 7400
Wire Notes Line
	4000 7400 1900 7400
Wire Notes Line
	1900 7400 1900 6700
Text Notes 2100 7300 0    85   ~ 0
Connector to \nBMS-Interface_1 \nSecondary\nincl. supply
Wire Notes Line
	3200 6100 3200 6700
Text Notes 3300 6400 0    59   ~ 0
SPI
Text Notes 2000 5800 0    85   ~ 0
MCU_1 including:\n- JTAG/SWD Interface\n- USB (Bootable)
Text Notes 8800 3700 0    85   ~ 0
MCU_0\nPrimary\nSTM32F429 \nTQFP-176
Wire Notes Line
	8500 6700 9300 6700
Wire Notes Line
	9300 6700 10400 6700
Wire Notes Line
	10400 6700 10400 7400
Wire Notes Line
	10400 7400 8500 7400
Wire Notes Line
	8500 7400 8500 6700
Text Notes 8700 7300 0    85   ~ 0
Connector to \nBMS-Interface_0\nPrimary\nincl. supply
Wire Notes Line
	9300 6100 9300 6700
Text Notes 9400 6400 0    59   ~ 0
SPI
Text Notes 8600 4900 0    85   ~ 0
MCU_0 including:\n- RTC\n- JTAG/SWD Interface\n- USB (bootable)
Wire Notes Line
	11700 2900 13300 2900
Wire Notes Line
	13300 2900 13300 3600
Wire Notes Line
	13300 3600 11700 3600
Wire Notes Line
	11700 3600 11700 3200
Text Notes 12000 5200 0    85   ~ 0
Connector for \nFPGA addon\n(45MHz SPI)
Text Notes 13600 4500 0    59   ~ 0
Memory Controler Interface
Wire Notes Line
	11700 3200 11700 2900
Wire Notes Line
	15200 3400 16800 3400
Wire Notes Line
	16800 3400 16800 4100
Wire Notes Line
	16800 4100 15200 4100
Wire Notes Line
	15200 4100 15200 3700
Text Notes 15400 5700 0    85   ~ 0
Data Storage:\n- EEPROM\n- MicroSDCARD
Wire Notes Line
	15200 3700 15200 3400
Wire Notes Line
	11700 3800 13300 3800
Wire Notes Line
	13300 3800 13300 4500
Wire Notes Line
	13300 4500 11700 4500
Wire Notes Line
	11700 4500 11700 4100
Text Notes 11900 4200 0    85   ~ 0
Bender Isometer
Wire Notes Line
	11700 4100 11700 3800
Wire Notes Line
	10400 3200 11700 3200
Wire Notes Line
	15200 3700 10400 3700
Wire Notes Line
	10400 4100 11700 4100
Text Notes 10900 4900 0    59   ~ 0
SPI
Text Notes 14000 5400 0    59   ~ 0
SPI
Wire Notes Line
	15200 4300 16800 4300
Wire Notes Line
	16800 4300 16800 5000
Wire Notes Line
	16800 5000 15200 5000
Wire Notes Line
	15200 5000 15200 4600
Text Notes 15500 4700 0    85   ~ 0
8 MB SDRAM
Wire Notes Line
	15200 4600 15200 4300
Wire Notes Line
	15200 4600 10400 4600
Text Notes 10500 4000 0    59   ~ 0
Isolated Digital/PWM
Wire Notes Line
	11700 4700 13300 4700
Wire Notes Line
	13300 4700 13300 5400
Wire Notes Line
	13300 5400 11700 5400
Wire Notes Line
	11700 5400 11700 5000
Text Notes 11800 3400 0    85   ~ 0
9x Contactor\nControl+Feedback
Wire Notes Line
	11700 5000 11700 4700
Wire Notes Line
	10400 5000 11700 5000
Text Notes 10700 3100 0    59   ~ 0
Isolated Digital
Wire Notes Line
	6700 1700 6700 900 
Wire Notes Line
	15200 5200 16800 5200
Wire Notes Line
	16800 5200 16800 5900
Wire Notes Line
	16800 5900 15200 5900
Wire Notes Line
	15200 5900 15200 5500
Wire Notes Line
	15200 5500 15200 5200
Wire Notes Line
	10400 5500 15200 5500
Text Notes 13800 3600 0    59   ~ 0
2x Isolated CAN
Text Notes 15400 3900 0    85   ~ 0
2x CAN\n(one Bootable)
Wire Notes Line
	5400 4400 7000 4400
Wire Notes Line
	7000 4400 7000 5100
Wire Notes Line
	7000 5100 6700 5100
Wire Notes Line
	6700 5100 6500 5100
Wire Notes Line
	6500 5100 5400 5100
Wire Notes Line
	5400 5100 5400 4700
Text Notes 5500 4900 0    85   ~ 0
Interlock\nControl+Feedback
Wire Notes Line
	5400 4700 5400 4400
Wire Notes Line
	6100 5300 6500 5300
Wire Notes Line
	6500 5300 6700 5300
Wire Notes Line
	6700 5300 7700 5300
Wire Notes Line
	7700 5300 7700 5600
Wire Notes Line
	7700 5600 7700 6000
Wire Notes Line
	7700 6000 6700 6000
Wire Notes Line
	6700 6000 6500 6000
Wire Notes Line
	6500 6000 6100 6000
Wire Notes Line
	6100 6000 6100 5300
Text Notes 6200 5800 0    85   ~ 0
Interlock\nControl+Feedback
Wire Notes Line
	7700 5600 8500 5600
Wire Notes Line
	3900 4700 5400 4700
Wire Notes Line
	6500 10100 6500 6000
Wire Notes Line
	6500 5300 6500 5100
Wire Notes Line
	6700 5300 6700 5100
Text Notes 4000 4600 0    59   ~ 0
Isolated Digital
Text Notes 8000 5500 0    59   ~ 0
Isolated \nDigital
Wire Notes Line
	8500 8100 10400 8100
Wire Notes Line
	10400 8100 10400 8800
Wire Notes Line
	10400 8800 8500 8800
Wire Notes Line
	8500 8800 8500 8600
Text Notes 8700 8600 0    85   ~ 0
Interlock 10mA\nCurrent Source
Wire Notes Line
	8500 8600 8500 8400
Wire Notes Line
	8500 8400 8500 8100
Wire Notes Line
	8500 8600 6700 8600
Wire Notes Line
	6700 8600 6700 10100
Wire Notes Line
	8500 8400 6700 8400
Wire Notes Line
	6700 8400 6700 6000
Text Notes 12000 6100 0    85   ~ 0
RS485\n(bootable)
Wire Notes Line
	11700 5600 13300 5600
Wire Notes Line
	13300 5600 13300 6300
Wire Notes Line
	13300 6300 11700 6300
Wire Notes Line
	11700 6300 11700 5900
Wire Notes Line
	11700 5900 11700 5600
Wire Notes Line
	10400 5900 11700 5900
Text Notes 10600 5800 0    59   ~ 0
Isolated Digital
Wire Notes Line
	9400 1700 6900 1700
Wire Notes Line
	6900 1700 6900 900 
$EndSCHEMATC
