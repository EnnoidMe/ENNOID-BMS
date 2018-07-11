EESchema Schematic File Version 4
LIBS:BMS-Master-cache
EELAYER 26 0
EELAYER END
$Descr A2 23386 16535
encoding utf-8
Sheet 2 8
Title "BMS-Master"
Date ""
Rev "V0.1"
Comp "ENNOID"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 5850 1500 2    50   Output ~ 0
VSENSE16
Text HLabel 5850 2850 2    50   Output ~ 0
VSENSE15
$Comp
L BMS-Master-rescue:D_Zener-Device D?
U 1 1 5AB6D843
P 5750 2100
F 0 "D?" V 5704 2179 50  0000 L CNN
F 1 "DDZ9690-7" V 5795 2179 50  0000 L CNN
F 2 "Diodes_SMD:D_SOD-123" H 5750 2100 50  0001 C CNN
F 3 "~" H 5750 2100 50  0001 C CNN
	1    5750 2100
	0    1    1    0   
$EndComp
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AB6DF8E
P 5050 1700
F 0 "C?" H 5165 1791 50  0000 L CNN
F 1 "0.1U" H 5165 1700 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5088 1550 50  0001 C CNN
F 3 "~" H 5050 1700 50  0001 C CNN
F 4 "100V" H 5165 1609 50  0000 L CNN "Voltage"
	1    5050 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 1950 5750 1500
Wire Wire Line
	5750 1500 5850 1500
Wire Wire Line
	5750 2250 5750 2850
Wire Wire Line
	5750 2850 5850 2850
Wire Wire Line
	5050 1550 5050 1500
Wire Wire Line
	5050 1500 5750 1500
Connection ~ 5750 1500
Text HLabel 5200 2000 2    50   BiDi ~ 0
BAT0
Wire Wire Line
	5050 1850 5050 2000
Wire Wire Line
	5050 2000 5200 2000
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB6E1E6
P 4550 1500
F 0 "R?" V 4343 1500 50  0000 C CNN
F 1 "100" V 4434 1500 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4480 1500 50  0001 C CNN
F 3 "~" H 4550 1500 50  0001 C CNN
	1    4550 1500
	0    1    1    0   
$EndComp
Wire Wire Line
	5050 1500 4700 1500
Connection ~ 5050 1500
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB6E27C
P 3900 1800
F 0 "R?" H 3830 1754 50  0000 R CNN
F 1 "75" H 3830 1845 50  0000 R CNN
F 2 "Resistors_SMD:R_2512" V 3830 1800 50  0001 C CNN
F 3 "~" H 3900 1800 50  0001 C CNN
	1    3900 1800
	-1   0    0    1   
$EndComp
Wire Wire Line
	3900 1650 3900 1500
Wire Wire Line
	3900 1500 4400 1500
Wire Wire Line
	3900 1500 3300 1500
Connection ~ 3900 1500
Text Label 3300 1500 0    50   ~ 0
BAT16S
$Comp
L BMS-Master-rescue:Q_NMOS_GSD-Device Q?
U 1 1 5AB6E620
P 4000 2300
F 0 "Q?" H 4206 2346 50  0000 L CNN
F 1 "2V7002K" H 4206 2255 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 4200 2400 50  0001 C CNN
F 3 "~" H 4000 2300 50  0001 C CNN
	1    4000 2300
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3900 1950 3900 2100
Text HLabel 4850 2300 2    50   Input ~ 0
EQ16
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB6E9AF
P 4600 2300
F 0 "R?" V 4807 2300 50  0000 C CNN
F 1 "2k" V 4716 2300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4530 2300 50  0001 C CNN
F 3 "~" H 4600 2300 50  0001 C CNN
	1    4600 2300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4750 2300 4850 2300
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB6EC14
P 4300 2650
F 0 "R?" H 4370 2696 50  0000 L CNN
F 1 "100k" H 4370 2605 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4230 2650 50  0001 C CNN
F 3 "~" H 4300 2650 50  0001 C CNN
	1    4300 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 2300 4300 2300
Wire Wire Line
	4300 2500 4300 2300
Connection ~ 4300 2300
Wire Wire Line
	4300 2300 4450 2300
Wire Wire Line
	4300 2800 4300 2850
Wire Wire Line
	4300 2850 4500 2850
Connection ~ 5750 2850
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB6F43F
P 4650 2850
F 0 "R?" V 4443 2850 50  0000 C CNN
F 1 "100" V 4534 2850 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4580 2850 50  0001 C CNN
F 3 "~" H 4650 2850 50  0001 C CNN
	1    4650 2850
	0    1    1    0   
$EndComp
Wire Wire Line
	4800 2850 5050 2850
Wire Wire Line
	3900 2500 3900 2850
Wire Wire Line
	3900 2850 4300 2850
Connection ~ 4300 2850
Wire Wire Line
	3900 2850 3300 2850
Connection ~ 3900 2850
Text Label 3300 2850 0    50   ~ 0
BAT15
$Comp
L BMS-Master-rescue:Conn_01x22-Connector_Generic J?
U 1 1 5AB6FB07
P 1050 3000
F 0 "J?" H 970 1675 50  0000 C CNN
F 1 "Conn_01x22" H 970 1766 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x22_Pitch2.54mm" H 1050 3000 50  0001 C CNN
F 3 "~" H 1050 3000 50  0001 C CNN
	1    1050 3000
	-1   0    0    1   
$EndComp
Text HLabel 5850 4200 2    50   Output ~ 0
VSENSE14
$Comp
L BMS-Master-rescue:D_Zener-Device D?
U 1 1 5AB73F65
P 5750 3450
F 0 "D?" V 5704 3529 50  0000 L CNN
F 1 "DDZ9690-7" V 5795 3529 50  0000 L CNN
F 2 "Diodes_SMD:D_SOD-123" H 5750 3450 50  0001 C CNN
F 3 "~" H 5750 3450 50  0001 C CNN
	1    5750 3450
	0    1    1    0   
$EndComp
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AB73F6C
P 5050 3050
F 0 "C?" H 5165 3141 50  0000 L CNN
F 1 "0.1U" H 5165 3050 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5088 2900 50  0001 C CNN
F 3 "~" H 5050 3050 50  0001 C CNN
F 4 "100V" H 5165 2959 50  0000 L CNN "Voltage"
	1    5050 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 3300 5750 2850
Wire Wire Line
	5750 3600 5750 4200
Wire Wire Line
	5750 4200 5850 4200
Wire Wire Line
	5050 2900 5050 2850
Text HLabel 5200 3350 2    50   BiDi ~ 0
BAT0
Wire Wire Line
	5050 3200 5050 3350
Wire Wire Line
	5050 3350 5200 3350
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB73F84
P 3900 3150
F 0 "R?" H 3830 3104 50  0000 R CNN
F 1 "75" H 3830 3195 50  0000 R CNN
F 2 "Resistors_SMD:R_2512" V 3830 3150 50  0001 C CNN
F 3 "~" H 3900 3150 50  0001 C CNN
	1    3900 3150
	-1   0    0    1   
$EndComp
Wire Wire Line
	3900 3000 3900 2850
$Comp
L BMS-Master-rescue:Q_NMOS_GSD-Device Q?
U 1 1 5AB73F8F
P 4000 3650
F 0 "Q?" H 4206 3696 50  0000 L CNN
F 1 "2V7002K" H 4206 3605 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 4200 3750 50  0001 C CNN
F 3 "~" H 4000 3650 50  0001 C CNN
	1    4000 3650
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3900 3300 3900 3450
Text HLabel 4850 3650 2    50   Input ~ 0
EQ15
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB73F97
P 4600 3650
F 0 "R?" V 4807 3650 50  0000 C CNN
F 1 "2k" V 4716 3650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4530 3650 50  0001 C CNN
F 3 "~" H 4600 3650 50  0001 C CNN
	1    4600 3650
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4750 3650 4850 3650
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB73F9E
P 4300 4000
F 0 "R?" H 4370 4046 50  0000 L CNN
F 1 "100k" H 4370 3955 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4230 4000 50  0001 C CNN
F 3 "~" H 4300 4000 50  0001 C CNN
	1    4300 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 3650 4300 3650
Wire Wire Line
	4300 3850 4300 3650
Connection ~ 4300 3650
Wire Wire Line
	4300 3650 4450 3650
Wire Wire Line
	4300 4150 4300 4200
Wire Wire Line
	4300 4200 4500 4200
