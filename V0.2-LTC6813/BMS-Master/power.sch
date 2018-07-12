EESchema Schematic File Version 4
LIBS:BMS-Master-cache
EELAYER 26 0
EELAYER END
$Descr A1 33110 23386
encoding utf-8
Sheet 5 25
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 850  2200 0    50   Input ~ 0
BAT+
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC18763
P 1450 2200
F 0 "R?" V 1243 2200 50  0000 C CNN
F 1 "200" V 1334 2200 50  0000 C CNN
F 2 "Resistors_SMD:R_2512" V 1380 2200 50  0001 C CNN
F 3 "~" H 1450 2200 50  0001 C CNN
	1    1450 2200
	0    1    1    0   
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC1876B
P 1950 2450
F 0 "R?" H 1880 2404 50  0000 R CNN
F 1 "200" H 1880 2495 50  0000 R CNN
F 2 "Resistors_SMD:R_2512" V 1880 2450 50  0001 C CNN
F 3 "~" H 1950 2450 50  0001 C CNN
	1    1950 2450
	-1   0    0    1   
$EndComp
Wire Wire Line
	1600 2200 1950 2200
Wire Wire Line
	1950 2200 1950 2300
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AC18775
P 2400 3000
F 0 "C?" H 2515 3046 50  0000 L CNN
F 1 "1u" H 2515 2955 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 2438 2850 50  0001 C CNN
F 3 "~" H 2400 3000 50  0001 C CNN
F 4 "50V" H 2400 3000 50  0001 C CNN "Volatge"
	1    2400 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 2600 1950 2750
Wire Wire Line
	1950 2750 2400 2750
Wire Wire Line
	2400 2750 2400 2850
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5AC1877F
P 2400 3250
F 0 "#PWR?" H 2400 3000 50  0001 C CNN
F 1 "GND" H 2405 3077 50  0000 C CNN
F 2 "" H 2400 3250 50  0001 C CNN
F 3 "" H 2400 3250 50  0001 C CNN
	1    2400 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 3150 2400 3250
$Comp
L BMS-Master-rescue:Q_NPN_BCE-Device Q?
U 1 1 5AC18786
P 1850 3700
F 0 "Q?" H 2041 3746 50  0000 L CNN
F 1 "ZXTN4004KTC" H 2041 3655 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:TO-252-2" H 2050 3800 50  0001 C CNN
F 3 "~" H 1850 3700 50  0001 C CNN
	1    1850 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 3500 1950 2750
Connection ~ 1950 2750
Text HLabel 1000 3700 0    50   Input ~ 0
NPN_BASE
Wire Wire Line
	1650 3700 1000 3700
Text HLabel 1050 4100 0    50   Input ~ 0
VP1
Wire Wire Line
	1050 4100 1950 4100
Wire Wire Line
	1950 4100 1950 3900
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC18794
P 1950 4400
F 0 "R?" H 2020 4446 50  0000 L CNN
F 1 "0.39" H 2020 4355 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 1880 4400 50  0001 C CNN
F 3 "~" H 1950 4400 50  0001 C CNN
	1    1950 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 4250 1950 4100
Connection ~ 1950 4100
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC1879D
P 2400 4400
F 0 "R?" H 2470 4446 50  0000 L CNN
F 1 "1" H 2470 4355 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2330 4400 50  0001 C CNN
F 3 "~" H 2400 4400 50  0001 C CNN
	1    2400 4400
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC187A4
P 2750 4400
F 0 "R?" H 2820 4446 50  0000 L CNN
F 1 "1" H 2820 4355 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2680 4400 50  0001 C CNN
F 3 "~" H 2750 4400 50  0001 C CNN
	1    2750 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 4100 2400 4100
Wire Wire Line
	2400 4100 2400 4250
Wire Wire Line
	2400 4100 2750 4100
Wire Wire Line
	2750 4100 2750 4250
