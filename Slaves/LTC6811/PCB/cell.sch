EESchema Schematic File Version 4
LIBS:LTC6811-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 13
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
L Device:R R?
U 1 1 5C4F5B30
P 4650 2700
AR Path="/5C4F59F4/5C4F5B30" Ref="R?"  Part="1" 
AR Path="/5C4FCBE0/5C4F5B30" Ref="R?"  Part="1" 
AR Path="/5C4FCED2/5C4F5B30" Ref="R?"  Part="1" 
AR Path="/5C4FCED8/5C4F5B30" Ref="R?"  Part="1" 
AR Path="/5C4FD32A/5C4F5B30" Ref="R?"  Part="1" 
AR Path="/5C4FD330/5C4F5B30" Ref="R?"  Part="1" 
AR Path="/5C4FD336/5C4F5B30" Ref="R?"  Part="1" 
AR Path="/5C4FD33C/5C4F5B30" Ref="R?"  Part="1" 
AR Path="/5C4FFA25/5C4F5B30" Ref="R?"  Part="1" 
AR Path="/5C4FFA2B/5C4F5B30" Ref="R?"  Part="1" 
AR Path="/5C4FFA31/5C4F5B30" Ref="R?"  Part="1" 
AR Path="/5C4FFA37/5C4F5B30" Ref="R?"  Part="1" 
AR Path="/5C4FFA3D/5C4F5B30" Ref="R?"  Part="1" 
AR Path="/5C4FFA43/5C4F5B30" Ref="R?"  Part="1" 
F 0 "R?" V 4443 2700 50  0000 C CNN
F 1 "100" V 4534 2700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4580 2700 50  0001 C CNN
F 3 "~" H 4650 2700 50  0001 C CNN
	1    4650 2700
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 5C4F5DDD
P 4650 3350
AR Path="/5C4F59F4/5C4F5DDD" Ref="R?"  Part="1" 
AR Path="/5C4FCBE0/5C4F5DDD" Ref="R?"  Part="1" 
AR Path="/5C4FCED2/5C4F5DDD" Ref="R?"  Part="1" 
AR Path="/5C4FCED8/5C4F5DDD" Ref="R?"  Part="1" 
AR Path="/5C4FD32A/5C4F5DDD" Ref="R?"  Part="1" 
AR Path="/5C4FD330/5C4F5DDD" Ref="R?"  Part="1" 
AR Path="/5C4FD336/5C4F5DDD" Ref="R?"  Part="1" 
AR Path="/5C4FD33C/5C4F5DDD" Ref="R?"  Part="1" 
AR Path="/5C4FFA25/5C4F5DDD" Ref="R?"  Part="1" 
AR Path="/5C4FFA2B/5C4F5DDD" Ref="R?"  Part="1" 
AR Path="/5C4FFA31/5C4F5DDD" Ref="R?"  Part="1" 
AR Path="/5C4FFA37/5C4F5DDD" Ref="R?"  Part="1" 
AR Path="/5C4FFA3D/5C4F5DDD" Ref="R?"  Part="1" 
AR Path="/5C4FFA43/5C4F5DDD" Ref="R?"  Part="1" 
F 0 "R?" V 4443 3350 50  0000 C CNN
F 1 "3.3k" V 4534 3350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4580 3350 50  0001 C CNN
F 3 "~" H 4650 3350 50  0001 C CNN
	1    4650 3350
	0    1    1    0   
$EndComp
$Comp
L Device:Q_PMOS_GSD Q?
U 1 1 5C4F756B
P 3900 3350
AR Path="/5C4F59F4/5C4F756B" Ref="Q?"  Part="1" 
AR Path="/5C4FCBE0/5C4F756B" Ref="Q?"  Part="1" 
AR Path="/5C4FCED2/5C4F756B" Ref="Q?"  Part="1" 
AR Path="/5C4FCED8/5C4F756B" Ref="Q?"  Part="1" 
AR Path="/5C4FD32A/5C4F756B" Ref="Q?"  Part="1" 
AR Path="/5C4FD330/5C4F756B" Ref="Q?"  Part="1" 
AR Path="/5C4FD336/5C4F756B" Ref="Q?"  Part="1" 
AR Path="/5C4FD33C/5C4F756B" Ref="Q?"  Part="1" 
AR Path="/5C4FFA25/5C4F756B" Ref="Q?"  Part="1" 
AR Path="/5C4FFA2B/5C4F756B" Ref="Q?"  Part="1" 
AR Path="/5C4FFA31/5C4F756B" Ref="Q?"  Part="1" 
AR Path="/5C4FFA37/5C4F756B" Ref="Q?"  Part="1" 
AR Path="/5C4FFA3D/5C4F756B" Ref="Q?"  Part="1" 
AR Path="/5C4FFA43/5C4F756B" Ref="Q?"  Part="1" 
F 0 "Q?" H 4105 3304 50  0000 L CNN
F 1 "TSM2323" H 4105 3395 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 4100 3450 50  0001 C CNN
F 3 "~" H 3900 3350 50  0001 C CNN
	1    3900 3350
	-1   0    0    1   
$EndComp
Wire Wire Line
	4100 3350 4500 3350
Wire Wire Line
	4500 2700 3800 2700
Wire Wire Line
	3800 2700 3800 3150
