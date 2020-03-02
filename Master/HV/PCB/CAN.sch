EESchema Schematic File Version 5
LIBS:BMS-Master-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 7
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
L ENNOID:ISO1050DUBR U8
U 1 1 5A91E99C
P 5600 3500
F 0 "U8" H 5600 3915 50  0000 C CNN
F 1 "ISO1050DUBR" H 5600 3824 50  0000 C CNN
F 2 "ENNOID:ISO1050" H 5500 3150 50  0001 C CNN
F 3 "" H 5500 3150 50  0001 C CNN
F 4 "ISO1050DUBR" H 5600 3500 50  0001 C CNN "MPN"
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
L BMS-Master-rescue:+3.3V-power-BMS-Master-rescue #PWR0183
U 1 1 5A91EA58
P 5050 3250
AR Path="/5A91EA58" Ref="#PWR0183"  Part="1" 
AR Path="/5A91E16C/5A91EA58" Ref="#PWR0183"  Part="1" 
F 0 "#PWR0183" H 5050 3100 50  0001 C CNN
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
L BMS-Master-rescue:GND-power-BMS-Master-rescue #PWR0184
U 1 1 5A91EADF
P 5000 5150
F 0 "#PWR0184" H 5000 4900 50  0001 C CNN
F 1 "GND" H 5005 4977 50  0000 C CNN
F 2 "" H 5000 5150 50  0001 C CNN
F 3 "" H 5000 5150 50  0001 C CNN
	1    5000 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 3650 5000 3650
$Comp
L BMS-Master-rescue:+3.3V-power-BMS-Master-rescue #PWR0185
U 1 1 5A91EB6B
P 3750 3050
AR Path="/5A91EB6B" Ref="#PWR0185"  Part="1" 
AR Path="/5A91E16C/5A91EB6B" Ref="#PWR0185"  Part="1" 
F 0 "#PWR0185" H 3750 2900 50  0001 C CNN
F 1 "+3.3V" H 3765 3223 50  0000 C CNN
F 2 "" H 3750 3050 50  0001 C CNN
F 3 "" H 3750 3050 50  0001 C CNN
	1    3750 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 3050 3750 3350
$Comp
L BMS-Master-rescue:GND-power-BMS-Master-rescue #PWR0186
U 1 1 5A91EBA8
P 3750 3750
F 0 "#PWR0186" H 3750 3500 50  0001 C CNN
F 1 "GND" H 3755 3577 50  0000 C CNN
F 2 "" H 3750 3750 50  0001 C CNN
F 3 "" H 3750 3750 50  0001 C CNN
	1    3750 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 3650 3750 3750
Text HLabel 7550 3550 2    50   Input ~ 0
CANH
Text HLabel 7550 3450 2    50   Input ~ 0
CANL
$Comp
L BMS-Master-rescue:+3.3V-power-BMS-Master-rescue #PWR0187
U 1 1 5A924C1D
P 3300 3050
AR Path="/5A924C1D" Ref="#PWR0187"  Part="1" 
AR Path="/5A91E16C/5A924C1D" Ref="#PWR0187"  Part="1" 
F 0 "#PWR0187" H 3300 2900 50  0001 C CNN
F 1 "+3.3V" H 3315 3223 50  0000 C CNN
F 2 "" H 3300 3050 50  0001 C CNN
F 3 "" H 3300 3050 50  0001 C CNN
	1    3300 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 3050 3300 3350
$Comp
L BMS-Master-rescue:GND-power-BMS-Master-rescue #PWR0188
U 1 1 5A925217
P 3300 3750
F 0 "#PWR0188" H 3300 3500 50  0001 C CNN
F 1 "GND" H 3305 3577 50  0000 C CNN
F 2 "" H 3300 3750 50  0001 C CNN
F 3 "" H 3300 3750 50  0001 C CNN
	1    3300 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 3650 3300 3750
Text HLabel 7700 1700 2    50   Input ~ 0
5V_CAN
Wire Wire Line
	7700 1700 7000 1700
Connection ~ 7000 1700
Wire Wire Line
	7000 1700 6650 1700
Wire Wire Line
	6650 1700 6200 1700
Wire Wire Line
	6200 1700 6200 3350
Wire Wire Line
	6200 3350 6000 3350
Connection ~ 6650 1700
Text Label 7350 2100 0    50   ~ 0
CAN_GND
Wire Wire Line
	6650 2000 6650 2100
Wire Wire Line
	6650 2100 7000 2100
Wire Wire Line
	7000 2000 7000 2100
Connection ~ 7000 2100
Wire Wire Line
	7000 2100 7350 2100
Text Label 6100 5150 0    50   ~ 0
CAN_GND
Wire Wire Line
	6000 5150 6100 5150
Text HLabel 6050 5300 2    50   Input ~ 0
CAN_GND
Wire Wire Line
	6050 5300 6000 5300
Wire Wire Line
	6000 5300 6000 5150
