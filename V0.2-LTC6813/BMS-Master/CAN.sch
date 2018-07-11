EESchema Schematic File Version 4
LIBS:BMS-Master-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 8
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
L ENNOID:ISO1050DUBR U?
U 1 1 5A91E99C
P 5600 3500
F 0 "U?" H 5600 3915 50  0000 C CNN
F 1 "ISO1050DUBR" H 5600 3824 50  0000 C CNN
F 2 "" H 5500 3150 50  0001 C CNN
F 3 "" H 5500 3150 50  0001 C CNN
	1    5600 3500
	1    0    0    -1  
$EndComp
Text HLabel 4750 3450 0    50   Output ~ 0
CAN_RX
Wire Wire Line
	4750 3450 5200 3450
Text HLabel 4750 3550 0    50   Input ~ 0
CAN_TX
Wire Wire Line
	4750 3550 5200 3550
$Comp
L BMS-Master-rescue:+3.3V-power #PWR?
U 1 1 5A91EA58
P 5050 3250
F 0 "#PWR?" H 5050 3100 50  0001 C CNN
F 1 "+3.3V" H 5065 3423 50  0000 C CNN
F 2 "" H 5050 3250 50  0001 C CNN
F 3 "" H 5050 3250 50  0001 C CNN
	1    5050 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 3250 5050 3350
Wire Wire Line
	5050 3350 5200 3350
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5A91EADF
P 5000 3850
F 0 "#PWR?" H 5000 3600 50  0001 C CNN
F 1 "GND" H 5005 3677 50  0000 C CNN
F 2 "" H 5000 3850 50  0001 C CNN
F 3 "" H 5000 3850 50  0001 C CNN
	1    5000 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 3650 5000 3650
Wire Wire Line
	5000 3650 5000 3850
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5A91EB41
P 3750 3500
F 0 "C?" H 3865 3546 50  0000 L CNN
F 1 "100n" H 3865 3455 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 3788 3350 50  0001 C CNN
F 3 "~" H 3750 3500 50  0001 C CNN
	1    3750 3500
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:+3.3V-power #PWR?
U 1 1 5A91EB6B
P 3750 3050
F 0 "#PWR?" H 3750 2900 50  0001 C CNN
F 1 "+3.3V" H 3765 3223 50  0000 C CNN
F 2 "" H 3750 3050 50  0001 C CNN
F 3 "" H 3750 3050 50  0001 C CNN
	1    3750 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 3050 3750 3350
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5A91EBA8
P 3750 3750
F 0 "#PWR?" H 3750 3500 50  0001 C CNN
F 1 "GND" H 3755 3577 50  0000 C CNN
F 2 "" H 3750 3750 50  0001 C CNN
F 3 "" H 3750 3750 50  0001 C CNN
	1    3750 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 3650 3750 3750
Text HLabel 7500 2700 2    50   Input ~ 0
CANH
Text HLabel 7550 4150 2    50   Input ~ 0
CANL
Wire Notes Line
	5550 2350 5550 4850
Wire Notes Line
	5650 2350 5650 4850
$Comp
L BMS-Master-rescue:GNDD-power #PWR?
U 1 1 5A91F28A
P 6100 3850
F 0 "#PWR?" H 6100 3600 50  0001 C CNN
F 1 "GNDD" H 6104 3695 50  0000 C CNN
F 2 "" H 6100 3850 50  0001 C CNN
F 3 "" H 6100 3850 50  0001 C CNN
	1    6100 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 3650 6100 3650
Wire Wire Line
	6100 3650 6100 3850
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5A91F3C0
P 7300 3500
F 0 "C?" V 7048 3500 50  0000 C CNN
F 1 "100n" V 7139 3500 50  0000 C CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 7338 3350 50  0001 C CNN
F 3 "~" H 7300 3500 50  0001 C CNN
	1    7300 3500
	0    1    1    0   
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5A91F45F
P 6900 3050
F 0 "R?" H 6970 3096 50  0000 L CNN
F 1 "60.4" H 6970 3005 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" V 6830 3050 50  0001 C CNN
F 3 "~" H 6900 3050 50  0001 C CNN
	1    6900 3050
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5A91F4AE
P 6900 3900
F 0 "R?" H 6970 3946 50  0000 L CNN
F 1 "60.4" H 6970 3855 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" V 6830 3900 50  0001 C CNN
F 3 "~" H 6900 3900 50  0001 C CNN
	1    6900 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 3200 6900 3500
Wire Wire Line
	7150 3500 6900 3500
Connection ~ 6900 3500
Wire Wire Line
	6900 3500 6900 3750
Wire Wire Line
	6900 2900 6900 2700
Wire Wire Line
	6900 2700 7500 2700
Wire Wire Line
	6900 4050 6900 4150
Wire Wire Line
	6900 4150 7550 4150
$Comp
L BMS-Master-rescue:GNDD-power #PWR?
U 1 1 5A91F76E
P 7650 3650
F 0 "#PWR?" H 7650 3400 50  0001 C CNN
F 1 "GNDD" H 7654 3495 50  0000 C CNN
F 2 "" H 7650 3650 50  0001 C CNN
F 3 "" H 7650 3650 50  0001 C CNN
	1    7650 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 3500 7650 3500
Wire Wire Line
	7650 3500 7650 3650
Wire Wire Line
	6900 4150 6600 4150
Wire Wire Line
	6600 4150 6600 3550
Wire Wire Line
	6600 3550 6000 3550