Connection ~ 5750 4200
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB73FAB
P 4650 4200
F 0 "R?" V 4443 4200 50  0000 C CNN
F 1 "100" V 4534 4200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4580 4200 50  0001 C CNN
F 3 "~" H 4650 4200 50  0001 C CNN
	1    4650 4200
	0    1    1    0   
$EndComp
Wire Wire Line
	4800 4200 5050 4200
Wire Wire Line
	3900 3850 3900 4200
Wire Wire Line
	3900 4200 4300 4200
Connection ~ 4300 4200
Wire Wire Line
	3900 4200 3300 4200
Connection ~ 3900 4200
Text Label 3300 4200 0    50   ~ 0
BAT14
Connection ~ 5050 2850
Wire Wire Line
	5050 2850 5750 2850
Text HLabel 5850 5550 2    50   Output ~ 0
VSENSE13
$Comp
L BMS-Master-rescue:D_Zener-Device D?
U 1 1 5AB77DD4
P 5750 4800
F 0 "D?" V 5704 4879 50  0000 L CNN
F 1 "DDZ9690-7" V 5795 4879 50  0000 L CNN
F 2 "Diodes_SMD:D_SOD-123" H 5750 4800 50  0001 C CNN
F 3 "~" H 5750 4800 50  0001 C CNN
	1    5750 4800
	0    1    1    0   
$EndComp
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AB77DDB
P 5050 4400
F 0 "C?" H 5165 4491 50  0000 L CNN
F 1 "0.1U" H 5165 4400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5088 4250 50  0001 C CNN
F 3 "~" H 5050 4400 50  0001 C CNN
F 4 "100V" H 5165 4309 50  0000 L CNN "Voltage"
	1    5050 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 4650 5750 4200
Wire Wire Line
	5750 4950 5750 5550
Wire Wire Line
	5750 5550 5850 5550
Wire Wire Line
	5050 4250 5050 4200
Text HLabel 5200 4700 2    50   BiDi ~ 0
BAT0
Wire Wire Line
	5050 4550 5050 4700
Wire Wire Line
	5050 4700 5200 4700
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB77DF3
P 3900 4500
F 0 "R?" H 3830 4454 50  0000 R CNN
F 1 "75" H 3830 4545 50  0000 R CNN
F 2 "Resistors_SMD:R_2512" V 3830 4500 50  0001 C CNN
F 3 "~" H 3900 4500 50  0001 C CNN
	1    3900 4500
	-1   0    0    1   
$EndComp
Wire Wire Line
	3900 4350 3900 4200
$Comp
L BMS-Master-rescue:Q_NMOS_GSD-Device Q?
U 1 1 5AB77DFE
P 4000 5000
F 0 "Q?" H 4206 5046 50  0000 L CNN
F 1 "2V7002K" H 4206 4955 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 4200 5100 50  0001 C CNN
F 3 "~" H 4000 5000 50  0001 C CNN
	1    4000 5000
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3900 4650 3900 4800
Text HLabel 4850 5000 2    50   Input ~ 0
EQ14
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB77E06
P 4600 5000
F 0 "R?" V 4807 5000 50  0000 C CNN
F 1 "2k" V 4716 5000 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4530 5000 50  0001 C CNN
F 3 "~" H 4600 5000 50  0001 C CNN
	1    4600 5000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4750 5000 4850 5000
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB77E0D
P 4300 5350
F 0 "R?" H 4370 5396 50  0000 L CNN
F 1 "100k" H 4370 5305 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4230 5350 50  0001 C CNN
F 3 "~" H 4300 5350 50  0001 C CNN
	1    4300 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 5000 4300 5000
Wire Wire Line
	4300 5200 4300 5000
Connection ~ 4300 5000
Wire Wire Line
	4300 5000 4450 5000
Wire Wire Line
	4300 5500 4300 5550
Wire Wire Line
	4300 5550 4500 5550
Connection ~ 5750 5550
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB77E1A
P 4650 5550
F 0 "R?" V 4443 5550 50  0000 C CNN
F 1 "100" V 4534 5550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4580 5550 50  0001 C CNN
F 3 "~" H 4650 5550 50  0001 C CNN
	1    4650 5550
	0    1    1    0   
$EndComp
Wire Wire Line
	4800 5550 5050 5550
Wire Wire Line
	3900 5200 3900 5550
Wire Wire Line
	3900 5550 4300 5550
Connection ~ 4300 5550
Wire Wire Line
	3900 5550 3300 5550
Connection ~ 3900 5550
Text Label 3300 5550 0    50   ~ 0
BAT13
Text HLabel 5850 6900 2    50   Output ~ 0
VSENSE12
$Comp
L BMS-Master-rescue:D_Zener-Device D?
U 1 1 5AB77E28
P 5750 6150
F 0 "D?" V 5704 6229 50  0000 L CNN
F 1 "DDZ9690-7" V 5795 6229 50  0000 L CNN
F 2 "Diodes_SMD:D_SOD-123" H 5750 6150 50  0001 C CNN
F 3 "~" H 5750 6150 50  0001 C CNN
	1    5750 6150
	0    1    1    0   
$EndComp
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AB77E2F
P 5050 5750
F 0 "C?" H 5165 5841 50  0000 L CNN
F 1 "0.1U" H 5165 5750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5088 5600 50  0001 C CNN
F 3 "~" H 5050 5750 50  0001 C CNN
F 4 "100V" H 5165 5659 50  0000 L CNN "Voltage"
	1    5050 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 6000 5750 5550
Wire Wire Line
	5750 6300 5750 6900
Wire Wire Line
	5750 6900 5850 6900
Wire Wire Line
	5050 5600 5050 5550
Text HLabel 5200 6050 2    50   BiDi ~ 0
BAT0
Wire Wire Line
	5050 5900 5050 6050
Wire Wire Line
	5050 6050 5200 6050
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB77E3C
P 3900 5850
F 0 "R?" H 3830 5804 50  0000 R CNN
F 1 "75" H 3830 5895 50  0000 R CNN
F 2 "Resistors_SMD:R_2512" V 3830 5850 50  0001 C CNN
F 3 "~" H 3900 5850 50  0001 C CNN
	1    3900 5850
	-1   0    0    1   
$EndComp
Wire Wire Line
	3900 5700 3900 5550
$Comp
L BMS-Master-rescue:Q_NMOS_GSD-Device Q?
U 1 1 5AB77E43
P 4000 6350
F 0 "Q?" H 4206 6396 50  0000 L CNN
F 1 "2V7002K" H 4206 6305 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 4200 6450 50  0001 C CNN
F 3 "~" H 4000 6350 50  0001 C CNN
	1    4000 6350
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3900 6000 3900 6150
Text HLabel 4850 6350 2    50   Input ~ 0
EQ13
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB77E4B
P 4600 6350
F 0 "R?" V 4807 6350 50  0000 C CNN
F 1 "2k" V 4716 6350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4530 6350 50  0001 C CNN
F 3 "~" H 4600 6350 50  0001 C CNN
	1    4600 6350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4750 6350 4850 6350
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB77E52
P 4300 6700
F 0 "R?" H 4370 6746 50  0000 L CNN
F 1 "100k" H 4370 6655 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4230 6700 50  0001 C CNN
F 3 "~" H 4300 6700 50  0001 C CNN
	1    4300 6700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 6350 4300 6350
Wire Wire Line
	4300 6550 4300 6350
Connection ~ 4300 6350
Wire Wire Line
	4300 6350 4450 6350
Wire Wire Line
	4300 6850 4300 6900
Wire Wire Line
	4300 6900 4500 6900
Connection ~ 5750 6900
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB77E5F
P 4650 6900
F 0 "R?" V 4443 6900 50  0000 C CNN
F 1 "100" V 4534 6900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4580 6900 50  0001 C CNN
F 3 "~" H 4650 6900 50  0001 C CNN
	1    4650 6900
	0    1    1    0   
$EndComp
Wire Wire Line
	4800 6900 5050 6900
Wire Wire Line
	3900 6550 3900 6900
Wire Wire Line
	3900 6900 4300 6900
Connection ~ 4300 6900
Wire Wire Line
	3900 6900 3300 6900
Connection ~ 3900 6900
Text Label 3300 6900 0    50   ~ 0
BAT12
Connection ~ 5050 5550
Wire Wire Line
	5050 5550 5750 5550
Connection ~ 5050 4200
Wire Wire Line
	5050 4200 5750 4200
