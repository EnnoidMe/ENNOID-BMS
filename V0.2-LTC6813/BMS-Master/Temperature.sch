EESchema Schematic File Version 4
LIBS:BMS-Master-cache
EELAYER 26 0
EELAYER END
$Descr A2 23386 16535
encoding utf-8
Sheet 2 25
Title "BMS-Master"
Date ""
Rev "V0.1"
Comp "ENNOID"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L BMS-Master-rescue:Conn_02x08_Counter_Clockwise-Connector_Generic J?
U 1 1 5AEA9148
P 8500 9050
F 0 "J?" H 8550 9567 50  0000 C CNN
F 1 "Conn_02x08_Counter_Clockwise" H 8550 9476 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_2x08_Pitch2.54mm" H 8500 9050 50  0001 C CNN
F 3 "~" H 8500 9050 50  0001 C CNN
	1    8500 9050
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5AEA9211
P 7850 9650
F 0 "#PWR?" H 7850 9400 50  0001 C CNN
F 1 "GND" H 7855 9477 50  0000 C CNN
F 2 "" H 7850 9650 50  0001 C CNN
F 3 "" H 7850 9650 50  0001 C CNN
	1    7850 9650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 9650 7850 9450
Wire Wire Line
	7850 8750 8300 8750
Wire Wire Line
	8300 8850 7850 8850
Connection ~ 7850 8850
Wire Wire Line
	7850 8850 7850 8750
Wire Wire Line
	8300 8950 7850 8950
Connection ~ 7850 8950
Wire Wire Line
	7850 8950 7850 8850
Wire Wire Line
	8300 9050 7850 9050
Connection ~ 7850 9050
Wire Wire Line
	7850 9050 7850 8950
Wire Wire Line
	8300 9150 7850 9150
Connection ~ 7850 9150
Wire Wire Line
	7850 9150 7850 9050
Wire Wire Line
	8300 9250 7850 9250
Connection ~ 7850 9250
Wire Wire Line
	7850 9250 7850 9150
Wire Wire Line
	8300 9350 7850 9350
Connection ~ 7850 9350
Wire Wire Line
	7850 9350 7850 9250
Wire Wire Line
	8300 9450 7850 9450
Connection ~ 7850 9450
Wire Wire Line
	7850 9450 7850 9350
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AEA9568
P 11450 3150
F 0 "R?" H 11520 3196 50  0000 L CNN
F 1 "10k" H 11520 3105 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11380 3150 50  0001 C CNN
F 3 "~" H 11450 3150 50  0001 C CNN
	1    11450 3150
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AEA95F3
P 10950 3350
F 0 "R?" V 10743 3350 50  0000 C CNN
F 1 "10" V 10834 3350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 10880 3350 50  0001 C CNN
F 3 "~" H 10950 3350 50  0001 C CNN
	1    10950 3350
	0    1    1    0   
$EndComp
Wire Wire Line
	11450 3300 11450 3350
Wire Wire Line
	11450 3350 11100 3350
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AEA973B
P 11450 3700
F 0 "C?" H 11565 3746 50  0000 L CNN
F 1 "0.1u" H 11565 3655 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 11488 3550 50  0001 C CNN
F 3 "~" H 11450 3700 50  0001 C CNN
F 4 "50V" H 11450 3700 50  0001 C CNN "Volatge"
	1    11450 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	11450 3550 11450 3350
Connection ~ 11450 3350
Text HLabel 11850 3350 2    50   Output ~ 0
AUX7
Wire Wire Line
	11450 3350 11850 3350
Wire Wire Line
	11450 3000 11450 2900
Wire Wire Line
	11450 2900 10900 2900
Text Label 10900 2900 0    50   ~ 0
VP1
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5AEA9CA3
P 11450 3950
F 0 "#PWR?" H 11450 3700 50  0001 C CNN
F 1 "GND" H 11455 3777 50  0000 C CNN
F 2 "" H 11450 3950 50  0001 C CNN
F 3 "" H 11450 3950 50  0001 C CNN
	1    11450 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	11450 3850 11450 3950
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AEAA2B3
P 11450 4600
F 0 "R?" H 11520 4646 50  0000 L CNN
F 1 "10k" H 11520 4555 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11380 4600 50  0001 C CNN
F 3 "~" H 11450 4600 50  0001 C CNN
	1    11450 4600
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AEAA2B9
P 10950 4800
F 0 "R?" V 10743 4800 50  0000 C CNN
F 1 "10" V 10834 4800 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 10880 4800 50  0001 C CNN
F 3 "~" H 10950 4800 50  0001 C CNN
	1    10950 4800
	0    1    1    0   
$EndComp
Wire Wire Line
	11450 4750 11450 4800