Connection ~ 2400 4100
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AC187B1
P 1950 4850
F 0 "C?" H 2065 4896 50  0000 L CNN
F 1 "4.7u" H 2065 4805 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 1988 4700 50  0001 C CNN
F 3 "~" H 1950 4850 50  0001 C CNN
F 4 "50V" H 1950 4850 50  0001 C CNN "Volatge"
	1    1950 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 4550 1950 4700
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5AC187B9
P 1950 5150
F 0 "#PWR?" H 1950 4900 50  0001 C CNN
F 1 "GND" H 1955 4977 50  0000 C CNN
F 2 "" H 1950 5150 50  0001 C CNN
F 3 "" H 1950 5150 50  0001 C CNN
	1    1950 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 5000 1950 5150
Text HLabel 2400 4850 3    50   Input ~ 0
VDIG
Wire Wire Line
	2400 4550 2400 4850
Text HLabel 2750 4850 3    50   Input ~ 0
VIO
Wire Wire Line
	2750 4550 2750 4850
$Comp
L BMS-Master-rescue:LM5008MM-Regulator_Switching U?
U 1 1 5AC187C4
P 6900 2400
F 0 "U?" H 6900 2867 50  0000 C CNN
F 1 "LM5008MM" H 6900 2776 50  0000 C CNN
F 2 "Package_SO:MSOP-8_3x3mm_P0.65mm" H 6950 1950 50  0001 L CIN
F 3 "http://www.ti.com/lit/ds/symlink/lm5008.pdf" H 6900 2400 50  0001 C CNN
	1    6900 2400
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:LP2985-3.3-Regulator_Linear U?
U 1 1 5AC187CB
P 13400 2650
F 0 "U?" H 13400 2992 50  0000 C CNN
F 1 "LP2985-3.3" H 13400 2901 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 13400 2975 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lp2985.pdf" H 13400 2650 50  0001 C CNN
	1    13400 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 1650 6250 2200
Wire Wire Line
	6250 2200 6500 2200
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AC187D9
P 5400 2500
F 0 "C?" H 5515 2546 50  0000 L CNN
F 1 "0.1u" H 5515 2455 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5438 2350 50  0001 C CNN
F 3 "~" H 5400 2500 50  0001 C CNN
F 4 "50V" H 5400 2500 50  0001 C CNN "Volatge"
	1    5400 2500
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5AC187E0
P 5150 2750
F 0 "#PWR?" H 5150 2500 50  0001 C CNN
F 1 "GND" H 5155 2577 50  0000 C CNN
F 2 "" H 5150 2750 50  0001 C CNN
F 3 "" H 5150 2750 50  0001 C CNN
	1    5150 2750
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AC187E7
P 4900 2500
F 0 "C?" H 5015 2546 50  0000 L CNN
F 1 "1u" H 5015 2455 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 4938 2350 50  0001 C CNN
F 3 "~" H 4900 2500 50  0001 C CNN
F 4 "50V" H 4900 2500 50  0001 C CNN "Volatge"
	1    4900 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 2650 4900 2750
Wire Wire Line
	4900 2750 5150 2750
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC187F0
P 5700 1950
F 0 "R?" H 5770 1996 50  0000 L CNN
F 1 "174k" H 5770 1905 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5630 1950 50  0001 C CNN
F 3 "~" H 5700 1950 50  0001 C CNN
	1    5700 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 1800 5700 1650
Connection ~ 5700 1650
Wire Wire Line
	5700 1650 6250 1650
Wire Wire Line
	5700 2100 5700 2300
Wire Wire Line
	5700 2300 6500 2300
Wire Wire Line
	5400 2350 5400 1650
Connection ~ 5400 1650
Wire Wire Line
	5400 1650 5700 1650
Wire Wire Line
	4900 1650 5400 1650
Wire Wire Line
	4900 1650 4900 2350
Wire Wire Line
	5150 2750 5400 2750
Wire Wire Line
	5400 2750 5400 2650
Connection ~ 5150 2750
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC18805
P 6100 2600
F 0 "R?" V 5893 2600 50  0000 C CNN
F 1 "665k" V 5984 2600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 6030 2600 50  0001 C CNN
F 3 "~" H 6100 2600 50  0001 C CNN
	1    6100 2600
	0    1    1    0   
$EndComp
Wire Wire Line
	6250 2600 6500 2600
Wire Wire Line
	6500 2600 6500 2500
Wire Wire Line
	5950 2600 5850 2600
Wire Wire Line
	5850 2600 5850 2750
Wire Wire Line
	5850 3100 6900 3100