Text HLabel 5850 8250 2    50   Output ~ 0
VSENSE11
$Comp
L BMS-Master-rescue:D_Zener-Device D?
U 1 1 5AB7E6BE
P 5750 7500
F 0 "D?" V 5704 7579 50  0000 L CNN
F 1 "DDZ9690-7" V 5795 7579 50  0000 L CNN
F 2 "Diodes_SMD:D_SOD-123" H 5750 7500 50  0001 C CNN
F 3 "~" H 5750 7500 50  0001 C CNN
	1    5750 7500
	0    1    1    0   
$EndComp
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AB7E6C5
P 5050 7100
F 0 "C?" H 5165 7191 50  0000 L CNN
F 1 "0.1U" H 5165 7100 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5088 6950 50  0001 C CNN
F 3 "~" H 5050 7100 50  0001 C CNN
F 4 "100V" H 5165 7009 50  0000 L CNN "Voltage"
	1    5050 7100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 7350 5750 6900
Wire Wire Line
	5750 7650 5750 8250
Wire Wire Line
	5750 8250 5850 8250
Wire Wire Line
	5050 6950 5050 6900
Text HLabel 5200 7400 2    50   BiDi ~ 0
BAT0
Wire Wire Line
	5050 7250 5050 7400
Wire Wire Line
	5050 7400 5200 7400
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB7E6DD
P 3900 7200
F 0 "R?" H 3830 7154 50  0000 R CNN
F 1 "75" H 3830 7245 50  0000 R CNN
F 2 "Resistors_SMD:R_2512" V 3830 7200 50  0001 C CNN
F 3 "~" H 3900 7200 50  0001 C CNN
	1    3900 7200
	-1   0    0    1   
$EndComp
Wire Wire Line
	3900 7050 3900 6900
$Comp
L BMS-Master-rescue:Q_NMOS_GSD-Device Q?
U 1 1 5AB7E6E8
P 4000 7700
F 0 "Q?" H 4206 7746 50  0000 L CNN
F 1 "2V7002K" H 4206 7655 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 4200 7800 50  0001 C CNN
F 3 "~" H 4000 7700 50  0001 C CNN
	1    4000 7700
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3900 7350 3900 7500
Text HLabel 4850 7700 2    50   Input ~ 0
EQ12
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB7E6F0
P 4600 7700
F 0 "R?" V 4807 7700 50  0000 C CNN
F 1 "2k" V 4716 7700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4530 7700 50  0001 C CNN
F 3 "~" H 4600 7700 50  0001 C CNN
	1    4600 7700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4750 7700 4850 7700
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB7E6F7
P 4300 8050
F 0 "R?" H 4370 8096 50  0000 L CNN
F 1 "100k" H 4370 8005 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4230 8050 50  0001 C CNN
F 3 "~" H 4300 8050 50  0001 C CNN
	1    4300 8050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 7700 4300 7700
Wire Wire Line
	4300 7900 4300 7700
Connection ~ 4300 7700
Wire Wire Line
	4300 7700 4450 7700
Wire Wire Line
	4300 8200 4300 8250
Wire Wire Line
	4300 8250 4500 8250
Connection ~ 5750 8250
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB7E704
P 4650 8250
F 0 "R?" V 4443 8250 50  0000 C CNN
F 1 "100" V 4534 8250 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4580 8250 50  0001 C CNN
F 3 "~" H 4650 8250 50  0001 C CNN
	1    4650 8250
	0    1    1    0   
$EndComp
Wire Wire Line
	4800 8250 5050 8250
Wire Wire Line
	3900 7900 3900 8250
Wire Wire Line
	3900 8250 4300 8250
Connection ~ 4300 8250
Wire Wire Line
	3900 8250 3300 8250
Connection ~ 3900 8250
Text Label 3300 8250 0    50   ~ 0
BAT11
Text HLabel 5850 9600 2    50   Output ~ 0
VSENSE10
$Comp
L BMS-Master-rescue:D_Zener-Device D?
U 1 1 5AB7E712
P 5750 8850
F 0 "D?" V 5704 8929 50  0000 L CNN
F 1 "DDZ9690-7" V 5795 8929 50  0000 L CNN
F 2 "Diodes_SMD:D_SOD-123" H 5750 8850 50  0001 C CNN
F 3 "~" H 5750 8850 50  0001 C CNN
	1    5750 8850
	0    1    1    0   
$EndComp
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AB7E719
P 5050 8450
F 0 "C?" H 5165 8541 50  0000 L CNN
F 1 "0.1U" H 5165 8450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5088 8300 50  0001 C CNN
F 3 "~" H 5050 8450 50  0001 C CNN
F 4 "100V" H 5165 8359 50  0000 L CNN "Voltage"
	1    5050 8450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 8700 5750 8250
Wire Wire Line
	5750 9000 5750 9600
Wire Wire Line
	5750 9600 5850 9600
Wire Wire Line
	5050 8300 5050 8250
Text HLabel 5200 8750 2    50   BiDi ~ 0
BAT0
Wire Wire Line
	5050 8600 5050 8750
Wire Wire Line
	5050 8750 5200 8750
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB7E726
P 3900 8550
F 0 "R?" H 3830 8504 50  0000 R CNN
F 1 "75" H 3830 8595 50  0000 R CNN
F 2 "Resistors_SMD:R_2512" V 3830 8550 50  0001 C CNN
F 3 "~" H 3900 8550 50  0001 C CNN
	1    3900 8550
	-1   0    0    1   
$EndComp
Wire Wire Line
	3900 8400 3900 8250
$Comp
L BMS-Master-rescue:Q_NMOS_GSD-Device Q?
U 1 1 5AB7E72D
P 4000 9050
F 0 "Q?" H 4206 9096 50  0000 L CNN
F 1 "2V7002K" H 4206 9005 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 4200 9150 50  0001 C CNN
F 3 "~" H 4000 9050 50  0001 C CNN
	1    4000 9050
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3900 8700 3900 8850
Text HLabel 4850 9050 2    50   Input ~ 0
EQ11
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB7E735
P 4600 9050
F 0 "R?" V 4807 9050 50  0000 C CNN
F 1 "2k" V 4716 9050 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4530 9050 50  0001 C CNN
F 3 "~" H 4600 9050 50  0001 C CNN
	1    4600 9050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4750 9050 4850 9050
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB7E73C
P 4300 9400
F 0 "R?" H 4370 9446 50  0000 L CNN
F 1 "100k" H 4370 9355 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4230 9400 50  0001 C CNN
F 3 "~" H 4300 9400 50  0001 C CNN
	1    4300 9400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 9050 4300 9050
Wire Wire Line
	4300 9250 4300 9050
Connection ~ 4300 9050
Wire Wire Line
	4300 9050 4450 9050
Wire Wire Line
	4300 9550 4300 9600
Wire Wire Line
	4300 9600 4500 9600
Connection ~ 5750 9600
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB7E749
P 4650 9600
F 0 "R?" V 4443 9600 50  0000 C CNN
F 1 "100" V 4534 9600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4580 9600 50  0001 C CNN
F 3 "~" H 4650 9600 50  0001 C CNN
	1    4650 9600
	0    1    1    0   
$EndComp
Wire Wire Line
	4800 9600 5050 9600
Wire Wire Line
	3900 9250 3900 9600
Wire Wire Line
	3900 9600 4300 9600
Connection ~ 4300 9600
Wire Wire Line
	3900 9600 3300 9600
Connection ~ 3900 9600
Text Label 3300 9600 0    50   ~ 0
BAT10
Connection ~ 5050 8250
Wire Wire Line
	5050 8250 5750 8250
Text HLabel 5850 10950 2    50   Output ~ 0
VSENSE9
$Comp
L BMS-Master-rescue:D_Zener-Device D?
U 1 1 5AB7E759
P 5750 10200
F 0 "D?" V 5704 10279 50  0000 L CNN
F 1 "DDZ9690-7" V 5795 10279 50  0000 L CNN
F 2 "Diodes_SMD:D_SOD-123" H 5750 10200 50  0001 C CNN
F 3 "~" H 5750 10200 50  0001 C CNN
	1    5750 10200
	0    1    1    0   
$EndComp
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AB7E760
P 5050 9800
F 0 "C?" H 5165 9891 50  0000 L CNN
F 1 "0.1U" H 5165 9800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5088 9650 50  0001 C CNN
F 3 "~" H 5050 9800 50  0001 C CNN
F 4 "100V" H 5165 9709 50  0000 L CNN "Voltage"
	1    5050 9800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 10050 5750 9600
Wire Wire Line
	5750 10350 5750 10950
