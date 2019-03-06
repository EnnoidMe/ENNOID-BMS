EESchema Schematic File Version 4
LIBS:LTC6811-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 13 13
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
L Device:R R13
U 1 1 5C4F5B30
P 4650 2700
AR Path="/5C4F59F4/5C4F5B30" Ref="R13"  Part="1" 
AR Path="/5C4FCBE0/5C4F5B30" Ref="R16"  Part="1" 
AR Path="/5C4FCED2/5C4F5B30" Ref="R19"  Part="1" 
AR Path="/5C4FCED8/5C4F5B30" Ref="R22"  Part="1" 
AR Path="/5C4FD32A/5C4F5B30" Ref="R25"  Part="1" 
AR Path="/5C4FD330/5C4F5B30" Ref="R28"  Part="1" 
AR Path="/5C4FD336/5C4F5B30" Ref="R31"  Part="1" 
AR Path="/5C4FD33C/5C4F5B30" Ref="R34"  Part="1" 
AR Path="/5C4FFA25/5C4F5B30" Ref="R37"  Part="1" 
AR Path="/5C4FFA2B/5C4F5B30" Ref="R40"  Part="1" 
AR Path="/5C4FFA31/5C4F5B30" Ref="R43"  Part="1" 
AR Path="/5C4FFA37/5C4F5B30" Ref="R46"  Part="1" 
AR Path="/5C4FFA3D/5C4F5B30" Ref="R?"  Part="1" 
AR Path="/5C4FFA43/5C4F5B30" Ref="R?"  Part="1" 
F 0 "R46" V 4443 2700 50  0000 C CNN
F 1 "100" V 4534 2700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4580 2700 50  0001 C CNN
F 3 "~" H 4650 2700 50  0001 C CNN
	1    4650 2700
	0    1    1    0   
$EndComp
$Comp
L Device:R R14
U 1 1 5C4F5DDD
P 4650 3350
AR Path="/5C4F59F4/5C4F5DDD" Ref="R14"  Part="1" 
AR Path="/5C4FCBE0/5C4F5DDD" Ref="R17"  Part="1" 
AR Path="/5C4FCED2/5C4F5DDD" Ref="R20"  Part="1" 
AR Path="/5C4FCED8/5C4F5DDD" Ref="R23"  Part="1" 
AR Path="/5C4FD32A/5C4F5DDD" Ref="R26"  Part="1" 
AR Path="/5C4FD330/5C4F5DDD" Ref="R29"  Part="1" 
AR Path="/5C4FD336/5C4F5DDD" Ref="R32"  Part="1" 
AR Path="/5C4FD33C/5C4F5DDD" Ref="R35"  Part="1" 
AR Path="/5C4FFA25/5C4F5DDD" Ref="R38"  Part="1" 
AR Path="/5C4FFA2B/5C4F5DDD" Ref="R41"  Part="1" 
AR Path="/5C4FFA31/5C4F5DDD" Ref="R44"  Part="1" 
AR Path="/5C4FFA37/5C4F5DDD" Ref="R47"  Part="1" 
AR Path="/5C4FFA3D/5C4F5DDD" Ref="R?"  Part="1" 
AR Path="/5C4FFA43/5C4F5DDD" Ref="R?"  Part="1" 
F 0 "R47" V 4443 3350 50  0000 C CNN
F 1 "3.3k" V 4534 3350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4580 3350 50  0001 C CNN
F 3 "~" H 4650 3350 50  0001 C CNN
	1    4650 3350
	0    1    1    0   