Wire Wire Line
	6900 3100 6900 2900
Wire Wire Line
	5850 2750 5400 2750
Connection ~ 5850 2750
Wire Wire Line
	5850 2750 5850 3100
Connection ~ 5400 2750
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AC18817
P 8350 1400
F 0 "C?" H 8465 1446 50  0000 L CNN
F 1 "4.7u" H 8465 1355 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 8388 1250 50  0001 C CNN
F 3 "~" H 8350 1400 50  0001 C CNN
F 4 "50V" H 8350 1400 50  0001 C CNN "Volatge"
	1    8350 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7650 1150 7650 2200
Wire Wire Line
	7650 2200 7300 2200
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5AC18820
P 8350 1650
F 0 "#PWR?" H 8350 1400 50  0001 C CNN
F 1 "GND" H 8355 1477 50  0000 C CNN
F 2 "" H 8350 1650 50  0001 C CNN
F 3 "" H 8350 1650 50  0001 C CNN
	1    8350 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 1650 8350 1550
Wire Wire Line
	8350 1150 8350 1250
Wire Wire Line
	7650 1150 8350 1150
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AC1882A
P 7900 2300
F 0 "C?" V 7950 2350 50  0000 L CNN
F 1 "0.1u" V 7850 2350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 7938 2150 50  0001 C CNN
F 3 "~" H 7900 2300 50  0001 C CNN
F 4 "50V" H 7900 2300 50  0001 C CNN "Volatge"
	1    7900 2300
	0    1    1    0   
$EndComp
Wire Wire Line
	7300 2300 7750 2300
Wire Wire Line
	7300 2500 8150 2500
Wire Wire Line
	8150 2500 8150 2300
Wire Wire Line
	8150 2300 8050 2300
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC18835
P 8450 2300
F 0 "R?" V 8243 2300 50  0000 C CNN
F 1 "1.2k" V 8334 2300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 8380 2300 50  0001 C CNN
F 3 "~" H 8450 2300 50  0001 C CNN
	1    8450 2300
	0    1    1    0   
$EndComp
Wire Wire Line
	8150 2300 8300 2300
Connection ~ 8150 2300
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AC1883F
P 9050 2300
F 0 "C?" V 9100 2350 50  0000 L CNN
F 1 "0.01u" V 9000 2350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 9088 2150 50  0001 C CNN
F 3 "~" H 9050 2300 50  0001 C CNN
F 4 "50V" H 9050 2300 50  0001 C CNN "Volatge"
	1    9050 2300
	0    1    1    0   
$EndComp
Wire Wire Line
	8600 2300 8750 2300
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AC18848
P 8750 2650
F 0 "C?" H 8550 2600 50  0000 L CNN
F 1 "0.01u" H 8400 2700 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 8788 2500 50  0001 C CNN
F 3 "~" H 8750 2650 50  0001 C CNN
F 4 "50V" H 8750 2650 50  0001 C CNN "Volatge"
	1    8750 2650
	-1   0    0    1   
$EndComp
Wire Wire Line
	8750 2500 8750 2300
Connection ~ 8750 2300
Wire Wire Line
	8750 2300 8900 2300
Wire Wire Line
	8750 2800 8750 3000
Wire Wire Line
	8750 3000 7550 3000
Wire Wire Line
	7550 3000 7550 2700
Wire Wire Line
	7550 2700 7300 2700
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AC18857
P 8750 3150
F 0 "C?" H 8550 3100 50  0000 L CNN
F 1 "0.01u" H 8400 3200 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 8788 3000 50  0001 C CNN
F 3 "~" H 8750 3150 50  0001 C CNN
F 4 "50V" H 8750 3150 50  0001 C CNN "Volatge"
	1    8750 3150
	-1   0    0    1   
$EndComp
Connection ~ 8750 3000
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5AC1885F
P 8750 3350
F 0 "#PWR?" H 8750 3100 50  0001 C CNN
F 1 "GND" H 8755 3177 50  0000 C CNN
F 2 "" H 8750 3350 50  0001 C CNN
F 3 "" H 8750 3350 50  0001 C CNN
	1    8750 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	8750 3300 8750 3350
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AC18867
P 9450 2650
F 0 "C?" H 9250 2600 50  0000 L CNN
F 1 "0.01u" H 9100 2700 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 9488 2500 50  0001 C CNN
F 3 "~" H 9450 2650 50  0001 C CNN
F 4 "50V" H 9450 2650 50  0001 C CNN "Volatge"
	1    9450 2650
	-1   0    0    1   