Wire Wire Line
	5750 10950 5850 10950
Wire Wire Line
	5050 9650 5050 9600
Text HLabel 5200 10100 2    50   BiDi ~ 0
BAT0
Wire Wire Line
	5050 9950 5050 10100
Wire Wire Line
	5050 10100 5200 10100
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB7E76D
P 3900 9900
F 0 "R?" H 3830 9854 50  0000 R CNN
F 1 "75" H 3830 9945 50  0000 R CNN
F 2 "Resistors_SMD:R_2512" V 3830 9900 50  0001 C CNN
F 3 "~" H 3900 9900 50  0001 C CNN
	1    3900 9900
	-1   0    0    1   
$EndComp
Wire Wire Line
	3900 9750 3900 9600
$Comp
L BMS-Master-rescue:Q_NMOS_GSD-Device Q?
U 1 1 5AB7E774
P 4000 10400
F 0 "Q?" H 4206 10446 50  0000 L CNN
F 1 "2V7002K" H 4206 10355 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 4200 10500 50  0001 C CNN
F 3 "~" H 4000 10400 50  0001 C CNN
	1    4000 10400
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3900 10050 3900 10200
Text HLabel 4850 10400 2    50   Input ~ 0
EQ10
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB7E77C
P 4600 10400
F 0 "R?" V 4807 10400 50  0000 C CNN
F 1 "2k" V 4716 10400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4530 10400 50  0001 C CNN
F 3 "~" H 4600 10400 50  0001 C CNN
	1    4600 10400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4750 10400 4850 10400
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB7E783
P 4300 10750
F 0 "R?" H 4370 10796 50  0000 L CNN
F 1 "100k" H 4370 10705 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4230 10750 50  0001 C CNN
F 3 "~" H 4300 10750 50  0001 C CNN
	1    4300 10750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 10400 4300 10400
Wire Wire Line
	4300 10600 4300 10400
Connection ~ 4300 10400
Wire Wire Line
	4300 10400 4450 10400
Wire Wire Line
	4300 10900 4300 10950
Wire Wire Line
	4300 10950 4500 10950
Connection ~ 5750 10950
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB7E790
P 4650 10950
F 0 "R?" V 4443 10950 50  0000 C CNN
F 1 "100" V 4534 10950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4580 10950 50  0001 C CNN
F 3 "~" H 4650 10950 50  0001 C CNN
	1    4650 10950
	0    1    1    0   
$EndComp
Wire Wire Line
	4800 10950 5050 10950
Wire Wire Line
	3900 10600 3900 10950
Wire Wire Line
	3900 10950 4300 10950
Connection ~ 4300 10950
Wire Wire Line
	3900 10950 3300 10950
Connection ~ 3900 10950
Text Label 3300 10950 0    50   ~ 0
BAT9
Text HLabel 5850 12300 2    50   Output ~ 0
VSENSE8
$Comp
L BMS-Master-rescue:D_Zener-Device D?
U 1 1 5AB7E79E
P 5750 11550
F 0 "D?" V 5704 11629 50  0000 L CNN
F 1 "DDZ9690-7" V 5795 11629 50  0000 L CNN
F 2 "Diodes_SMD:D_SOD-123" H 5750 11550 50  0001 C CNN
F 3 "~" H 5750 11550 50  0001 C CNN
	1    5750 11550
	0    1    1    0   
$EndComp
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AB7E7A5
P 5050 11150
F 0 "C?" H 5165 11241 50  0000 L CNN
F 1 "0.1U" H 5165 11150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5088 11000 50  0001 C CNN
F 3 "~" H 5050 11150 50  0001 C CNN
F 4 "100V" H 5165 11059 50  0000 L CNN "Voltage"
	1    5050 11150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 11400 5750 10950
Wire Wire Line
	5750 11700 5750 12300
Wire Wire Line
	5750 12300 5850 12300
Wire Wire Line
	5050 11000 5050 10950
Text HLabel 5200 11450 2    50   BiDi ~ 0
BAT0
Wire Wire Line
	5050 11300 5050 11450
Wire Wire Line
	5050 11450 5200 11450
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB7E7B2
P 3900 11250
F 0 "R?" H 3830 11204 50  0000 R CNN
F 1 "75" H 3830 11295 50  0000 R CNN
F 2 "Resistors_SMD:R_2512" V 3830 11250 50  0001 C CNN
F 3 "~" H 3900 11250 50  0001 C CNN
	1    3900 11250
	-1   0    0    1   
$EndComp
Wire Wire Line
	3900 11100 3900 10950
$Comp
L BMS-Master-rescue:Q_NMOS_GSD-Device Q?
U 1 1 5AB7E7B9
P 4000 11750
F 0 "Q?" H 4206 11796 50  0000 L CNN
F 1 "2V7002K" H 4206 11705 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 4200 11850 50  0001 C CNN
F 3 "~" H 4000 11750 50  0001 C CNN
	1    4000 11750
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3900 11400 3900 11550
Text HLabel 4850 11750 2    50   Input ~ 0
EQ9
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB7E7C1
P 4600 11750
F 0 "R?" V 4807 11750 50  0000 C CNN
F 1 "2k" V 4716 11750 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4530 11750 50  0001 C CNN
F 3 "~" H 4600 11750 50  0001 C CNN
	1    4600 11750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4750 11750 4850 11750
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB7E7C8
P 4300 12100
F 0 "R?" H 4370 12146 50  0000 L CNN
F 1 "100k" H 4370 12055 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4230 12100 50  0001 C CNN
F 3 "~" H 4300 12100 50  0001 C CNN
	1    4300 12100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 11750 4300 11750
Wire Wire Line
	4300 11950 4300 11750
Connection ~ 4300 11750
Wire Wire Line
	4300 11750 4450 11750
Wire Wire Line
	4300 12250 4300 12300
Wire Wire Line
	3900 11950 3900 12300
Wire Wire Line
	3900 12300 4300 12300
Wire Wire Line
	3900 12300 3300 12300
Connection ~ 3900 12300
Text Label 3300 12300 0    50   ~ 0
BAT8
Connection ~ 5050 10950
Wire Wire Line
	5050 10950 5750 10950
Connection ~ 5050 9600
Wire Wire Line
	5050 9600 5750 9600
Connection ~ 5050 6900
Wire Wire Line
	5050 6900 5750 6900
Text HLabel 13000 1600 2    50   Output ~ 0
VSENSE8
Text HLabel 13000 2950 2    50   Output ~ 0
VSENSE7
$Comp
L BMS-Master-rescue:D_Zener-Device D?
U 1 1 5AB98C7C
P 12900 2200
F 0 "D?" V 12854 2279 50  0000 L CNN
F 1 "DDZ9690-7" V 12945 2279 50  0000 L CNN
F 2 "Diodes_SMD:D_SOD-123" H 12900 2200 50  0001 C CNN
F 3 "~" H 12900 2200 50  0001 C CNN
	1    12900 2200
	0    1    1    0   
$EndComp
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AB98C83
P 12200 1800
F 0 "C?" H 12315 1891 50  0000 L CNN
F 1 "0.1U" H 12315 1800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 12238 1650 50  0001 C CNN
F 3 "~" H 12200 1800 50  0001 C CNN
F 4 "100V" H 12315 1709 50  0000 L CNN "Voltage"
	1    12200 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	12900 2050 12900 1600
Wire Wire Line
	12900 1600 13000 1600
Wire Wire Line
	12900 2350 12900 2950
Wire Wire Line
	12900 2950 13000 2950
Wire Wire Line
	12200 1650 12200 1600
Wire Wire Line
	12200 1600 12900 1600
Connection ~ 12900 1600
Text HLabel 12350 2100 2    50   BiDi ~ 0
BAT0
Wire Wire Line
	12200 1950 12200 2100
Wire Wire Line
	12200 2100 12350 2100
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98C93
P 11700 1600
F 0 "R?" V 11493 1600 50  0000 C CNN
F 1 "100" V 11584 1600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11630 1600 50  0001 C CNN
F 3 "~" H 11700 1600 50  0001 C CNN
	1    11700 1600
	0    1    1    0   
$EndComp
Wire Wire Line
	12200 1600 11850 1600
Connection ~ 12200 1600
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98C9B
P 11050 1900
F 0 "R?" H 10980 1854 50  0000 R CNN
F 1 "75" H 10980 1945 50  0000 R CNN
F 2 "Resistors_SMD:R_2512" V 10980 1900 50  0001 C CNN
F 3 "~" H 11050 1900 50  0001 C CNN
	1    11050 1900
	-1   0    0    1   