$EndComp
$Comp
L Device:Q_PMOS_GSD Q2
U 1 1 5C4F756B
P 3900 3350
AR Path="/5C4F59F4/5C4F756B" Ref="Q2"  Part="1" 
AR Path="/5C4FCBE0/5C4F756B" Ref="Q3"  Part="1" 
AR Path="/5C4FCED2/5C4F756B" Ref="Q4"  Part="1" 
AR Path="/5C4FCED8/5C4F756B" Ref="Q5"  Part="1" 
AR Path="/5C4FD32A/5C4F756B" Ref="Q6"  Part="1" 
AR Path="/5C4FD330/5C4F756B" Ref="Q7"  Part="1" 
AR Path="/5C4FD336/5C4F756B" Ref="Q8"  Part="1" 
AR Path="/5C4FD33C/5C4F756B" Ref="Q9"  Part="1" 
AR Path="/5C4FFA25/5C4F756B" Ref="Q10"  Part="1" 
AR Path="/5C4FFA2B/5C4F756B" Ref="Q11"  Part="1" 
AR Path="/5C4FFA31/5C4F756B" Ref="Q12"  Part="1" 
AR Path="/5C4FFA37/5C4F756B" Ref="Q13"  Part="1" 
AR Path="/5C4FFA3D/5C4F756B" Ref="Q?"  Part="1" 
AR Path="/5C4FFA43/5C4F756B" Ref="Q?"  Part="1" 
F 0 "Q13" H 4105 3304 50  0000 L CNN
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
L Device:R R12
U 1 1 5C4F932B
P 3800 4000
AR Path="/5C4F59F4/5C4F932B" Ref="R12"  Part="1" 
AR Path="/5C4FCBE0/5C4F932B" Ref="R15"  Part="1" 
AR Path="/5C4FCED2/5C4F932B" Ref="R18"  Part="1" 
AR Path="/5C4FCED8/5C4F932B" Ref="R21"  Part="1" 
AR Path="/5C4FD32A/5C4F932B" Ref="R24"  Part="1" 
AR Path="/5C4FD330/5C4F932B" Ref="R27"  Part="1" 
AR Path="/5C4FD336/5C4F932B" Ref="R30"  Part="1" 
AR Path="/5C4FD33C/5C4F932B" Ref="R33"  Part="1" 
AR Path="/5C4FFA25/5C4F932B" Ref="R36"  Part="1" 
AR Path="/5C4FFA2B/5C4F932B" Ref="R39"  Part="1" 
AR Path="/5C4FFA31/5C4F932B" Ref="R42"  Part="1" 
AR Path="/5C4FFA37/5C4F932B" Ref="R45"  Part="1" 
AR Path="/5C4FFA3D/5C4F932B" Ref="R?"  Part="1" 
AR Path="/5C4FFA43/5C4F932B" Ref="R?"  Part="1" 
F 0 "R45" H 3730 3954 50  0000 R CNN
F 1 "33" H 3730 4045 50  0000 R CNN
F 2 "Resistors_SMD:R_2512" V 3730 4000 50  0001 C CNN
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
Text HLabel 6500 2700 2    50   Input ~ 0
C
Wire Wire Line
	4800 2700 5400 2700
Wire Wire Line
	5400 2700 5400 3800
Connection ~ 5400 2700
Wire Wire Line
	5400 2700 6000 2700
$Comp
L Device:C C13
U 1 1 5C4F9945
P 5400 3950
AR Path="/5C4F59F4/5C4F9945" Ref="C13"  Part="1" 
AR Path="/5C4FCBE0/5C4F9945" Ref="C14"  Part="1" 
AR Path="/5C4FCED2/5C4F9945" Ref="C15"  Part="1" 
AR Path="/5C4FCED8/5C4F9945" Ref="C16"  Part="1" 
AR Path="/5C4FD32A/5C4F9945" Ref="C17"  Part="1" 
AR Path="/5C4FD330/5C4F9945" Ref="C18"  Part="1" 
AR Path="/5C4FD336/5C4F9945" Ref="C19"  Part="1" 
AR Path="/5C4FD33C/5C4F9945" Ref="C20"  Part="1" 
AR Path="/5C4FFA25/5C4F9945" Ref="C21"  Part="1" 
AR Path="/5C4FFA2B/5C4F9945" Ref="C22"  Part="1" 
AR Path="/5C4FFA31/5C4F9945" Ref="C23"  Part="1" 
AR Path="/5C4FFA37/5C4F9945" Ref="C24"  Part="1" 
AR Path="/5C4FFA3D/5C4F9945" Ref="C?"  Part="1" 
AR Path="/5C4FFA43/5C4F9945" Ref="C?"  Part="1" 
F 0 "C24" H 5515 3996 50  0000 L CNN
F 1 "100n" H 5515 3905 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5438 3800 50  0001 C CNN
F 3 "~" H 5400 3950 50  0001 C CNN
	1    5400 3950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0109