$EndComp
Wire Wire Line
	9450 2500 9450 2300
Wire Wire Line
	9450 2300 9200 2300
Wire Wire Line
	9450 2800 9450 3000
Wire Wire Line
	9450 3000 8750 3000
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC18872
P 10100 2700
F 0 "R?" H 10030 2654 50  0000 R CNN
F 1 "10k" H 10030 2745 50  0000 R CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 10030 2700 50  0001 C CNN
F 3 "~" H 10100 2700 50  0001 C CNN
	1    10100 2700
	-1   0    0    1   
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC18879
P 10100 3150
F 0 "R?" H 10030 3104 50  0000 R CNN
F 1 "10k" H 10030 3195 50  0000 R CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 10030 3150 50  0001 C CNN
F 3 "~" H 10100 3150 50  0001 C CNN
	1    10100 3150
	-1   0    0    1   
$EndComp
Wire Wire Line
	8750 3350 10100 3350
Wire Wire Line
	10100 3350 10100 3300
Connection ~ 8750 3350
Wire Wire Line
	10100 3000 9450 3000
Connection ~ 9450 3000
Wire Wire Line
	10100 3000 10100 2850
Connection ~ 10100 3000
Wire Wire Line
	10100 2550 10100 2300
Wire Wire Line
	10100 2300 9450 2300
Connection ~ 9450 2300
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC1888A
P 10550 2700
F 0 "R?" H 10480 2654 50  0000 R CNN
F 1 "1.5" H 10480 2745 50  0000 R CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 10480 2700 50  0001 C CNN
F 3 "~" H 10550 2700 50  0001 C CNN
	1    10550 2700
	-1   0    0    1   
$EndComp
Wire Wire Line
	10100 2300 10550 2300
Wire Wire Line
	10550 2300 10550 2550
Connection ~ 10100 2300
$Comp
L BMS-Master-rescue:D_Zener-Device D?
U 1 1 5AC18894
P 8150 2700
F 0 "D?" V 8104 2779 50  0000 L CNN
F 1 "B180-13-F" V 8195 2779 50  0000 L CNN
F 2 "Diodes_SMD:D_SMA" H 8150 2700 50  0001 C CNN
F 3 "~" H 8150 2700 50  0001 C CNN
	1    8150 2700
	0    1    1    0   
$EndComp
Wire Wire Line
	8150 2550 8150 2500
Connection ~ 8150 2500
Wire Wire Line
	8150 2850 8150 3350
Wire Wire Line
	8150 3350 8750 3350
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AC1889F
P 10550 3100
F 0 "C?" H 10665 3146 50  0000 L CNN
F 1 "22u" H 10665 3055 50  0000 L CNN
F 2 "Capacitors_SMD:C_1812_HandSoldering" H 10588 2950 50  0001 C CNN
F 3 "~" H 10550 3100 50  0001 C CNN
	1    10550 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	10550 2950 10550 2850
Wire Wire Line
	10100 3350 10550 3350
Wire Wire Line
	10550 3350 10550 3250
Connection ~ 10100 3350
$Comp
L BMS-Master-rescue:+5V-power #PWR?
U 1 1 5AC188AA
P 10950 2300
F 0 "#PWR?" H 10950 2150 50  0001 C CNN
F 1 "+5V" H 10965 2473 50  0000 C CNN
F 2 "" H 10950 2300 50  0001 C CNN
F 3 "" H 10950 2300 50  0001 C CNN
	1    10950 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	10550 2300 10950 2300
Connection ~ 10550 2300
$Comp
L BMS-Master-rescue:INDUCTOR-pspice L?
U 1 1 5AC188B2
P 8650 2000
F 0 "L?" H 8650 2215 50  0000 C CNN
F 1 "220u" H 8650 2124 50  0000 C CNN
F 2 "" H 8650 2000 50  0001 C CNN
F 3 "" H 8650 2000 50  0001 C CNN
	1    8650 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	8400 2000 8150 2000