$EndComp
Wire Wire Line
	11050 1750 11050 1600
Wire Wire Line
	11050 1600 11550 1600
Wire Wire Line
	11050 1600 10450 1600
Connection ~ 11050 1600
Text Label 10450 1600 0    50   ~ 0
BAT8
$Comp
L BMS-Master-rescue:Q_NMOS_GSD-Device Q?
U 1 1 5AB98CA6
P 11150 2400
F 0 "Q?" H 11356 2446 50  0000 L CNN
F 1 "2V7002K" H 11356 2355 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 11350 2500 50  0001 C CNN
F 3 "~" H 11150 2400 50  0001 C CNN
	1    11150 2400
	-1   0    0    -1  
$EndComp
Wire Wire Line
	11050 2050 11050 2200
Text HLabel 12000 2400 2    50   Input ~ 0
EQ8
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98CAE
P 11750 2400
F 0 "R?" V 11957 2400 50  0000 C CNN
F 1 "2k" V 11866 2400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11680 2400 50  0001 C CNN
F 3 "~" H 11750 2400 50  0001 C CNN
	1    11750 2400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	11900 2400 12000 2400
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98CB5
P 11450 2750
F 0 "R?" H 11520 2796 50  0000 L CNN
F 1 "100k" H 11520 2705 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11380 2750 50  0001 C CNN
F 3 "~" H 11450 2750 50  0001 C CNN
	1    11450 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	11350 2400 11450 2400
Wire Wire Line
	11450 2600 11450 2400
Connection ~ 11450 2400
Wire Wire Line
	11450 2400 11600 2400
Wire Wire Line
	11450 2900 11450 2950
Wire Wire Line
	11450 2950 11650 2950
Connection ~ 12900 2950
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98CC2
P 11800 2950
F 0 "R?" V 11593 2950 50  0000 C CNN
F 1 "100" V 11684 2950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11730 2950 50  0001 C CNN
F 3 "~" H 11800 2950 50  0001 C CNN
	1    11800 2950
	0    1    1    0   
$EndComp
Wire Wire Line
	11950 2950 12200 2950
Wire Wire Line
	11050 2600 11050 2950
Wire Wire Line
	11050 2950 11450 2950
Connection ~ 11450 2950
Wire Wire Line
	11050 2950 10450 2950
Connection ~ 11050 2950
Text Label 10450 2950 0    50   ~ 0
BAT7
Text HLabel 13000 4300 2    50   Output ~ 0
VSENSE6
$Comp
L BMS-Master-rescue:D_Zener-Device D?
U 1 1 5AB98CD0
P 12900 3550
F 0 "D?" V 12854 3629 50  0000 L CNN
F 1 "DDZ9690-7" V 12945 3629 50  0000 L CNN
F 2 "Diodes_SMD:D_SOD-123" H 12900 3550 50  0001 C CNN
F 3 "~" H 12900 3550 50  0001 C CNN
	1    12900 3550
	0    1    1    0   
$EndComp
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AB98CD7
P 12200 3150
F 0 "C?" H 12315 3241 50  0000 L CNN
F 1 "0.1U" H 12315 3150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 12238 3000 50  0001 C CNN
F 3 "~" H 12200 3150 50  0001 C CNN
F 4 "100V" H 12315 3059 50  0000 L CNN "Voltage"
	1    12200 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	12900 3400 12900 2950
Wire Wire Line
	12900 3700 12900 4300
Wire Wire Line
	12900 4300 13000 4300
Wire Wire Line
	12200 3000 12200 2950
Text HLabel 12350 3450 2    50   BiDi ~ 0
BAT0
Wire Wire Line
	12200 3300 12200 3450
Wire Wire Line
	12200 3450 12350 3450
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98CE4
P 11050 3250
F 0 "R?" H 10980 3204 50  0000 R CNN
F 1 "75" H 10980 3295 50  0000 R CNN
F 2 "Resistors_SMD:R_2512" V 10980 3250 50  0001 C CNN
F 3 "~" H 11050 3250 50  0001 C CNN
	1    11050 3250
	-1   0    0    1   
$EndComp
Wire Wire Line
	11050 3100 11050 2950
$Comp
L BMS-Master-rescue:Q_NMOS_GSD-Device Q?
U 1 1 5AB98CEB
P 11150 3750
F 0 "Q?" H 11356 3796 50  0000 L CNN
F 1 "2V7002K" H 11356 3705 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 11350 3850 50  0001 C CNN
F 3 "~" H 11150 3750 50  0001 C CNN
	1    11150 3750
	-1   0    0    -1  
$EndComp
Wire Wire Line
	11050 3400 11050 3550
Text HLabel 12000 3750 2    50   Input ~ 0
EQ7
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98CF3
P 11750 3750
F 0 "R?" V 11957 3750 50  0000 C CNN
F 1 "2k" V 11866 3750 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11680 3750 50  0001 C CNN
F 3 "~" H 11750 3750 50  0001 C CNN
	1    11750 3750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	11900 3750 12000 3750
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98CFA
P 11450 4100
F 0 "R?" H 11520 4146 50  0000 L CNN
F 1 "100k" H 11520 4055 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11380 4100 50  0001 C CNN
F 3 "~" H 11450 4100 50  0001 C CNN
	1    11450 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	11350 3750 11450 3750
Wire Wire Line
	11450 3950 11450 3750
Connection ~ 11450 3750
Wire Wire Line
	11450 3750 11600 3750
Wire Wire Line
	11450 4250 11450 4300
Wire Wire Line
	11450 4300 11650 4300
Connection ~ 12900 4300
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98D07
P 11800 4300
F 0 "R?" V 11593 4300 50  0000 C CNN
F 1 "100" V 11684 4300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11730 4300 50  0001 C CNN
F 3 "~" H 11800 4300 50  0001 C CNN
	1    11800 4300
	0    1    1    0   
$EndComp
Wire Wire Line
	11950 4300 12200 4300
Wire Wire Line
	11050 3950 11050 4300
Wire Wire Line
	11050 4300 11450 4300
Connection ~ 11450 4300
Wire Wire Line
	11050 4300 10450 4300
Connection ~ 11050 4300
Text Label 10450 4300 0    50   ~ 0
BAT6
Connection ~ 12200 2950
Wire Wire Line
	12200 2950 12900 2950
Text HLabel 13000 5650 2    50   Output ~ 0
VSENSE5
$Comp
L BMS-Master-rescue:D_Zener-Device D?
U 1 1 5AB98D17
P 12900 4900
F 0 "D?" V 12854 4979 50  0000 L CNN
F 1 "DDZ9690-7" V 12945 4979 50  0000 L CNN
F 2 "Diodes_SMD:D_SOD-123" H 12900 4900 50  0001 C CNN
F 3 "~" H 12900 4900 50  0001 C CNN
	1    12900 4900
	0    1    1    0   
$EndComp
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AB98D1E
P 12200 4500
F 0 "C?" H 12315 4591 50  0000 L CNN
F 1 "0.1U" H 12315 4500 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 12238 4350 50  0001 C CNN
F 3 "~" H 12200 4500 50  0001 C CNN
F 4 "100V" H 12315 4409 50  0000 L CNN "Voltage"
	1    12200 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	12900 4750 12900 4300
Wire Wire Line
	12900 5050 12900 5650
Wire Wire Line
	12900 5650 13000 5650
Wire Wire Line
	12200 4350 12200 4300
Text HLabel 12350 4800 2    50   BiDi ~ 0
BAT0
Wire Wire Line
	12200 4650 12200 4800
Wire Wire Line
	12200 4800 12350 4800
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98D2B
P 11050 4600
F 0 "R?" H 10980 4554 50  0000 R CNN
F 1 "75" H 10980 4645 50  0000 R CNN
F 2 "Resistors_SMD:R_2512" V 10980 4600 50  0001 C CNN
F 3 "~" H 11050 4600 50  0001 C CNN
	1    11050 4600
	-1   0    0    1   
$EndComp
Wire Wire Line
	11050 4450 11050 4300
$Comp
L BMS-Master-rescue:Q_NMOS_GSD-Device Q?
U 1 1 5AB98D32
P 11150 5100
F 0 "Q?" H 11356 5146 50  0000 L CNN
F 1 "2V7002K" H 11356 5055 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 11350 5200 50  0001 C CNN
F 3 "~" H 11150 5100 50  0001 C CNN
	1    11150 5100
	-1   0    0    -1  