U 1 1 5C4FAC2C
P 5400 4200
AR Path="/5C4F59F4/5C4FAC2C" Ref="#PWR0109"  Part="1" 
AR Path="/5C4FCBE0/5C4FAC2C" Ref="#PWR0110"  Part="1" 
AR Path="/5C4FCED2/5C4FAC2C" Ref="#PWR0111"  Part="1" 
AR Path="/5C4FCED8/5C4FAC2C" Ref="#PWR0112"  Part="1" 
AR Path="/5C4FD32A/5C4FAC2C" Ref="#PWR0113"  Part="1" 
AR Path="/5C4FD330/5C4FAC2C" Ref="#PWR0114"  Part="1" 
AR Path="/5C4FD336/5C4FAC2C" Ref="#PWR0115"  Part="1" 
AR Path="/5C4FD33C/5C4FAC2C" Ref="#PWR0116"  Part="1" 
AR Path="/5C4FFA25/5C4FAC2C" Ref="#PWR0117"  Part="1" 
AR Path="/5C4FFA2B/5C4FAC2C" Ref="#PWR0118"  Part="1" 
AR Path="/5C4FFA31/5C4FAC2C" Ref="#PWR0119"  Part="1" 
AR Path="/5C4FFA37/5C4FAC2C" Ref="#PWR0120"  Part="1" 
AR Path="/5C4FFA3D/5C4FAC2C" Ref="#PWR?"  Part="1" 
AR Path="/5C4FFA43/5C4FAC2C" Ref="#PWR?"  Part="1" 
F 0 "#PWR0120" H 5400 3950 50  0001 C CNN
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
$Comp
L Device:D_Zener D1
U 1 1 5C7C1653
P 6000 3300
AR Path="/5C4F59F4/5C7C1653" Ref="D1"  Part="1" 
AR Path="/5C4FCED2/5C7C1653" Ref="D3"  Part="1" 
AR Path="/5C4FCBE0/5C7C1653" Ref="D2"  Part="1" 
AR Path="/5C4FCED8/5C7C1653" Ref="D4"  Part="1" 
AR Path="/5C4FD32A/5C7C1653" Ref="D5"  Part="1" 
AR Path="/5C4FD330/5C7C1653" Ref="D6"  Part="1" 
AR Path="/5C4FD336/5C7C1653" Ref="D7"  Part="1" 
AR Path="/5C4FD33C/5C7C1653" Ref="D8"  Part="1" 
AR Path="/5C4FFA25/5C7C1653" Ref="D9"  Part="1" 
AR Path="/5C4FFA2B/5C7C1653" Ref="D10"  Part="1" 
AR Path="/5C4FFA31/5C7C1653" Ref="D11"  Part="1" 
AR Path="/5C4FFA37/5C7C1653" Ref="D12"  Part="1" 
F 0 "D12" V 5954 3379 50  0000 L CNN
F 1 "PDZ7.5B" V 6045 3379 50  0000 L CNN
F 2 "Diodes_SMD:D_SOD-323_HandSoldering" H 6000 3300 50  0001 C CNN
F 3 "~" H 6000 3300 50  0001 C CNN
	1    6000 3300
	0    1    1    0   
$EndComp
Text HLabel 6250 4250 2    50   Input ~ 0
CM
Wire Wire Line
	6000 3150 6000 2700
Connection ~ 6000 2700
Wire Wire Line
	6000 2700 6500 2700
Wire Wire Line
	6000 3450 6000 4250
Wire Wire Line
	6000 4250 6250 4250
$EndSCHEMATC
