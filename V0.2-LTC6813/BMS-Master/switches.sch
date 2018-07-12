EESchema Schematic File Version 4
LIBS:BMS-Master-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 8 25
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 2300 2350 0    50   Input ~ 0
DischargeEnable
Text HLabel 2050 6000 0    50   Input ~ 0
ChargeEnable
Text HLabel 2300 4550 0    50   Input ~ 0
PrechargeEnable
Text HLabel 6300 2050 2    50   Input ~ 0
Discharge
Text HLabel 6600 4250 2    50   Input ~ 0
Precharge
Text HLabel 6600 5700 2    50   Input ~ 0
Charge
$Comp
L BMS-Master-rescue:SFH617A-2X009T-Isolator U?
U 1 1 5ACC44C1
P 5150 1950
F 0 "U?" H 5150 2275 50  0000 C CNN
F 1 "SFH617A-2X009T" H 5150 2184 50  0000 C CNN
F 2 "Package_DIP:SMDIP-4_W7.62mm" H 4950 1750 50  0001 L CIN
F 3 "http://www.vishay.com/docs/83740/sfh617a.pdf" H 5150 1950 50  0001 L CNN
	1    5150 1950
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:SFH617A-2X009T-Isolator U?
U 1 1 5ACC48D9
P 5150 4150
F 0 "U?" H 5150 4475 50  0000 C CNN
F 1 "SFH617A-2X009T" H 5150 4384 50  0000 C CNN
F 2 "Package_DIP:SMDIP-4_W7.62mm" H 4950 3950 50  0001 L CIN
F 3 "http://www.vishay.com/docs/83740/sfh617a.pdf" H 5150 4150 50  0001 L CNN
	1    5150 4150
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:SFH617A-2X009T-Isolator U?
U 1 1 5ACC490F
P 5150 5600
F 0 "U?" H 5150 5925 50  0000 C CNN
F 1 "SFH617A-2X009T" H 5150 5834 50  0000 C CNN
F 2 "Package_DIP:SMDIP-4_W7.62mm" H 4950 5400 50  0001 L CIN
F 3 "http://www.vishay.com/docs/83740/sfh617a.pdf" H 5150 5600 50  0001 L CNN
	1    5150 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 5700 6600 5700
Wire Wire Line
	5450 4250 6600 4250
Wire Wire Line
	5450 2050 6300 2050
Text HLabel 6550 1300 2    50   Input ~ 0
+12V
Wire Wire Line
	6550 1300 5850 1300
Wire Wire Line
	5850 1300 5850 1850
Wire Wire Line
	5850 1850 5450 1850
Wire Wire Line
	5850 1850 5850 4050
Wire Wire Line
	5850 4050 5450 4050
Connection ~ 5850 1850
Wire Wire Line
	5850 4050 5850 5500
Wire Wire Line
	5850 5500 5450 5500
Connection ~ 5850 4050
$Comp
L BMS-Master-rescue:+3.3V-power #PWR?
U 1 1 5ACFE468
P 4550 1500
F 0 "#PWR?" H 4550 1350 50  0001 C CNN
F 1 "+3.3V" H 4565 1673 50  0000 C CNN
F 2 "" H 4550 1500 50  0001 C CNN
F 3 "" H 4550 1500 50  0001 C CNN
	1    4550 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 1500 4550 1850
Wire Wire Line
	4550 1850 4850 1850
$Comp
L BMS-Master-rescue:+3.3V-power #PWR?
U 1 1 5ACFE5C5
P 4600 3850
F 0 "#PWR?" H 4600 3700 50  0001 C CNN
F 1 "+3.3V" H 4615 4023 50  0000 C CNN
F 2 "" H 4600 3850 50  0001 C CNN
F 3 "" H 4600 3850 50  0001 C CNN
	1    4600 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 3850 4600 4050
Wire Wire Line
	4600 4050 4850 4050
$Comp
L BMS-Master-rescue:+3.3V-power #PWR?
U 1 1 5ACFE760
P 4550 5250
F 0 "#PWR?" H 4550 5100 50  0001 C CNN
F 1 "+3.3V" H 4565 5423 50  0000 C CNN
F 2 "" H 4550 5250 50  0001 C CNN
F 3 "" H 4550 5250 50  0001 C CNN
	1    4550 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 5250 4550 5500
Wire Wire Line
	4550 5500 4850 5500
$Comp
L BMS-Master-rescue:BSS138-Transistor_FET Q?
U 1 1 5AD010B8
P 3900 2350
F 0 "Q?" H 4105 2396 50  0000 L CNN
F 1 "BSS138" H 4105 2305 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 4100 2275 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/BS/BSS138.pdf" H 3900 2350 50  0001 L CNN
	1    3900 2350
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AD011B7
P 4350 2050
F 0 "R?" V 4557 2050 50  0000 C CNN
F 1 "430" V 4466 2050 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4280 2050 50  0001 C CNN
F 3 "~" H 4350 2050 50  0001 C CNN
	1    4350 2050
	0    -1   -1   0   
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AD01241
P 2950 2350
F 0 "R?" V 3157 2350 50  0000 C CNN
F 1 "10" V 3066 2350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2880 2350 50  0001 C CNN
F 3 "~" H 2950 2350 50  0001 C CNN
	1    2950 2350
	0    -1   -1   0   
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AD01290
P 3400 2550
F 0 "R?" H 3470 2596 50  0000 L CNN
F 1 "1k" H 3470 2505 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3330 2550 50  0001 C CNN
F 3 "~" H 3400 2550 50  0001 C CNN
	1    3400 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 2350 3400 2350