Wire Wire Line
	8150 2000 8150 2300
Wire Wire Line
	8900 2000 9450 2000
Wire Wire Line
	9450 2000 9450 2300
$Comp
L BMS-Master-rescue:+3.3V-power #PWR?
U 1 1 5AC188BD
P 14650 2500
F 0 "#PWR?" H 14650 2350 50  0001 C CNN
F 1 "+3.3V" H 14665 2673 50  0000 C CNN
F 2 "" H 14650 2500 50  0001 C CNN
F 3 "" H 14650 2500 50  0001 C CNN
	1    14650 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	13800 2550 14500 2550
Wire Wire Line
	14650 2550 14650 2500
$Comp
L BMS-Master-rescue:+5V-power #PWR?
U 1 1 5AC188C5
P 11550 2300
F 0 "#PWR?" H 11550 2150 50  0001 C CNN
F 1 "+5V" H 11565 2473 50  0000 C CNN
F 2 "" H 11550 2300 50  0001 C CNN
F 3 "" H 11550 2300 50  0001 C CNN
	1    11550 2300
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AC188CB
P 14500 2900
F 0 "C?" H 14615 2946 50  0000 L CNN
F 1 "4.7u" H 14615 2855 50  0000 L CNN
F 2 "" H 14538 2750 50  0001 C CNN
F 3 "~" H 14500 2900 50  0001 C CNN
	1    14500 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	14500 2750 14500 2550
Connection ~ 14500 2550
Wire Wire Line
	14500 2550 14650 2550
Wire Wire Line
	13400 2950 13400 3500
Wire Wire Line
	13400 3500 13950 3500
Wire Wire Line
	14500 3500 14500 3050
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5AC188D8
P 13950 3500
F 0 "#PWR?" H 13950 3250 50  0001 C CNN
F 1 "GND" H 13955 3327 50  0000 C CNN
F 2 "" H 13950 3500 50  0001 C CNN
F 3 "" H 13950 3500 50  0001 C CNN
	1    13950 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	13950 3500 14500 3500
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AC188DF
P 12550 3300
F 0 "C?" H 12665 3346 50  0000 L CNN
F 1 "330p" H 12665 3255 50  0000 L CNN
F 2 "" H 12588 3150 50  0001 C CNN
F 3 "~" H 12550 3300 50  0001 C CNN
	1    12550 3300
	1    0    0    -1  
$EndComp
Connection ~ 13950 3500
Wire Wire Line
	13350 3500 12550 3500
Wire Wire Line
	12550 3500 12550 3450
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC188E9
P 12550 2550
F 0 "R?" H 12480 2504 50  0000 R CNN
F 1 "10k" H 12480 2595 50  0000 R CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 12480 2550 50  0001 C CNN
F 3 "~" H 12550 2550 50  0001 C CNN
	1    12550 2550
	-1   0    0    1   
$EndComp
Wire Wire Line
	13000 2550 12800 2550
Wire Wire Line
	12800 2550 12800 2300
Wire Wire Line
	12800 2300 12550 2300
Wire Wire Line
	12550 2300 12550 2400
Wire Wire Line
	12550 2700 12550 2800
Wire Wire Line
	13000 2800 13000 2650
Wire Wire Line
	12550 2800 12550 3150
Connection ~ 12550 2800
Wire Wire Line
	12550 2800 13000 2800
Connection ~ 12550 2300
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AC188FA
P 12100 3000
F 0 "C?" H 12215 3046 50  0000 L CNN
F 1 "2.2u" H 12215 2955 50  0000 L CNN
F 2 "" H 12138 2850 50  0001 C CNN
F 3 "~" H 12100 3000 50  0001 C CNN
	1    12100 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	11550 2300 12100 2300
Wire Wire Line
	12100 2850 12100 2300
Connection ~ 12100 2300
Wire Wire Line
	12100 2300 12550 2300
Wire Wire Line
	12100 3150 12100 3500
Wire Wire Line
	12100 3500 12550 3500
Connection ~ 12550 3500
Text HLabel 5250 4350 0    50   Input ~ 0
PushButton
Text HLabel 5250 4250 0    50   Input ~ 0
PowerButton
Text HLabel 5250 4150 0    50   Input ~ 0
PowerEnable
Wire Wire Line
	1100 1650 2550 1650