Connection ~ 6000 5150
$Comp
L Power_Protection:SZNUP2105L D4
U 1 1 5C258D40
P 7000 3900
F 0 "D4" H 7205 3946 50  0000 L CNN
F 1 "SZNUP2105L" H 7205 3855 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 7225 3850 50  0001 L CNN
F 3 "http://www.onsemi.com/pub_link/Collateral/NUP2105L-D.PDF" H 7125 4025 50  0001 C CNN
F 4 "SZNUP2105LT1G" H 7000 3900 50  0001 C CNN "MPN"
	1    7000 3900
	1    0    0    -1  
$EndComp
Text Label 7150 4200 0    50   ~ 0
CAN_GND
Wire Wire Line
	7000 4100 7000 4200
Wire Wire Line
	7000 4200 7150 4200
Wire Notes Line
	5650 2350 5650 6300
Wire Notes Line
	5550 2350 5550 6300
$Comp
L Isolator:SFH617A-2X017T U9
U 1 1 5C25CD2F
P 5600 6100
F 0 "U9" H 5600 6425 50  0000 C CNN
F 1 "SFH617A-2X009T" H 5600 6334 50  0000 C CNN
F 2 "Housings_DIP:SMDIP-4_W9.53mm_Clearance8mm" H 5600 5750 50  0001 C CIN
F 3 "http://www.vishay.com/docs/83740/sfh617a.pdf" H 5600 6050 50  0001 L CNN
F 4 "SFH617A-2X009T" H 5600 6100 50  0001 C CNN "MPN"
	1    5600 6100
	-1   0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:GND-power-BMS-Master-rescue #PWR0189
U 1 1 5C25DB56
P 5100 6400
F 0 "#PWR0189" H 5100 6150 50  0001 C CNN
F 1 "GND" H 5105 6227 50  0000 C CNN
F 2 "" H 5100 6400 50  0001 C CNN
F 3 "" H 5100 6400 50  0001 C CNN
	1    5100 6400
	1    0    0    -1  
$EndComp
Text HLabel 4300 6000 0    50   Input ~ 0
EnableRequest
Wire Wire Line
	5300 6000 4300 6000
Wire Wire Line
	5300 6200 5100 6200
Wire Wire Line
	5100 6200 5100 6400
$Comp
L BMS-Master-rescue:R-Device-BMS-Master-rescue R116
U 1 1 5C25FA4A
P 6450 6000
F 0 "R116" V 6400 5700 50  0000 L CNN
F 1 "2.2k" V 6300 5700 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6380 6000 50  0001 C CNN
F 3 "~" H 6450 6000 50  0001 C CNN
F 4 "RC0603JR-072K2L" H 6450 6000 50  0001 C CNN "MPN"
	1    6450 6000
	0    1    1    0   
$EndComp
Wire Wire Line
	5900 6000 6300 6000
Text Label 6200 6200 0    50   ~ 0
CAN_GND
Wire Wire Line
	5900 6200 6200 6200
Text HLabel 7450 6000 2    50   Input ~ 0
Enable_CAN
Wire Wire Line
	6600 6000 7450 6000
Wire Wire Line
	6000 3450 6900 3450
Wire Wire Line
	6000 3550 7100 3550
Wire Wire Line
	6900 3700 6900 3450
Connection ~ 6900 3450
Wire Wire Line
	6900 3450 7550 3450
Wire Wire Line
	7100 3700 7100 3550
Connection ~ 7100 3550
Wire Wire Line
	7100 3550 7550 3550
$Comp
L BMS-Master-rescue:C-Device-BMS-Master-rescue C47
U 1 1 5C24AE55
P 7000 1850
F 0 "C47" V 6748 1850 50  0000 C CNN
F 1 "100n" V 6839 1850 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7038 1700 50  0001 C CNN
F 3 "~" H 7000 1850 50  0001 C CNN
F 4 "GRM188R71E104KA01D" H 7000 1850 50  0001 C CNN "MPN"
	1    7000 1850
	-1   0    0    1   
$EndComp
$Comp
L BMS-Master-rescue:C-Device-BMS-Master-rescue C46
U 1 1 5C24B678
P 6650 1850
F 0 "C46" H 6765 1896 50  0000 L CNN
F 1 "1u" H 6765 1805 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6688 1700 50  0001 C CNN
F 3 "~" H 6650 1850 50  0001 C CNN
F 4 "GRM188R71E105KA12D" H 6650 1850 50  0001 C CNN "MPN"
	1    6650 1850
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:C-Device-BMS-Master-rescue C43
U 1 1 5A924642
P 3300 3500
F 0 "C43" H 3415 3546 50  0000 L CNN
F 1 "1u" H 3415 3455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3338 3350 50  0001 C CNN
F 3 "~" H 3300 3500 50  0001 C CNN
F 4 "GRM188R71E105KA12D" H 3300 3500 50  0001 C CNN "MPN"
	1    3300 3500
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:C-Device-BMS-Master-rescue C44
U 1 1 5A91EB41
P 3750 3500
F 0 "C44" H 3865 3546 50  0000 L CNN
F 1 "100n" H 3865 3455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3788 3350 50  0001 C CNN
F 3 "~" H 3750 3500 50  0001 C CNN
F 4 "GRM188R71E104KA01D" H 3750 3500 50  0001 C CNN "MPN"
	1    3750 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 3650 5000 5150
Wire Wire Line
	6000 3650 6000 5150
$EndSCHEMATC