Wire Wire Line
	11450 4800 11100 4800
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AEAA2C2
P 11450 5150
F 0 "C?" H 11565 5196 50  0000 L CNN
F 1 "0.1u" H 11565 5105 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 11488 5000 50  0001 C CNN
F 3 "~" H 11450 5150 50  0001 C CNN
F 4 "50V" H 11450 5150 50  0001 C CNN "Volatge"
	1    11450 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	11450 5000 11450 4800
Connection ~ 11450 4800
Text HLabel 11850 4800 2    50   Output ~ 0
AUX6
Wire Wire Line
	11450 4800 11850 4800
Wire Wire Line
	11450 4450 11450 4350
Wire Wire Line
	11450 4350 10900 4350
Text Label 10900 4350 0    50   ~ 0
VP1
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5AEAA2CF
P 11450 5400
F 0 "#PWR?" H 11450 5150 50  0001 C CNN
F 1 "GND" H 11455 5227 50  0000 C CNN
F 2 "" H 11450 5400 50  0001 C CNN
F 3 "" H 11450 5400 50  0001 C CNN
	1    11450 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	11450 5300 11450 5400
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AEAA88D
P 11450 5950
F 0 "R?" H 11520 5996 50  0000 L CNN
F 1 "10k" H 11520 5905 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11380 5950 50  0001 C CNN
F 3 "~" H 11450 5950 50  0001 C CNN
	1    11450 5950
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AEAA893
P 10950 6150
F 0 "R?" V 10743 6150 50  0000 C CNN
F 1 "10" V 10834 6150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 10880 6150 50  0001 C CNN
F 3 "~" H 10950 6150 50  0001 C CNN
	1    10950 6150
	0    1    1    0   
$EndComp
Wire Wire Line
	11450 6100 11450 6150
Wire Wire Line
	11450 6150 11100 6150
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AEAA89C
P 11450 6500
F 0 "C?" H 11565 6546 50  0000 L CNN
F 1 "0.1u" H 11565 6455 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 11488 6350 50  0001 C CNN
F 3 "~" H 11450 6500 50  0001 C CNN
F 4 "50V" H 11450 6500 50  0001 C CNN "Volatge"
	1    11450 6500
	1    0    0    -1  
$EndComp
Wire Wire Line
	11450 6350 11450 6150
Connection ~ 11450 6150
Text HLabel 11850 6150 2    50   Output ~ 0
AUX5
Wire Wire Line
	11450 6150 11850 6150
Wire Wire Line
	11450 5800 11450 5700
Wire Wire Line
	11450 5700 10900 5700
Text Label 10900 5700 0    50   ~ 0
VP1
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5AEAA8A9
P 11450 6750
F 0 "#PWR?" H 11450 6500 50  0001 C CNN
F 1 "GND" H 11455 6577 50  0000 C CNN
F 2 "" H 11450 6750 50  0001 C CNN
F 3 "" H 11450 6750 50  0001 C CNN
	1    11450 6750
	1    0    0    -1  
$EndComp
Wire Wire Line
	11450 6650 11450 6750
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AEAA8B0
P 11450 7400
F 0 "R?" H 11520 7446 50  0000 L CNN
F 1 "10k" H 11520 7355 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11380 7400 50  0001 C CNN
F 3 "~" H 11450 7400 50  0001 C CNN
	1    11450 7400
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AEAA8B6
P 10950 7600
F 0 "R?" V 10743 7600 50  0000 C CNN
F 1 "10" V 10834 7600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 10880 7600 50  0001 C CNN
F 3 "~" H 10950 7600 50  0001 C CNN
	1    10950 7600
	0    1    1    0   
$EndComp
Wire Wire Line
	11450 7550 11450 7600
Wire Wire Line
	11450 7600 11100 7600
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AEAA8BF
P 11450 7950
F 0 "C?" H 11565 7996 50  0000 L CNN
F 1 "0.1u" H 11565 7905 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 11488 7800 50  0001 C CNN
F 3 "~" H 11450 7950 50  0001 C CNN
F 4 "50V" H 11450 7950 50  0001 C CNN "Volatge"
	1    11450 7950
	1    0    0    -1  
$EndComp
Wire Wire Line
	11450 7800 11450 7600
Connection ~ 11450 7600
Text HLabel 11850 7600 2    50   Output ~ 0
AUX4
Wire Wire Line
	11450 7600 11850 7600
Wire Wire Line
	11450 7250 11450 7150
Wire Wire Line
	11450 7150 10900 7150
Text Label 10900 7150 0    50   ~ 0
VP1
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5AEAA8CC
P 11450 8200
F 0 "#PWR?" H 11450 7950 50  0001 C CNN
F 1 "GND" H 11455 8027 50  0000 C CNN
F 2 "" H 11450 8200 50  0001 C CNN
F 3 "" H 11450 8200 50  0001 C CNN
	1    11450 8200
	1    0    0    -1  