Wire Wire Line
	3400 2350 3400 2400
Wire Wire Line
	3400 2350 3100 2350
Connection ~ 3400 2350
Wire Wire Line
	4000 2050 4200 2050
Wire Wire Line
	2300 2350 2800 2350
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5AD03C7A
P 3750 2900
F 0 "#PWR?" H 3750 2650 50  0001 C CNN
F 1 "GND" H 3755 2727 50  0000 C CNN
F 2 "" H 3750 2900 50  0001 C CNN
F 3 "" H 3750 2900 50  0001 C CNN
	1    3750 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 2700 3400 2900
Wire Wire Line
	3400 2900 3750 2900
Wire Wire Line
	3750 2900 4000 2900
Wire Wire Line
	4000 2900 4000 2550
Connection ~ 3750 2900
Wire Wire Line
	4500 2050 4850 2050
Wire Wire Line
	4000 2050 4000 2150
$Comp
L BMS-Master-rescue:BSS138-Transistor_FET Q?
U 1 1 5AD06F84
P 3900 4550
F 0 "Q?" H 4105 4596 50  0000 L CNN
F 1 "BSS138" H 4105 4505 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 4100 4475 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/BS/BSS138.pdf" H 3900 4550 50  0001 L CNN
	1    3900 4550
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AD06F8A
P 4350 4250
F 0 "R?" V 4557 4250 50  0000 C CNN
F 1 "430" V 4466 4250 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4280 4250 50  0001 C CNN
F 3 "~" H 4350 4250 50  0001 C CNN
	1    4350 4250
	0    -1   -1   0   
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AD06F90
P 2950 4550
F 0 "R?" V 3157 4550 50  0000 C CNN
F 1 "10" V 3066 4550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2880 4550 50  0001 C CNN
F 3 "~" H 2950 4550 50  0001 C CNN
	1    2950 4550
	0    -1   -1   0   
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AD06F96
P 3400 4750
F 0 "R?" H 3470 4796 50  0000 L CNN
F 1 "1k" H 3470 4705 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3330 4750 50  0001 C CNN
F 3 "~" H 3400 4750 50  0001 C CNN
	1    3400 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 4550 3400 4550
Wire Wire Line
	3400 4550 3400 4600
Wire Wire Line
	3400 4550 3100 4550
Connection ~ 3400 4550
Wire Wire Line
	4000 4250 4200 4250
Wire Wire Line
	2300 4550 2800 4550
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5AD06FA2
P 3750 5100
F 0 "#PWR?" H 3750 4850 50  0001 C CNN
F 1 "GND" H 3755 4927 50  0000 C CNN
F 2 "" H 3750 5100 50  0001 C CNN
F 3 "" H 3750 5100 50  0001 C CNN
	1    3750 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 4900 3400 5100
Wire Wire Line
	3400 5100 3750 5100
Wire Wire Line
	3750 5100 4000 5100
Wire Wire Line
	4000 5100 4000 4750
Connection ~ 3750 5100
Wire Wire Line
	4500 4250 4850 4250
Wire Wire Line
	4000 4250 4000 4350
$Comp
L BMS-Master-rescue:BSS138-Transistor_FET Q?
U 1 1 5AD07A68
P 4150 6000
F 0 "Q?" H 4355 6046 50  0000 L CNN
F 1 "BSS138" H 4355 5955 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 4350 5925 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/BS/BSS138.pdf" H 4150 6000 50  0001 L CNN
	1    4150 6000
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AD07A6E
P 4600 5700
F 0 "R?" V 4807 5700 50  0000 C CNN
F 1 "430" V 4716 5700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4530 5700 50  0001 C CNN
F 3 "~" H 4600 5700 50  0001 C CNN
	1    4600 5700
	0    -1   -1   0   
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AD07A74
P 3200 6000
F 0 "R?" V 3407 6000 50  0000 C CNN
F 1 "10" V 3316 6000 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3130 6000 50  0001 C CNN
F 3 "~" H 3200 6000 50  0001 C CNN
	1    3200 6000
	0    -1   -1   0   
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AD07A7A
P 3650 6200
F 0 "R?" H 3720 6246 50  0000 L CNN
F 1 "1k" H 3720 6155 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3580 6200 50  0001 C CNN
F 3 "~" H 3650 6200 50  0001 C CNN
	1    3650 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 6000 3650 6000
Wire Wire Line
	3650 6000 3650 6050
Wire Wire Line
	3650 6000 3350 6000
Connection ~ 3650 6000
Wire Wire Line
	4250 5700 4450 5700
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5AD07A86
P 4000 6550
F 0 "#PWR?" H 4000 6300 50  0001 C CNN
F 1 "GND" H 4005 6377 50  0000 C CNN
F 2 "" H 4000 6550 50  0001 C CNN
F 3 "" H 4000 6550 50  0001 C CNN
	1    4000 6550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 6350 3650 6550
Wire Wire Line
	3650 6550 4000 6550
Wire Wire Line
	4000 6550 4250 6550
Wire Wire Line
	4250 6550 4250 6200
Connection ~ 4000 6550
Wire Wire Line
	4250 5700 4250 5800
Wire Wire Line
	4850 5700 4750 5700
Wire Wire Line
	2050 6000 3050 6000
$EndSCHEMATC