$EndComp
Wire Wire Line
	11050 4750 11050 4900
Text HLabel 12000 5100 2    50   Input ~ 0
EQ6
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98D3A
P 11750 5100
F 0 "R?" V 11957 5100 50  0000 C CNN
F 1 "2k" V 11866 5100 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11680 5100 50  0001 C CNN
F 3 "~" H 11750 5100 50  0001 C CNN
	1    11750 5100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	11900 5100 12000 5100
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98D41
P 11450 5450
F 0 "R?" H 11520 5496 50  0000 L CNN
F 1 "100k" H 11520 5405 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11380 5450 50  0001 C CNN
F 3 "~" H 11450 5450 50  0001 C CNN
	1    11450 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	11350 5100 11450 5100
Wire Wire Line
	11450 5300 11450 5100
Connection ~ 11450 5100
Wire Wire Line
	11450 5100 11600 5100
Wire Wire Line
	11450 5600 11450 5650
Wire Wire Line
	11450 5650 11650 5650
Connection ~ 12900 5650
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98D4E
P 11800 5650
F 0 "R?" V 11593 5650 50  0000 C CNN
F 1 "100" V 11684 5650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11730 5650 50  0001 C CNN
F 3 "~" H 11800 5650 50  0001 C CNN
	1    11800 5650
	0    1    1    0   
$EndComp
Wire Wire Line
	11950 5650 12200 5650
Wire Wire Line
	11050 5300 11050 5650
Wire Wire Line
	11050 5650 11450 5650
Connection ~ 11450 5650
Wire Wire Line
	11050 5650 10450 5650
Connection ~ 11050 5650
Text Label 10450 5650 0    50   ~ 0
BAT5
Text HLabel 13000 7000 2    50   Output ~ 0
VSENSE4
$Comp
L BMS-Master-rescue:D_Zener-Device D?
U 1 1 5AB98D5C
P 12900 6250
F 0 "D?" V 12854 6329 50  0000 L CNN
F 1 "DDZ9690-7" V 12945 6329 50  0000 L CNN
F 2 "Diodes_SMD:D_SOD-123" H 12900 6250 50  0001 C CNN
F 3 "~" H 12900 6250 50  0001 C CNN
	1    12900 6250
	0    1    1    0   
$EndComp
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AB98D63
P 12200 5850
F 0 "C?" H 12315 5941 50  0000 L CNN
F 1 "0.1U" H 12315 5850 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 12238 5700 50  0001 C CNN
F 3 "~" H 12200 5850 50  0001 C CNN
F 4 "100V" H 12315 5759 50  0000 L CNN "Voltage"
	1    12200 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	12900 6100 12900 5650
Wire Wire Line
	12900 6400 12900 7000
Wire Wire Line
	12900 7000 13000 7000
Wire Wire Line
	12200 5700 12200 5650
Text HLabel 12350 6150 2    50   BiDi ~ 0
BAT0
Wire Wire Line
	12200 6000 12200 6150
Wire Wire Line
	12200 6150 12350 6150
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98D70
P 11050 5950
F 0 "R?" H 10980 5904 50  0000 R CNN
F 1 "75" H 10980 5995 50  0000 R CNN
F 2 "Resistors_SMD:R_2512" V 10980 5950 50  0001 C CNN
F 3 "~" H 11050 5950 50  0001 C CNN
	1    11050 5950
	-1   0    0    1   
$EndComp
Wire Wire Line
	11050 5800 11050 5650
$Comp
L BMS-Master-rescue:Q_NMOS_GSD-Device Q?
U 1 1 5AB98D77
P 11150 6450
F 0 "Q?" H 11356 6496 50  0000 L CNN
F 1 "2V7002K" H 11356 6405 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 11350 6550 50  0001 C CNN
F 3 "~" H 11150 6450 50  0001 C CNN
	1    11150 6450
	-1   0    0    -1  
$EndComp
Wire Wire Line
	11050 6100 11050 6250
Text HLabel 12000 6450 2    50   Input ~ 0
EQ5
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98D7F
P 11750 6450
F 0 "R?" V 11957 6450 50  0000 C CNN
F 1 "2k" V 11866 6450 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11680 6450 50  0001 C CNN
F 3 "~" H 11750 6450 50  0001 C CNN
	1    11750 6450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	11900 6450 12000 6450
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98D86
P 11450 6800
F 0 "R?" H 11520 6846 50  0000 L CNN
F 1 "100k" H 11520 6755 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11380 6800 50  0001 C CNN
F 3 "~" H 11450 6800 50  0001 C CNN
	1    11450 6800
	1    0    0    -1  
$EndComp
Wire Wire Line
	11350 6450 11450 6450
Wire Wire Line
	11450 6650 11450 6450
Connection ~ 11450 6450
Wire Wire Line
	11450 6450 11600 6450
Wire Wire Line
	11450 6950 11450 7000
Wire Wire Line
	11450 7000 11650 7000
Connection ~ 12900 7000
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98D93
P 11800 7000
F 0 "R?" V 11593 7000 50  0000 C CNN
F 1 "100" V 11684 7000 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11730 7000 50  0001 C CNN
F 3 "~" H 11800 7000 50  0001 C CNN
	1    11800 7000
	0    1    1    0   
$EndComp
Wire Wire Line
	11950 7000 12200 7000
Wire Wire Line
	11050 6650 11050 7000
Wire Wire Line
	11050 7000 11450 7000
Connection ~ 11450 7000
Wire Wire Line
	11050 7000 10450 7000
Connection ~ 11050 7000
Text Label 10450 7000 0    50   ~ 0
BAT4
Connection ~ 12200 5650
Wire Wire Line
	12200 5650 12900 5650
Connection ~ 12200 4300
Wire Wire Line
	12200 4300 12900 4300
Text HLabel 13000 8350 2    50   Output ~ 0
VSENSE3
$Comp
L BMS-Master-rescue:D_Zener-Device D?
U 1 1 5AB98DA5
P 12900 7600
F 0 "D?" V 12854 7679 50  0000 L CNN
F 1 "DDZ9690-7" V 12945 7679 50  0000 L CNN
F 2 "Diodes_SMD:D_SOD-123" H 12900 7600 50  0001 C CNN
F 3 "~" H 12900 7600 50  0001 C CNN
	1    12900 7600
	0    1    1    0   
$EndComp
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AB98DAC
P 12200 7200
F 0 "C?" H 12315 7291 50  0000 L CNN
F 1 "0.1U" H 12315 7200 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 12238 7050 50  0001 C CNN
F 3 "~" H 12200 7200 50  0001 C CNN
F 4 "100V" H 12315 7109 50  0000 L CNN "Voltage"
	1    12200 7200
	1    0    0    -1  
$EndComp
Wire Wire Line
	12900 7450 12900 7000
Wire Wire Line
	12900 7750 12900 8350
Wire Wire Line
	12900 8350 13000 8350
Wire Wire Line
	12200 7050 12200 7000
Text HLabel 12350 7500 2    50   BiDi ~ 0
BAT0
Wire Wire Line
	12200 7350 12200 7500
Wire Wire Line
	12200 7500 12350 7500
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98DB9
P 11050 7300
F 0 "R?" H 10980 7254 50  0000 R CNN
F 1 "75" H 10980 7345 50  0000 R CNN
F 2 "Resistors_SMD:R_2512" V 10980 7300 50  0001 C CNN
F 3 "~" H 11050 7300 50  0001 C CNN
	1    11050 7300
	-1   0    0    1   
$EndComp
Wire Wire Line
	11050 7150 11050 7000
$Comp
L BMS-Master-rescue:Q_NMOS_GSD-Device Q?
U 1 1 5AB98DC0
P 11150 7800
F 0 "Q?" H 11356 7846 50  0000 L CNN
F 1 "2V7002K" H 11356 7755 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 11350 7900 50  0001 C CNN
F 3 "~" H 11150 7800 50  0001 C CNN
	1    11150 7800
	-1   0    0    -1  
$EndComp
Wire Wire Line
	11050 7450 11050 7600
Text HLabel 12000 7800 2    50   Input ~ 0
EQ4
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98DC8
P 11750 7800
F 0 "R?" V 11957 7800 50  0000 C CNN
F 1 "2k" V 11866 7800 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11680 7800 50  0001 C CNN
F 3 "~" H 11750 7800 50  0001 C CNN
	1    11750 7800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	11900 7800 12000 7800
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98DCF
P 11450 8150
F 0 "R?" H 11520 8196 50  0000 L CNN
F 1 "100k" H 11520 8105 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11380 8150 50  0001 C CNN
F 3 "~" H 11450 8150 50  0001 C CNN
	1    11450 8150
	1    0    0    -1  