Connection ~ 6900 4150
Wire Wire Line
	6000 3450 6600 3450
Wire Wire Line
	6600 3450 6600 2700
Wire Wire Line
	6600 2700 6900 2700
Connection ~ 6900 2700
$Comp
L BMS-Master-rescue:GNDD-power #PWR?
U 1 1 5A91FE8A
P 7650 2150
F 0 "#PWR?" H 7650 1900 50  0001 C CNN
F 1 "GNDD" H 7654 1995 50  0000 C CNN
F 2 "" H 7650 2150 50  0001 C CNN
F 3 "" H 7650 2150 50  0001 C CNN
	1    7650 2150
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5A91FEEA
P 7650 1900
F 0 "C?" H 7535 1854 50  0000 R CNN
F 1 "100n" H 7535 1945 50  0000 R CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 7688 1750 50  0001 C CNN
F 3 "~" H 7650 1900 50  0001 C CNN
	1    7650 1900
	-1   0    0    1   
$EndComp
Wire Wire Line
	7650 2150 7650 2050
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5A92018C
P 8150 1900
F 0 "C?" H 8035 1854 50  0000 R CNN
F 1 "4.7u" H 8035 1945 50  0000 R CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 8188 1750 50  0001 C CNN
F 3 "~" H 8150 1900 50  0001 C CNN
	1    8150 1900
	-1   0    0    1   
$EndComp
$Comp
L BMS-Master-rescue:GNDD-power #PWR?
U 1 1 5A9201EB
P 8150 2150
F 0 "#PWR?" H 8150 1900 50  0001 C CNN
F 1 "GNDD" H 8154 1995 50  0000 C CNN
F 2 "" H 8150 2150 50  0001 C CNN
F 3 "" H 8150 2150 50  0001 C CNN
	1    8150 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	8150 2050 8150 2150
$Comp
L BMS-Master-rescue:MBR0540-Diode D?
U 1 1 5A92055E
P 9000 1600
F 0 "D?" H 9000 1500 50  0000 C CNN
F 1 "MBR0540" H 9000 1750 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-123" H 9000 1425 50  0001 C CNN
F 3 "http://www.mccsemi.com/up_pdf/MBR0520~MBR0580(SOD123).pdf" H 9000 1600 50  0001 C CNN
	1    9000 1600
	-1   0    0    1   
$EndComp
$Comp
L BMS-Master-rescue:2N7002-Transistor_FET Q?
U 1 1 5A92061C
P 9000 950
F 0 "Q?" V 9250 950 50  0000 C CNN
F 1 "2N7002" V 9341 950 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 9200 875 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N7002.pdf" H 9000 950 50  0001 L CNN
	1    9000 950 
	0    1    1    0   
$EndComp
Wire Wire Line
	8150 1600 8150 1750
Wire Wire Line
	7650 1750 7650 1600
Wire Wire Line
	7650 1600 8150 1600
Connection ~ 8150 1600
Wire Wire Line
	8150 750  9000 750 
Wire Wire Line
	8150 750  8150 1600
$Comp
L BMS-Master-rescue:GNDD-power #PWR?
U 1 1 5A921C15
P 8700 1800
F 0 "#PWR?" H 8700 1550 50  0001 C CNN
F 1 "GNDD" H 8704 1645 50  0000 C CNN
F 2 "" H 8700 1800 50  0001 C CNN
F 3 "" H 8700 1800 50  0001 C CNN
	1    8700 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8700 1800 8700 1600
Wire Wire Line
	8700 1600 8850 1600
Wire Wire Line
	8700 1600 8700 1050
Wire Wire Line
	8700 1050 8800 1050
Connection ~ 8700 1600
Wire Wire Line
	9200 1050 9400 1050
Wire Wire Line
	9400 1050 9400 1600
Wire Wire Line
	9400 1600 9150 1600
Text HLabel 9650 750  2    50   Input ~ 0
5V_CAN
Wire Wire Line
	9000 750  9650 750 
Connection ~ 9000 750 
Text HLabel 9650 1600 2    50   Input ~ 0
GND_CAN
Wire Wire Line
	9400 1600 9650 1600
Connection ~ 9400 1600
Wire Notes Line
	8500 550  10300 550 
Wire Notes Line
	10300 550  10300 2050
Wire Notes Line
	10300 2050 8500 2050
Wire Notes Line
	8500 2050 8500 550 
Text Notes 9150 1950 0    50   ~ 0
Reverse polarity protection\n
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5A924642
P 3300 3500
F 0 "C?" H 3415 3546 50  0000 L CNN
F 1 "1u" H 3415 3455 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 3338 3350 50  0001 C CNN
F 3 "~" H 3300 3500 50  0001 C CNN
	1    3300 3500
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:+3.3V-power #PWR?
U 1 1 5A924C1D
P 3300 3050
F 0 "#PWR?" H 3300 2900 50  0001 C CNN
F 1 "+3.3V" H 3315 3223 50  0000 C CNN
F 2 "" H 3300 3050 50  0001 C CNN
F 3 "" H 3300 3050 50  0001 C CNN
	1    3300 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 3050 3300 3350
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5A925217
P 3300 3750
F 0 "#PWR?" H 3300 3500 50  0001 C CNN
F 1 "GND" H 3305 3577 50  0000 C CNN
F 2 "" H 3300 3750 50  0001 C CNN
F 3 "" H 3300 3750 50  0001 C CNN
	1    3300 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 3650 3300 3750
$EndSCHEMATC