$EndComp
Wire Wire Line
	11450 8100 11450 8200
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AEAB9E1
P 11450 8750
F 0 "R?" H 11520 8796 50  0000 L CNN
F 1 "10k" H 11520 8705 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11380 8750 50  0001 C CNN
F 3 "~" H 11450 8750 50  0001 C CNN
	1    11450 8750
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AEAB9E7
P 10950 8950
F 0 "R?" V 10743 8950 50  0000 C CNN
F 1 "10" V 10834 8950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 10880 8950 50  0001 C CNN
F 3 "~" H 10950 8950 50  0001 C CNN
	1    10950 8950
	0    1    1    0   
$EndComp
Wire Wire Line
	11450 8900 11450 8950
Wire Wire Line
	11450 8950 11100 8950
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AEAB9F0
P 11450 9300
F 0 "C?" H 11565 9346 50  0000 L CNN
F 1 "0.1u" H 11565 9255 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 11488 9150 50  0001 C CNN
F 3 "~" H 11450 9300 50  0001 C CNN
F 4 "50V" H 11450 9300 50  0001 C CNN "Volatge"
	1    11450 9300
	1    0    0    -1  
$EndComp
Wire Wire Line
	11450 9150 11450 8950
Connection ~ 11450 8950
Text HLabel 11850 8950 2    50   Output ~ 0
AUX3
Wire Wire Line
	11450 8950 11850 8950
Wire Wire Line
	11450 8600 11450 8500
Wire Wire Line
	11450 8500 10900 8500
Text Label 10900 8500 0    50   ~ 0
VP1
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5AEAB9FD
P 11450 9550
F 0 "#PWR?" H 11450 9300 50  0001 C CNN
F 1 "GND" H 11455 9377 50  0000 C CNN
F 2 "" H 11450 9550 50  0001 C CNN
F 3 "" H 11450 9550 50  0001 C CNN
	1    11450 9550
	1    0    0    -1  
$EndComp
Wire Wire Line
	11450 9450 11450 9550
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AEABA04
P 11450 10200
F 0 "R?" H 11520 10246 50  0000 L CNN
F 1 "10k" H 11520 10155 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11380 10200 50  0001 C CNN
F 3 "~" H 11450 10200 50  0001 C CNN
	1    11450 10200
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AEABA0A
P 10950 10400
F 0 "R?" V 10743 10400 50  0000 C CNN
F 1 "10" V 10834 10400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 10880 10400 50  0001 C CNN
F 3 "~" H 10950 10400 50  0001 C CNN
	1    10950 10400
	0    1    1    0   
$EndComp
Wire Wire Line
	11450 10350 11450 10400
Wire Wire Line
	11450 10400 11100 10400
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AEABA13
P 11450 10750
F 0 "C?" H 11565 10796 50  0000 L CNN
F 1 "0.1u" H 11565 10705 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 11488 10600 50  0001 C CNN
F 3 "~" H 11450 10750 50  0001 C CNN
F 4 "50V" H 11450 10750 50  0001 C CNN "Volatge"
	1    11450 10750
	1    0    0    -1  
$EndComp
Wire Wire Line
	11450 10600 11450 10400
Connection ~ 11450 10400
Text HLabel 11850 10400 2    50   Output ~ 0
AUX2
Wire Wire Line
	11450 10400 11850 10400
Wire Wire Line
	11450 10050 11450 9950
Wire Wire Line
	11450 9950 10900 9950
Text Label 10900 9950 0    50   ~ 0
VP1
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5AEABA20
P 11450 11000
F 0 "#PWR?" H 11450 10750 50  0001 C CNN
F 1 "GND" H 11455 10827 50  0000 C CNN
F 2 "" H 11450 11000 50  0001 C CNN
F 3 "" H 11450 11000 50  0001 C CNN
	1    11450 11000
	1    0    0    -1  
$EndComp
Wire Wire Line
	11450 10900 11450 11000
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AEABA27
P 11450 11550
F 0 "R?" H 11520 11596 50  0000 L CNN
F 1 "10k" H 11520 11505 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11380 11550 50  0001 C CNN
F 3 "~" H 11450 11550 50  0001 C CNN
	1    11450 11550
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AEABA2D
P 10950 11750
F 0 "R?" V 10743 11750 50  0000 C CNN
F 1 "10" V 10834 11750 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 10880 11750 50  0001 C CNN
F 3 "~" H 10950 11750 50  0001 C CNN
	1    10950 11750
	0    1    1    0   
$EndComp
Wire Wire Line
	11450 11700 11450 11750