$EndComp
Wire Wire Line
	11350 7800 11450 7800
Wire Wire Line
	11450 8000 11450 7800
Connection ~ 11450 7800
Wire Wire Line
	11450 7800 11600 7800
Wire Wire Line
	11450 8300 11450 8350
Wire Wire Line
	11450 8350 11650 8350
Connection ~ 12900 8350
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98DDC
P 11800 8350
F 0 "R?" V 11593 8350 50  0000 C CNN
F 1 "100" V 11684 8350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11730 8350 50  0001 C CNN
F 3 "~" H 11800 8350 50  0001 C CNN
	1    11800 8350
	0    1    1    0   
$EndComp
Wire Wire Line
	11950 8350 12200 8350
Wire Wire Line
	11050 8000 11050 8350
Wire Wire Line
	11050 8350 11450 8350
Connection ~ 11450 8350
Wire Wire Line
	11050 8350 10450 8350
Connection ~ 11050 8350
Text Label 10450 8350 0    50   ~ 0
BAT3
Text HLabel 13000 9700 2    50   Output ~ 0
VSENSE2
$Comp
L BMS-Master-rescue:D_Zener-Device D?
U 1 1 5AB98DEA
P 12900 8950
F 0 "D?" V 12854 9029 50  0000 L CNN
F 1 "DDZ9690-7" V 12945 9029 50  0000 L CNN
F 2 "Diodes_SMD:D_SOD-123" H 12900 8950 50  0001 C CNN
F 3 "~" H 12900 8950 50  0001 C CNN
	1    12900 8950
	0    1    1    0   
$EndComp
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AB98DF1
P 12200 8550
F 0 "C?" H 12315 8641 50  0000 L CNN
F 1 "0.1U" H 12315 8550 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 12238 8400 50  0001 C CNN
F 3 "~" H 12200 8550 50  0001 C CNN
F 4 "100V" H 12315 8459 50  0000 L CNN "Voltage"
	1    12200 8550
	1    0    0    -1  
$EndComp
Wire Wire Line
	12900 8800 12900 8350
Wire Wire Line
	12900 9100 12900 9700
Wire Wire Line
	12900 9700 13000 9700
Wire Wire Line
	12200 8400 12200 8350
Text HLabel 12350 8850 2    50   BiDi ~ 0
BAT0
Wire Wire Line
	12200 8700 12200 8850
Wire Wire Line
	12200 8850 12350 8850
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98DFE
P 11050 8650
F 0 "R?" H 10980 8604 50  0000 R CNN
F 1 "75" H 10980 8695 50  0000 R CNN
F 2 "Resistors_SMD:R_2512" V 10980 8650 50  0001 C CNN
F 3 "~" H 11050 8650 50  0001 C CNN
	1    11050 8650
	-1   0    0    1   
$EndComp
Wire Wire Line
	11050 8500 11050 8350
$Comp
L BMS-Master-rescue:Q_NMOS_GSD-Device Q?
U 1 1 5AB98E05
P 11150 9150
F 0 "Q?" H 11356 9196 50  0000 L CNN
F 1 "2V7002K" H 11356 9105 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 11350 9250 50  0001 C CNN
F 3 "~" H 11150 9150 50  0001 C CNN
	1    11150 9150
	-1   0    0    -1  
$EndComp
Wire Wire Line
	11050 8800 11050 8950
Text HLabel 12000 9150 2    50   Input ~ 0
EQ3
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98E0D
P 11750 9150
F 0 "R?" V 11957 9150 50  0000 C CNN
F 1 "2k" V 11866 9150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11680 9150 50  0001 C CNN
F 3 "~" H 11750 9150 50  0001 C CNN
	1    11750 9150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	11900 9150 12000 9150
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98E14
P 11450 9500
F 0 "R?" H 11520 9546 50  0000 L CNN
F 1 "100k" H 11520 9455 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11380 9500 50  0001 C CNN
F 3 "~" H 11450 9500 50  0001 C CNN
	1    11450 9500
	1    0    0    -1  
$EndComp
Wire Wire Line
	11350 9150 11450 9150
Wire Wire Line
	11450 9350 11450 9150
Connection ~ 11450 9150
Wire Wire Line
	11450 9150 11600 9150
Wire Wire Line
	11450 9650 11450 9700
Wire Wire Line
	11450 9700 11650 9700
Connection ~ 12900 9700
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98E21
P 11800 9700
F 0 "R?" V 11593 9700 50  0000 C CNN
F 1 "100" V 11684 9700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11730 9700 50  0001 C CNN
F 3 "~" H 11800 9700 50  0001 C CNN
	1    11800 9700
	0    1    1    0   
$EndComp
Wire Wire Line
	11950 9700 12200 9700
Wire Wire Line
	11050 9350 11050 9700
Wire Wire Line
	11050 9700 11450 9700
Connection ~ 11450 9700
Wire Wire Line
	11050 9700 10450 9700
Connection ~ 11050 9700
Text Label 10450 9700 0    50   ~ 0
BAT2
Connection ~ 12200 8350
Wire Wire Line
	12200 8350 12900 8350
Text HLabel 13000 11050 2    50   Output ~ 0
VSENSE1
$Comp
L BMS-Master-rescue:D_Zener-Device D?
U 1 1 5AB98E31
P 12900 10300
F 0 "D?" V 12854 10379 50  0000 L CNN
F 1 "DDZ9690-7" V 12945 10379 50  0000 L CNN
F 2 "Diodes_SMD:D_SOD-123" H 12900 10300 50  0001 C CNN
F 3 "~" H 12900 10300 50  0001 C CNN
	1    12900 10300
	0    1    1    0   
$EndComp
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AB98E38
P 12200 9900
F 0 "C?" H 12315 9991 50  0000 L CNN
F 1 "0.1U" H 12315 9900 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 12238 9750 50  0001 C CNN
F 3 "~" H 12200 9900 50  0001 C CNN
F 4 "100V" H 12315 9809 50  0000 L CNN "Voltage"
	1    12200 9900
	1    0    0    -1  
$EndComp
Wire Wire Line
	12900 10150 12900 9700
Wire Wire Line
	12900 10450 12900 11050
Wire Wire Line
	12900 11050 13000 11050
Wire Wire Line
	12200 9750 12200 9700
Text HLabel 12350 10200 2    50   BiDi ~ 0
BAT0
Wire Wire Line
	12200 10050 12200 10200
Wire Wire Line
	12200 10200 12350 10200
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98E45
P 11050 10000
F 0 "R?" H 10980 9954 50  0000 R CNN
F 1 "75" H 10980 10045 50  0000 R CNN
F 2 "Resistors_SMD:R_2512" V 10980 10000 50  0001 C CNN
F 3 "~" H 11050 10000 50  0001 C CNN
	1    11050 10000
	-1   0    0    1   
$EndComp
Wire Wire Line
	11050 9850 11050 9700
$Comp
L BMS-Master-rescue:Q_NMOS_GSD-Device Q?
U 1 1 5AB98E4C
P 11150 10500
F 0 "Q?" H 11356 10546 50  0000 L CNN
F 1 "2V7002K" H 11356 10455 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 11350 10600 50  0001 C CNN
F 3 "~" H 11150 10500 50  0001 C CNN
	1    11150 10500
	-1   0    0    -1  
$EndComp
Wire Wire Line
	11050 10150 11050 10300
Text HLabel 12000 10500 2    50   Input ~ 0
EQ2
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98E54
P 11750 10500
F 0 "R?" V 11957 10500 50  0000 C CNN
F 1 "2k" V 11866 10500 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11680 10500 50  0001 C CNN
F 3 "~" H 11750 10500 50  0001 C CNN
	1    11750 10500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	11900 10500 12000 10500
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98E5B
P 11450 10850
F 0 "R?" H 11520 10896 50  0000 L CNN
F 1 "100k" H 11520 10805 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11380 10850 50  0001 C CNN
F 3 "~" H 11450 10850 50  0001 C CNN
	1    11450 10850
	1    0    0    -1  
$EndComp
Wire Wire Line
	11350 10500 11450 10500
Wire Wire Line
	11450 10700 11450 10500
Connection ~ 11450 10500
Wire Wire Line
	11450 10500 11600 10500
Wire Wire Line
	11450 11000 11450 11050
Wire Wire Line
	11450 11050 11650 11050
