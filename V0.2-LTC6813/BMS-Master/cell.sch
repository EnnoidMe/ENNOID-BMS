EESchema Schematic File Version 4
LIBS:BMS-Master-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 25 25
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
U 1 1 5B4FCD0D
P 5050 3250
AR Path="/5B4A196B/5B4FCD0D" Ref="R?"  Part="1" 
AR Path="/5B4A196B/5B50012A/5B4FCD0D" Ref="R?"  Part="1" 
AR Path="/5B4A196B/5B504814/5B4FCD0D" Ref="R?"  Part="1" 
AR Path="/5B4A196B/5B5115CC/5B4FCD0D" Ref="R?"  Part="1" 
F 0 "R?" V 4843 3250 50  0000 C CNN
F 1 "10" V 4934 3250 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.99x1.00mm_HandSolder" V 4980 3250 50  0001 C CNN
F 3 "~" H 5050 3250 50  0001 C CNN
	1    5050 3250
	0    1    1    0   
$EndComp
$Comp
L Device:C C?
U 1 1 5B4FCD15
P 5650 3500
AR Path="/5B4A196B/5B4FCD15" Ref="C?"  Part="1" 
AR Path="/5B4A196B/5B4FCC47/5B4FCD15" Ref="C?"  Part="1" 
AR Path="/5B4A196B/5B50012A/5B4FCD15" Ref="C?"  Part="1" 
AR Path="/5B4A196B/5B504814/5B4FCD15" Ref="C?"  Part="1" 
AR Path="/5B4A196B/5B5115CC/5B4FCD15" Ref="C?"  Part="1" 
F 0 "C?" H 5535 3454 50  0000 R CNN
F 1 "C" H 5535 3545 50  0000 R CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad0.99x1.00mm_HandSolder" H 5688 3350 50  0001 C CNN
F 3 "~" H 5650 3500 50  0001 C CNN
	1    5650 3500
	-1   0    0    1   
$EndComp
Wire Wire Line
	4900 3250 4750 3250
$Comp
L Device:R R?
U 1 1 5B4FCD21
P 5050 3850
AR Path="/5B4A196B/5B4FCD21" Ref="R?"  Part="1" 
AR Path="/5B4A196B/5B4FCC47/5B4FCD21" Ref="R?"  Part="1" 
AR Path="/5B4A196B/5B50012A/5B4FCD21" Ref="R?"  Part="1" 
AR Path="/5B4A196B/5B504814/5B4FCD21" Ref="R?"  Part="1" 
AR Path="/5B4A196B/5B5115CC/5B4FCD21" Ref="R?"  Part="1" 
F 0 "R?" V 4843 3850 50  0000 C CNN
F 1 "33" V 4934 3850 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.99x1.00mm_HandSolder" V 4980 3850 50  0001 C CNN
F 3 "~" H 5050 3850 50  0001 C CNN
	1    5050 3850
	0    1    1    0   
$EndComp
Wire Wire Line
	4900 3850 4750 3850
Wire Wire Line
	4750 3850 4750 3250
Connection ~ 4750 3250
Wire Wire Line
	5200 3250 5650 3250
Wire Wire Line
	5650 3250 5650 3350
Text HLabel 5300 3850 2    50   Input ~ 0
S
Wire Wire Line
	5200 3850 5300 3850
Text HLabel 5900 3650 2    50   Input ~ 0
V-
Wire Wire Line
	5650 3650 5900 3650
Text HLabel 5950 3250 2    50   Input ~ 0
C
Wire Wire Line
	5650 3250 5950 3250
Connection ~ 5650 3250
Text HLabel 4150 3250 0    50   Input ~ 0
C+
Wire Wire Line
	4150 3250 4750 3250
$EndSCHEMATC