Wire Wire Line
	1100 2200 1300 2200
Wire Wire Line
	850  2200 1100 2200
Connection ~ 1100 2200
Wire Wire Line
	1100 2200 1100 1650
$Comp
L BMS-Master-rescue:Jumper_NC_Dual-Device JP?
U 1 1 5AB703CB
P 2550 1400
F 0 "JP?" V 2596 1502 50  0000 L CNN
F 1 "Jumper_NC_Dual" V 2505 1502 50  0000 L CNN
F 2 "" H 2550 1400 50  0001 C CNN
F 3 "~" H 2550 1400 50  0001 C CNN
	1    2550 1400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2650 1400 4900 1400
Wire Wire Line
	4900 1400 4900 1650
Connection ~ 4900 1650
Text HLabel 1800 800  0    50   Input ~ 0
+12V
Wire Wire Line
	1800 800  2550 800 
Wire Wire Line
	2550 800  2550 1150
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC6BB27
P 22650 6300
F 0 "R?" H 22720 6346 50  0000 L CNN
F 1 "1M" H 22720 6255 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" V 22580 6300 50  0001 C CNN
F 3 "~" H 22650 6300 50  0001 C CNN
	1    22650 6300
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC6BB2E
P 22650 6700
F 0 "R?" H 22720 6746 50  0000 L CNN
F 1 "1M" H 22720 6655 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" V 22580 6700 50  0001 C CNN
F 3 "~" H 22650 6700 50  0001 C CNN
	1    22650 6700
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC6BB35
P 22650 7100
F 0 "R?" H 22720 7146 50  0000 L CNN
F 1 "1M" H 22720 7055 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" V 22580 7100 50  0001 C CNN
F 3 "~" H 22650 7100 50  0001 C CNN
	1    22650 7100
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC6BB3C
P 22650 7500
F 0 "R?" H 22720 7546 50  0000 L CNN
F 1 "1M" H 22720 7455 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" V 22580 7500 50  0001 C CNN
F 3 "~" H 22650 7500 50  0001 C CNN
	1    22650 7500
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC6BB43
P 22650 8000
F 0 "R?" H 22720 8046 50  0000 L CNN
F 1 "100k" H 22720 7955 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" V 22580 8000 50  0001 C CNN
F 3 "~" H 22650 8000 50  0001 C CNN
	1    22650 8000
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC6BB4A
P 22650 5900
F 0 "R?" H 22720 5946 50  0000 L CNN
F 1 "1M" H 22720 5855 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" V 22580 5900 50  0001 C CNN
F 3 "~" H 22650 5900 50  0001 C CNN
	1    22650 5900
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC6BB51
P 22650 5450
F 0 "R?" H 22720 5496 50  0000 L CNN
F 1 "1M" H 22720 5405 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" V 22580 5450 50  0001 C CNN
F 3 "~" H 22650 5450 50  0001 C CNN
	1    22650 5450
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:D_Zener-Device D?
U 1 1 5AC6BB58
P 23000 8000
F 0 "D?" V 22954 8079 50  0000 L CNN
F 1 "5V zener" V 23045 8079 50  0000 L CNN
F 2 "" H 23000 8000 50  0001 C CNN
F 3 "~" H 23000 8000 50  0001 C CNN
	1    23000 8000
	0    1    1    0   
$EndComp
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5AC6BB5F
P 22750 8250
F 0 "#PWR?" H 22750 8000 50  0001 C CNN
F 1 "GND" H 22755 8077 50  0000 C CNN
F 2 "" H 22750 8250 50  0001 C CNN
F 3 "" H 22750 8250 50  0001 C CNN
	1    22750 8250
	1    0    0    -1  
$EndComp
Wire Wire Line
	22650 8250 22750 8250
Wire Wire Line
	23000 8250 22750 8250
Connection ~ 22750 8250
Wire Wire Line
	23000 7750 22650 7750
Wire Wire Line
	22650 7750 22650 7650
Connection ~ 22650 7750
Wire Wire Line
	22650 7350 22650 7250
Wire Wire Line
	22650 6950 22650 6850
Wire Wire Line
	22650 6550 22650 6450