Connection ~ 12900 11050
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98E68
P 11800 11050
F 0 "R?" V 11593 11050 50  0000 C CNN
F 1 "100" V 11684 11050 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11730 11050 50  0001 C CNN
F 3 "~" H 11800 11050 50  0001 C CNN
	1    11800 11050
	0    1    1    0   
$EndComp
Wire Wire Line
	11950 11050 12200 11050
Wire Wire Line
	11050 10700 11050 11050
Wire Wire Line
	11050 11050 11450 11050
Connection ~ 11450 11050
Wire Wire Line
	11050 11050 10450 11050
Connection ~ 11050 11050
Text Label 10450 11050 0    50   ~ 0
BAT1
Text HLabel 13000 12400 2    50   Output ~ 0
VSENSE0
$Comp
L BMS-Master-rescue:D_Zener-Device D?
U 1 1 5AB98E76
P 12900 11650
F 0 "D?" V 12854 11729 50  0000 L CNN
F 1 "DDZ9690-7" V 12945 11729 50  0000 L CNN
F 2 "Diodes_SMD:D_SOD-123" H 12900 11650 50  0001 C CNN
F 3 "~" H 12900 11650 50  0001 C CNN
	1    12900 11650
	0    1    1    0   
$EndComp
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AB98E7D
P 12200 11250
F 0 "C?" H 12315 11341 50  0000 L CNN
F 1 "0.1U" H 12315 11250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 12238 11100 50  0001 C CNN
F 3 "~" H 12200 11250 50  0001 C CNN
F 4 "100V" H 12315 11159 50  0000 L CNN "Voltage"
	1    12200 11250
	1    0    0    -1  
$EndComp
Wire Wire Line
	12900 11500 12900 11050
Wire Wire Line
	12900 11800 12900 12400
Wire Wire Line
	12900 12400 13000 12400
Wire Wire Line
	12200 11100 12200 11050
Text HLabel 12350 11550 2    50   BiDi ~ 0
BAT0
Wire Wire Line
	12200 11400 12200 11550
Wire Wire Line
	12200 11550 12350 11550
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98E8A
P 11050 11350
F 0 "R?" H 10980 11304 50  0000 R CNN
F 1 "75" H 10980 11395 50  0000 R CNN
F 2 "Resistors_SMD:R_2512" V 10980 11350 50  0001 C CNN
F 3 "~" H 11050 11350 50  0001 C CNN
	1    11050 11350
	-1   0    0    1   
$EndComp
Wire Wire Line
	11050 11200 11050 11050
$Comp
L BMS-Master-rescue:Q_NMOS_GSD-Device Q?
U 1 1 5AB98E91
P 11150 11850
F 0 "Q?" H 11356 11896 50  0000 L CNN
F 1 "2V7002K" H 11356 11805 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 11350 11950 50  0001 C CNN
F 3 "~" H 11150 11850 50  0001 C CNN
	1    11150 11850
	-1   0    0    -1  
$EndComp
Wire Wire Line
	11050 11500 11050 11650
Text HLabel 12000 11850 2    50   Input ~ 0
EQ1
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98E99
P 11750 11850
F 0 "R?" V 11957 11850 50  0000 C CNN
F 1 "2k" V 11866 11850 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11680 11850 50  0001 C CNN
F 3 "~" H 11750 11850 50  0001 C CNN
	1    11750 11850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	11900 11850 12000 11850
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AB98EA0
P 11450 12200
F 0 "R?" H 11520 12246 50  0000 L CNN
F 1 "100k" H 11520 12155 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11380 12200 50  0001 C CNN
F 3 "~" H 11450 12200 50  0001 C CNN
	1    11450 12200
	1    0    0    -1  
$EndComp
Wire Wire Line
	11350 11850 11450 11850
Wire Wire Line
	11450 12050 11450 11850
Connection ~ 11450 11850
Wire Wire Line
	11450 11850 11600 11850
Wire Wire Line
	11450 12350 11450 12400
Wire Wire Line
	11050 12050 11050 12400
Wire Wire Line
	11050 12400 11450 12400
Wire Wire Line
	11050 12400 10450 12400
Connection ~ 11050 12400
Text Label 10450 12400 0    50   ~ 0
BAT0S
Connection ~ 12200 11050
Wire Wire Line
	12200 11050 12900 11050
Connection ~ 12200 9700
Wire Wire Line
	12200 9700 12900 9700
Connection ~ 12200 7000
Wire Wire Line
	12200 7000 12900 7000
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5ABB5239
P 12000 12400
F 0 "R?" V 11793 12400 50  0000 C CNN
F 1 "100" V 11884 12400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11930 12400 50  0001 C CNN
F 3 "~" H 12000 12400 50  0001 C CNN
	1    12000 12400
	0    1    1    0   
$EndComp
Wire Wire Line
	11450 12400 11850 12400
Connection ~ 11450 12400
Wire Wire Line
	12150 12400 12900 12400
Connection ~ 12900 12400
$Comp
L BMS-Master-rescue:D_Zener-Device D?
U 1 1 5ABE0118
P 5400 13800
F 0 "D?" V 5354 13879 50  0000 L CNN
F 1 "SMAJ90A" V 5445 13879 50  0000 L CNN
F 2 "Diodes_SMD:D_SMA" H 5400 13800 50  0001 C CNN
F 3 "~" H 5400 13800 50  0001 C CNN
	1    5400 13800
	0    1    1    0   
$EndComp
Wire Wire Line
	5400 13650 5400 13450
Wire Wire Line
	5400 13450 4950 13450
Wire Wire Line
	5400 13950 5400 14200
Wire Wire Line
	5400 14200 4950 14200
Text Label 4950 14200 0    50   ~ 0
BAT0
Text Label 4950 13450 0    50   ~ 0
BAT16
Wire Wire Line
	1900 1900 1250 1900
Wire Wire Line
	1900 2000 1250 2000
Wire Wire Line
	1900 2100 1250 2100
Wire Wire Line
	1900 2200 1250 2200
Wire Wire Line
	1900 2300 1250 2300
Wire Wire Line
	1900 2400 1250 2400
Wire Wire Line
	1900 2500 1250 2500
Wire Wire Line
	1900 2600 1250 2600
Wire Wire Line
	1900 2700 1250 2700
Wire Wire Line
	1900 2800 1250 2800
Wire Wire Line
	1900 2900 1250 2900
Wire Wire Line
	1900 3000 1250 3000
Wire Wire Line
	1250 3200 1900 3200
Wire Wire Line
	1250 3100 1900 3100
Wire Wire Line
	1900 3300 1250 3300
Wire Wire Line
	1900 3400 1250 3400
Wire Wire Line
	1900 3500 1250 3500
Wire Wire Line
	1900 3600 1250 3600
Wire Wire Line
	1900 3700 1250 3700
Wire Wire Line
	1900 3800 1250 3800
Wire Wire Line
	1250 4000 1900 4000
Text Label 1900 2000 0    50   ~ 0
BAT16
Text Label 1900 2100 0    50   ~ 0
BAT16S
Text Label 1900 1900 0    50   ~ 0
BAT0
Text Label 1900 2200 0    50   ~ 0
BAT15
Text Label 1900 2300 0    50   ~ 0
BAT14
Text Label 1900 2400 0    50   ~ 0
BAT13
Text Label 1900 2500 0    50   ~ 0
BAT12
Text Label 1900 2600 0    50   ~ 0
BAT11
Text Label 1900 2700 0    50   ~ 0
BAT10
Text Label 1900 2800 0    50   ~ 0
BAT9
Text Label 1900 2900 0    50   ~ 0
BAT8
Text Label 1900 3000 0    50   ~ 0
BAT7
Text Label 1900 3100 0    50   ~ 0
BAT6
Text Label 1900 3200 0    50   ~ 0
BAT5
Text Label 1900 3300 0    50   ~ 0
BAT4
Text Label 1900 3400 0    50   ~ 0
BAT3
Text Label 1900 3500 0    50   ~ 0
BAT2
Text Label 1900 3600 0    50   ~ 0
BAT1
Text Label 1900 3700 0    50   ~ 0
BAT0S
Text Label 1900 3800 0    50   ~ 0
BAT0
Text Label 1900 3900 0    50   ~ 0
BAT16
Text Label 1900 4000 0    50   ~ 0
BAT0
Text HLabel 2300 3900 2    50   BiDi ~ 0
BAT16
Wire Wire Line
	1250 3900 2300 3900
$EndSCHEMATC