$Comp
L Device:R R?
U 1 1 5C4F932B
P 3800 4000
AR Path="/5C4F59F4/5C4F932B" Ref="R?"  Part="1" 
AR Path="/5C4FCBE0/5C4F932B" Ref="R?"  Part="1" 
AR Path="/5C4FCED2/5C4F932B" Ref="R?"  Part="1" 
AR Path="/5C4FCED8/5C4F932B" Ref="R?"  Part="1" 
AR Path="/5C4FD32A/5C4F932B" Ref="R?"  Part="1" 
AR Path="/5C4FD330/5C4F932B" Ref="R?"  Part="1" 
AR Path="/5C4FD336/5C4F932B" Ref="R?"  Part="1" 
AR Path="/5C4FD33C/5C4F932B" Ref="R?"  Part="1" 
AR Path="/5C4FFA25/5C4F932B" Ref="R?"  Part="1" 
AR Path="/5C4FFA2B/5C4F932B" Ref="R?"  Part="1" 
AR Path="/5C4FFA31/5C4F932B" Ref="R?"  Part="1" 
AR Path="/5C4FFA37/5C4F932B" Ref="R?"  Part="1" 
AR Path="/5C4FFA3D/5C4F932B" Ref="R?"  Part="1" 
AR Path="/5C4FFA43/5C4F932B" Ref="R?"  Part="1" 
F 0 "R?" H 3730 3954 50  0000 R CNN
F 1 "15" H 3730 4045 50  0000 R CNN
F 2 "Resistors_SMD:R_2512_HandSoldering" V 3730 4000 50  0001 C CNN
F 3 "~" H 3800 4000 50  0001 C CNN
	1    3800 4000
	-1   0    0    1   
$EndComp
Wire Wire Line
	3800 3550 3800 3850
Text HLabel 5100 3350 2    50   Input ~ 0
S
Wire Wire Line
	4800 3350 5100 3350
Text HLabel 5650 2700 2    50   Input ~ 0
C
Wire Wire Line
	4800 2700 5400 2700
Wire Wire Line
	5400 2700 5400 3800
Connection ~ 5400 2700
Wire Wire Line
	5400 2700 5650 2700
$Comp
L Device:C C?
U 1 1 5C4F9945
P 5400 3950
AR Path="/5C4F59F4/5C4F9945" Ref="C?"  Part="1" 
AR Path="/5C4FCBE0/5C4F9945" Ref="C?"  Part="1" 
AR Path="/5C4FCED2/5C4F9945" Ref="C?"  Part="1" 
AR Path="/5C4FCED8/5C4F9945" Ref="C?"  Part="1" 
AR Path="/5C4FD32A/5C4F9945" Ref="C?"  Part="1" 
AR Path="/5C4FD330/5C4F9945" Ref="C?"  Part="1" 
AR Path="/5C4FD336/5C4F9945" Ref="C?"  Part="1" 
AR Path="/5C4FD33C/5C4F9945" Ref="C?"  Part="1" 
AR Path="/5C4FFA25/5C4F9945" Ref="C?"  Part="1" 
AR Path="/5C4FFA2B/5C4F9945" Ref="C?"  Part="1" 
AR Path="/5C4FFA31/5C4F9945" Ref="C?"  Part="1" 
AR Path="/5C4FFA37/5C4F9945" Ref="C?"  Part="1" 
AR Path="/5C4FFA3D/5C4F9945" Ref="C?"  Part="1" 
AR Path="/5C4FFA43/5C4F9945" Ref="C?"  Part="1" 
F 0 "C?" H 5515 3996 50  0000 L CNN
F 1 "100n" H 5515 3905 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5438 3800 50  0001 C CNN
F 3 "~" H 5400 3950 50  0001 C CNN
	1    5400 3950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5C4FAC2C
P 5400 4200
AR Path="/5C4F59F4/5C4FAC2C" Ref="#PWR?"  Part="1" 
AR Path="/5C4FCBE0/5C4FAC2C" Ref="#PWR?"  Part="1" 
AR Path="/5C4FCED2/5C4FAC2C" Ref="#PWR?"  Part="1" 
AR Path="/5C4FCED8/5C4FAC2C" Ref="#PWR?"  Part="1" 
AR Path="/5C4FD32A/5C4FAC2C" Ref="#PWR?"  Part="1" 
AR Path="/5C4FD330/5C4FAC2C" Ref="#PWR?"  Part="1" 
AR Path="/5C4FD336/5C4FAC2C" Ref="#PWR?"  Part="1" 
AR Path="/5C4FD33C/5C4FAC2C" Ref="#PWR?"  Part="1" 
AR Path="/5C4FFA25/5C4FAC2C" Ref="#PWR?"  Part="1" 
AR Path="/5C4FFA2B/5C4FAC2C" Ref="#PWR?"  Part="1" 
AR Path="/5C4FFA31/5C4FAC2C" Ref="#PWR?"  Part="1" 
AR Path="/5C4FFA37/5C4FAC2C" Ref="#PWR?"  Part="1" 
AR Path="/5C4FFA3D/5C4FAC2C" Ref="#PWR?"  Part="1" 
AR Path="/5C4FFA43/5C4FAC2C" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5400 3950 50  0001 C CNN
F 1 "GND" H 5405 4027 50  0000 C CNN
F 2 "" H 5400 4200 50  0001 C CNN
F 3 "" H 5400 4200 50  0001 C CNN
	1    5400 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 4100 5400 4200
Text HLabel 2800 2700 0    50   Input ~ 0
C+
Wire Wire Line
	2800 2700 3800 2700
Connection ~ 3800 2700
Text HLabel 2850 4350 0    50   Input ~ 0
C-
Wire Wire Line
	3800 4150 3800 4350
Wire Wire Line
	3800 4350 2850 4350
$EndSCHEMATC