Wire Wire Line
	11450 11750 11100 11750
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AEABA36
P 11450 12100
F 0 "C?" H 11565 12146 50  0000 L CNN
F 1 "0.1u" H 11565 12055 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 11488 11950 50  0001 C CNN
F 3 "~" H 11450 12100 50  0001 C CNN
F 4 "50V" H 11450 12100 50  0001 C CNN "Volatge"
	1    11450 12100
	1    0    0    -1  
$EndComp
Wire Wire Line
	11450 11950 11450 11750
Connection ~ 11450 11750
Text HLabel 11850 11750 2    50   Output ~ 0
AUX1
Wire Wire Line
	11450 11750 11850 11750
Wire Wire Line
	11450 11400 11450 11300
Wire Wire Line
	11450 11300 10900 11300
Text Label 10900 11300 0    50   ~ 0
VP1
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5AEABA43
P 11450 12350
F 0 "#PWR?" H 11450 12100 50  0001 C CNN
F 1 "GND" H 11455 12177 50  0000 C CNN
F 2 "" H 11450 12350 50  0001 C CNN
F 3 "" H 11450 12350 50  0001 C CNN
	1    11450 12350
	1    0    0    -1  
$EndComp
Wire Wire Line
	11450 12250 11450 12350
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AEABA4A
P 11450 13000
F 0 "R?" H 11520 13046 50  0000 L CNN
F 1 "10k" H 11520 12955 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 11380 13000 50  0001 C CNN
F 3 "~" H 11450 13000 50  0001 C CNN
	1    11450 13000
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R?
U 1 1 5AEABA50
P 10950 13200
F 0 "R?" V 10743 13200 50  0000 C CNN
F 1 "10" V 10834 13200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 10880 13200 50  0001 C CNN
F 3 "~" H 10950 13200 50  0001 C CNN
	1    10950 13200
	0    1    1    0   
$EndComp
Wire Wire Line
	11450 13150 11450 13200
Wire Wire Line
	11450 13200 11100 13200
$Comp
L BMS-Master-rescue:C-Device C?
U 1 1 5AEABA59
P 11450 13550
F 0 "C?" H 11565 13596 50  0000 L CNN
F 1 "0.1u" H 11565 13505 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 11488 13400 50  0001 C CNN
F 3 "~" H 11450 13550 50  0001 C CNN
F 4 "50V" H 11450 13550 50  0001 C CNN "Volatge"
	1    11450 13550
	1    0    0    -1  
$EndComp
Wire Wire Line
	11450 13400 11450 13200
Connection ~ 11450 13200
Text HLabel 11850 13200 2    50   Output ~ 0
AUX0
Wire Wire Line
	11450 13200 11850 13200
Wire Wire Line
	11450 12850 11450 12750
Wire Wire Line
	11450 12750 10900 12750
Text Label 10900 12750 0    50   ~ 0
VP1
$Comp
L BMS-Master-rescue:GND-power #PWR?
U 1 1 5AEABA66
P 11450 13800
F 0 "#PWR?" H 11450 13550 50  0001 C CNN
F 1 "GND" H 11455 13627 50  0000 C CNN
F 2 "" H 11450 13800 50  0001 C CNN
F 3 "" H 11450 13800 50  0001 C CNN
	1    11450 13800
	1    0    0    -1  
$EndComp
Wire Wire Line
	11450 13700 11450 13800
Wire Wire Line
	10800 3350 9350 3350
Wire Wire Line
	9350 3350 9350 8750
Wire Wire Line
	9350 8750 8800 8750
Wire Wire Line
	10800 4800 9450 4800
Wire Wire Line
	9450 4800 9450 8850
Wire Wire Line
	9450 8850 8800 8850
Wire Wire Line
	10800 6150 9550 6150
Wire Wire Line
	9550 6150 9550 8950
Wire Wire Line
	9550 8950 8800 8950
Wire Wire Line
	10800 7600 9650 7600
Wire Wire Line
	9650 7600 9650 9050
Wire Wire Line
	9650 9050 8800 9050
Wire Wire Line
	10800 8950 9750 8950
Wire Wire Line
	9750 8950 9750 9150
Wire Wire Line
	9750 9150 8800 9150
Wire Wire Line
	10800 10400 9750 10400
Wire Wire Line
	9750 10400 9750 9250
Wire Wire Line
	9750 9250 8800 9250
Wire Wire Line
	10800 11750 9650 11750
Wire Wire Line
	9650 11750 9650 9350
Wire Wire Line
	9650 9350 8800 9350
Wire Wire Line
	10800 13200 9550 13200
Wire Wire Line
	9550 13200 9550 9450
Wire Wire Line
	9550 9450 8800 9450
$EndSCHEMATC