Wire Wire Line
	22650 6150 22650 6050
Wire Wire Line
	22650 5150 22650 5300
Wire Wire Line
	22650 5600 22650 5750
Wire Wire Line
	22650 8150 22650 8250
Wire Wire Line
	22650 7750 22650 7850
Wire Wire Line
	23000 8150 23000 8250
Wire Wire Line
	23000 7750 23000 7850
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC6BB7C
P 21900 5200
F 0 "R?" H 21970 5246 50  0000 L CNN
F 1 "1M" H 21970 5155 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" V 21830 5200 50  0001 C CNN
F 3 "~" H 21900 5200 50  0001 C CNN
	1    21900 5200
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC6BB83
P 21900 5600
F 0 "R?" H 21970 5646 50  0000 L CNN
F 1 "1M" H 21970 5555 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" V 21830 5600 50  0001 C CNN
F 3 "~" H 21900 5600 50  0001 C CNN
	1    21900 5600
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC6BB8A
P 21900 6000
F 0 "R?" H 21970 6046 50  0000 L CNN
F 1 "1M" H 21970 5955 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" V 21830 6000 50  0001 C CNN
F 3 "~" H 21900 6000 50  0001 C CNN
	1    21900 6000
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC6BB91
P 21900 6400
F 0 "R?" H 21970 6446 50  0000 L CNN
F 1 "1M" H 21970 6355 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" V 21830 6400 50  0001 C CNN
F 3 "~" H 21900 6400 50  0001 C CNN
	1    21900 6400
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC6BB98
P 21900 4800
F 0 "R?" H 21970 4846 50  0000 L CNN
F 1 "1M" H 21970 4755 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" V 21830 4800 50  0001 C CNN
F 3 "~" H 21900 4800 50  0001 C CNN
	1    21900 4800
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC6BB9F
P 21900 4350
F 0 "R?" H 21970 4396 50  0000 L CNN
F 1 "1M" H 21970 4305 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" V 21830 4350 50  0001 C CNN
F 3 "~" H 21900 4350 50  0001 C CNN
	1    21900 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	21900 6250 21900 6150
Wire Wire Line
	21900 5850 21900 5750
Wire Wire Line
	21900 5450 21900 5350
Wire Wire Line
	21900 5050 21900 4950
Wire Wire Line
	21900 4500 21900 4650
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AC6BBB3
P 21700 7200
F 0 "R?" H 21770 7246 50  0000 L CNN
F 1 "100k" H 21770 7155 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" V 21630 7200 50  0001 C CNN
F 3 "~" H 21700 7200 50  0001 C CNN
	1    21700 7200
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:D_Zener-Device D?
U 1 1 5AC6BBBA
P 22050 7200
F 0 "D?" V 22004 7279 50  0000 L CNN
F 1 "5V zener" V 22095 7279 50  0000 L CNN
F 2 "" H 22050 7200 50  0001 C CNN
F 3 "~" H 22050 7200 50  0001 C CNN
	1    22050 7200
	0    1    1    0   
$EndComp
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5AC6BBC1
P 21800 7450
F 0 "#PWR?" H 21800 7200 50  0001 C CNN
F 1 "GND" H 21805 7277 50  0000 C CNN
F 2 "" H 21800 7450 50  0001 C CNN
F 3 "" H 21800 7450 50  0001 C CNN
	1    21800 7450
	1    0    0    -1  
$EndComp
Wire Wire Line
	21700 7450 21800 7450
Wire Wire Line
	22050 7450 21800 7450
Connection ~ 21800 7450
Wire Wire Line
	21700 7350 21700 7450
Wire Wire Line
	22050 7350 22050 7450
Wire Wire Line
	21700 7050 21900 7050
Connection ~ 21900 7050
Wire Wire Line
	21900 7050 22050 7050
Wire Wire Line
	21900 6550 21900 7050
Text HLabel 20600 7950 0    50   Input ~ 0
V+
Text HLabel 20250 6900 0    50   Input ~ 0
ChargeDetect
Text HLabel 21900 3800 1    50   Input ~ 0
Charge+
Wire Wire Line
	21900 3800 21900 4200
Text HLabel 22650 5150 1    50   Input ~ 0
Bat+
$EndSCHEMATC
