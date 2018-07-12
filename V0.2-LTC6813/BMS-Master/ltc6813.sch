EESchema Schematic File Version 4
LIBS:BMS-Master-cache
EELAYER 26 0
EELAYER END
$Descr A2 23386 16535
encoding utf-8
Sheet 7 25
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
L Device:C C?
U 1 1 5B4D1ECD
P 4850 4350
F 0 "C?" V 4598 4350 50  0000 C CNN
F 1 "100nF" V 4689 4350 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad0.99x1.00mm_HandSolder" H 4888 4200 50  0001 C CNN
F 3 "~" H 4850 4350 50  0001 C CNN
	1    4850 4350
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 5B4D206C
P 4550 4550
F 0 "R?" V 4343 4550 50  0000 C CNN
F 1 "100" V 4434 4550 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.99x1.00mm_HandSolder" V 4480 4550 50  0001 C CNN
F 3 "~" H 4550 4550 50  0001 C CNN
	1    4550 4550
	0    1    1    0   
$EndComp
Wire Wire Line
	4700 4550 4700 4350
$Comp
L ENNOID:LTC6813 U?
U 1 1 5B4D2B36
P 9150 5300
F 0 "U?" H 9150 5400 50  0000 L CNN
F 1 "LTC6813" H 9000 5300 50  0000 L CNN
F 2 "" H 9100 5100 50  0001 C CNN
F 3 "" H 9100 5100 50  0001 C CNN
	1    9150 5300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5B4D2FBF
P 8800 3750
F 0 "#PWR?" H 8800 3500 50  0001 C CNN
F 1 "GND" H 8805 3577 50  0000 C CNN
F 2 "" H 8800 3750 50  0001 C CNN
F 3 "" H 8800 3750 50  0001 C CNN
	1    8800 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	8900 4100 8900 4000
Wire Wire Line
	8900 3700 8800 3700
Wire Wire Line
	8800 3700 8800 3750
$Comp
L Connector:Conn_01x19_Male J?
U 1 1 5B4F550B
P 1300 5550
F 0 "J?" H 1272 5480 50  0000 R CNN
F 1 "Conn_01x19_Male" H 1272 5571 50  0000 R CNN
F 2 "" H 1300 5550 50  0001 C CNN
F 3 "~" H 1300 5550 50  0001 C CNN
	1    1300 5550
	1    0    0    1   
$EndComp
Connection ~ 4700 4550
Wire Wire Line
	5000 4350 5050 4350
$Sheet
S 4350 14700 500  400 
U 5B4FCC47
F0 "cell1" 50
F1 "cell.sch" 50
F2 "S" I R 4850 15000 50 
F3 "V-" I R 4850 14900 50 
F4 "C" I R 4850 14800 50 
F5 "C+" I L 4350 14800 50 
$EndSheet
$Sheet
S 4350 14150 500  400 
U 5B4FD503
F0 "cell2" 50
F1 "cell.sch" 50
F2 "S" I R 4850 14450 50 
F3 "V-" I R 4850 14350 50 
F4 "C" I R 4850 14250 50 
F5 "C+" I L 4350 14250 50 
$EndSheet
Text Label 5100 15000 0    50   ~ 0
S1
Text Label 5100 14800 0    50   ~ 0
C1
$Comp
L Device:R R?
U 1 1 5B4FEA87
P 1900 6600
F 0 "R?" V 1693 6600 50  0000 C CNN
F 1 "100" V 1784 6600 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.99x1.00mm_HandSolder" V 1830 6600 50  0001 C CNN
F 3 "~" H 1900 6600 50  0001 C CNN
	1    1900 6600
	0    1    1    0   
$EndComp
Wire Wire Line
	1500 6600 1750 6600
Wire Wire Line
	2300 6600 2050 6600
$Comp
L power:GND #PWR?
U 1 1 5B4FEFB5
P 1500 6750
F 0 "#PWR?" H 1500 6500 50  0001 C CNN
F 1 "GND" H 1505 6577 50  0000 C CNN
F 2 "" H 1500 6750 50  0001 C CNN
F 3 "" H 1500 6750 50  0001 C CNN
	1    1500 6750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 6750 1500 6600
Connection ~ 1500 6600
Text Label 5100 14450 0    50   ~ 0
S2
Text Label 5100 14250 0    50   ~ 0
C2
$Sheet
S 4350 13600 500  400 
U 5B50012A
F0 "cell3" 50
F1 "cell.sch" 50
F2 "S" I R 4850 13900 50 
F3 "V-" I R 4850 13800 50 
F4 "C" I R 4850 13700 50 
F5 "C+" I L 4350 13700 50 
$EndSheet
Text Label 5100 13900 0    50   ~ 0
S3
Text Label 5100 13700 0    50   ~ 0
C3
$Sheet
S 4350 13050 500  400 
U 5B502FEF
F0 "cell4" 50
F1 "cell.sch" 50
F2 "S" I R 4850 13350 50 
F3 "V-" I R 4850 13250 50 
F4 "C" I R 4850 13150 50 
F5 "C+" I L 4350 13150 50 
$EndSheet
Text Label 5100 13350 0    50   ~ 0
S4
Text Label 5100 13150 0    50   ~ 0
C4
$Sheet
S 4350 12500 500  400 
U 5B50355C
F0 "cell5" 50
F1 "cell.sch" 50
F2 "S" I R 4850 12800 50 
F3 "V-" I R 4850 12700 50 
F4 "C" I R 4850 12600 50 
F5 "C+" I L 4350 12600 50 
$EndSheet
Text Label 5100 12800 0    50   ~ 0
S5
Text Label 5100 12600 0    50   ~ 0
C5
$Sheet
S 4350 11950 500  400 
U 5B504105
F0 "cell6" 50
F1 "cell.sch" 50
F2 "S" I R 4850 12250 50 
F3 "V-" I R 4850 12150 50 
F4 "C" I R 4850 12050 50 
F5 "C+" I L 4350 12050 50 
$EndSheet
Text Label 5100 12250 0    50   ~ 0
S6
Text Label 5100 12050 0    50   ~ 0
C6
$Sheet
S 4350 11400 500  400 
U 5B504814
F0 "cell7" 50
F1 "cell.sch" 50
F2 "S" I R 4850 11700 50 
F3 "V-" I R 4850 11600 50 
F4 "C" I R 4850 11500 50 
F5 "C+" I L 4350 11500 50 
$EndSheet
Text Label 5100 11700 0    50   ~ 0
S7
Text Label 5100 11500 0    50   ~ 0
C7
$Sheet
S 4350 10850 500  400 
U 5B5050BC
F0 "cell8" 50
F1 "cell.sch" 50
F2 "S" I R 4850 11150 50 
F3 "V-" I R 4850 11050 50 
F4 "C" I R 4850 10950 50 
F5 "C+" I L 4350 10950 50 
$EndSheet
Text Label 5100 11150 0    50   ~ 0
S8
Text Label 5100 10950 0    50   ~ 0
C8
$Sheet
S 4350 10300 500  400 
U 5B50594B
F0 "cell9" 50
F1 "cell.sch" 50
F2 "S" I R 4850 10600 50 
F3 "V-" I R 4850 10500 50 
F4 "C" I R 4850 10400 50 
F5 "C+" I L 4350 10400 50 
$EndSheet
Text Label 5100 10600 0    50   ~ 0
S9
Text Label 5100 10400 0    50   ~ 0
C9
$Sheet
S 4350 9750 500  400 
U 5B506BB1
F0 "cell10" 50
F1 "cell.sch" 50
F2 "S" I R 4850 10050 50 
F3 "V-" I R 4850 9950 50 
F4 "C" I R 4850 9850 50 
F5 "C+" I L 4350 9850 50 
$EndSheet
Text Label 5100 10050 0    50   ~ 0
S10
Text Label 5100 9850 0    50   ~ 0
C10
$Sheet
S 4350 9200 500  400 
U 5B507536
F0 "cell11" 50
F1 "cell.sch" 50
F2 "S" I R 4850 9500 50 
F3 "V-" I R 4850 9400 50 
F4 "C" I R 4850 9300 50 
F5 "C+" I L 4350 9300 50 
$EndSheet
Text Label 5100 9500 0    50   ~ 0
S11
Text Label 5100 9300 0    50   ~ 0
C11
$Sheet
S 4350 8650 500  400 
U 5B50804F
F0 "cell12" 50
F1 "cell.sch" 50
F2 "S" I R 4850 8950 50 
F3 "V-" I R 4850 8850 50 
F4 "C" I R 4850 8750 50 
F5 "C+" I L 4350 8750 50 
$EndSheet
Text Label 5100 8950 0    50   ~ 0
S12
Text Label 5100 8750 0    50   ~ 0
C12
$Sheet
S 4350 8100 500  400 
U 5B508C6C
F0 "cell13" 50
F1 "cell.sch" 50
F2 "S" I R 4850 8400 50 
F3 "V-" I R 4850 8300 50 
F4 "C" I R 4850 8200 50 
F5 "C+" I L 4350 8200 50 
$EndSheet
Text Label 5100 8400 0    50   ~ 0
S13
Text Label 5100 8200 0    50   ~ 0
C13
$Sheet
S 4350 7550 500  400 
U 5B509A8B
F0 "cell14" 50
F1 "cell.sch" 50
F2 "S" I R 4850 7850 50 
F3 "V-" I R 4850 7750 50 
F4 "C" I R 4850 7650 50 
F5 "C+" I L 4350 7650 50 
$EndSheet
Text Label 5100 7850 0    50   ~ 0
S14
Text Label 5100 7650 0    50   ~ 0
C14
$Sheet
S 4350 7000 500  400 
U 5B50AABA
F0 "cell15" 50
F1 "cell.sch" 50
F2 "S" I R 4850 7300 50 
F3 "V-" I R 4850 7200 50 
F4 "C" I R 4850 7100 50 
F5 "C+" I L 4350 7100 50 
$EndSheet
$Sheet
S 4350 6450 500  400 
U 5B50EED8
F0 "cell16" 50
F1 "cell.sch" 50
F2 "S" I R 4850 6750 50 
F3 "V-" I R 4850 6650 50 
F4 "C" I R 4850 6550 50 
F5 "C+" I L 4350 6550 50 
$EndSheet
$Sheet
S 4350 5900 500  400 
U 5B510167
F0 "cell17" 50
F1 "cell.sch" 50
F2 "S" I R 4850 6200 50 
F3 "V-" I R 4850 6100 50 
F4 "C" I R 4850 6000 50 
F5 "C+" I L 4350 6000 50 
$EndSheet
$Sheet
S 4350 5350 500  400 
U 5B5115CC
F0 "cell18" 50
F1 "cell.sch" 50
F2 "S" I R 4850 5650 50 
F3 "V-" I R 4850 5550 50 
F4 "C" I R 4850 5450 50 
F5 "C+" I L 4350 5450 50 
$EndSheet
Wire Wire Line
	4350 5450 4100 5450
Wire Wire Line
	4100 5450 4100 4650
Wire Wire Line
	4100 4650 1750 4650
Wire Wire Line
	4300 6000 4000 6000
Wire Wire Line
	4000 6000 4000 4750
Wire Wire Line
	4000 4750 1500 4750
Wire Wire Line
	4350 6550 3900 6550
Wire Wire Line
	3900 6550 3900 4850
Wire Wire Line
	3900 4850 1500 4850
Wire Wire Line
	4350 7100 3800 7100
Wire Wire Line
	3800 7100 3800 4950
Wire Wire Line
	3800 4950 1500 4950
Wire Wire Line
	4350 7650 3700 7650
Wire Wire Line
	3700 7650 3700 5050
Wire Wire Line
	3700 5050 1500 5050
Wire Wire Line
	4350 8200 3600 8200
Wire Wire Line
	3600 8200 3600 5150
Wire Wire Line
	3600 5150 1500 5150
Wire Wire Line
	4350 8750 3500 8750
Wire Wire Line
	3500 8750 3500 5250
Wire Wire Line
	3500 5250 1500 5250
Wire Wire Line
	1500 5350 3400 5350
Wire Wire Line
	3400 5350 3400 9300
Wire Wire Line
	3400 9300 4350 9300
Wire Wire Line
	1500 5450 3300 5450
Wire Wire Line
	3300 5450 3300 9850
Wire Wire Line
	3300 9850 4350 9850
Wire Wire Line
	4350 10400 3200 10400
Wire Wire Line
	3200 10400 3200 5550
Wire Wire Line
	3200 5550 1500 5550
Wire Wire Line
	1500 5650 3100 5650
Wire Wire Line
	3100 5650 3100 10950
Wire Wire Line
	3100 10950 4350 10950
Wire Wire Line
	4350 11500 3000 11500
Wire Wire Line
	3000 11500 3000 5750
Wire Wire Line
	3000 5750 1500 5750
Wire Wire Line
	1500 5850 2900 5850
Wire Wire Line
	2900 5850 2900 12050
Wire Wire Line
	2900 12050 4350 12050
Wire Wire Line
	4350 12600 2800 12600
Wire Wire Line
	2800 12600 2800 5950
Wire Wire Line
	4350 14800 2400 14800
Wire Wire Line
	2500 14250 4350 14250
Wire Wire Line
	4350 13700 2600 13700
Wire Wire Line
	4100 4650 4100 4550
Connection ~ 4100 4650
Wire Wire Line
	4100 4550 4200 4550
Wire Wire Line
	1500 6450 1500 6600
Wire Wire Line
	1500 6350 2400 6350
Wire Wire Line
	2400 6350 2400 14800
Wire Wire Line
	2500 6250 1500 6250
Wire Wire Line
	2500 6250 2500 14250
Wire Wire Line
	2600 6150 1500 6150
Wire Wire Line
	2600 6150 2600 13700
Wire Wire Line
	4350 13150 2700 13150
Wire Wire Line
	2700 13150 2700 6050
Wire Wire Line
	2700 6050 1500 6050
Wire Wire Line
	1500 5950 2800 5950
Wire Wire Line
	5050 4350 5050 5550
Wire Wire Line
	5050 5550 4850 5550
Wire Wire Line
	5050 5550 5050 6100
Wire Wire Line
	5050 6100 4850 6100
Connection ~ 5050 5550
Wire Wire Line
	5050 6100 5050 6650
Wire Wire Line
	5050 6650 4850 6650
Connection ~ 5050 6100
Wire Wire Line
	5050 6650 5050 7200
Wire Wire Line
	5050 7200 4850 7200
Connection ~ 5050 6650
Wire Wire Line
	5050 7200 5050 7750
Wire Wire Line
	5050 7750 4850 7750
Connection ~ 5050 7200
Wire Wire Line
	5050 7750 5050 8300
Wire Wire Line
	5050 8300 4850 8300
Connection ~ 5050 7750
Wire Wire Line
	5050 8300 5050 8850
Wire Wire Line
	5050 8850 4850 8850
Connection ~ 5050 8300
Wire Wire Line
	5050 8850 5050 9400
Wire Wire Line
	5050 9400 4850 9400
Connection ~ 5050 8850
Wire Wire Line
	5050 9400 5050 9950
Wire Wire Line
	5050 9950 4850 9950
Connection ~ 5050 9400
Wire Wire Line
	5050 9950 5050 10500
Wire Wire Line
	5050 10500 4850 10500
Connection ~ 5050 9950
Wire Wire Line
	5050 10500 5050 11050
Wire Wire Line
	5050 11050 4850 11050
Connection ~ 5050 10500
Wire Wire Line
	5050 11050 5050 11600
Wire Wire Line
	5050 11600 4850 11600
Connection ~ 5050 11050
Wire Wire Line
	5050 11600 5050 12150
Wire Wire Line
	5050 12150 4850 12150
Connection ~ 5050 11600
Wire Wire Line
	5050 12150 5050 12700
Wire Wire Line
	5050 12700 4850 12700
Connection ~ 5050 12150
Wire Wire Line
	5050 12700 5050 13250
Wire Wire Line
	5050 13250 4850 13250
Connection ~ 5050 12700
Wire Wire Line
	5050 13250 5050 13800
Wire Wire Line
	5050 13800 4850 13800
Connection ~ 5050 13250
Wire Wire Line
	5050 13800 5050 14350
Wire Wire Line
	5050 14350 4850 14350
Connection ~ 5050 13800
Wire Wire Line
	5050 14350 5050 14900
Wire Wire Line
	5050 14900 4850 14900
Connection ~ 5050 14350
$Comp
L power:GND #PWR?
U 1 1 5B5FFA6A
P 5050 15200
F 0 "#PWR?" H 5050 14950 50  0001 C CNN
F 1 "GND" H 5055 15027 50  0000 C CNN
F 2 "" H 5050 15200 50  0001 C CNN
F 3 "" H 5050 15200 50  0001 C CNN
	1    5050 15200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 14900 5050 15200
Connection ~ 5050 14900
Wire Wire Line
	5800 5450 5800 4650
Wire Wire Line
	5800 4650 7950 4650
Wire Wire Line
	4850 5450 5800 5450
Wire Wire Line
	5900 4750 5900 5650
Wire Wire Line
	4850 5650 5900 5650
Wire Wire Line
	5900 4750 7950 4750
Wire Wire Line
	4700 4550 7950 4550
Wire Wire Line
	7950 4850 6000 4850
Wire Wire Line
	6000 4850 6000 6000
Wire Wire Line
	4850 6000 6000 6000
Wire Wire Line
	7950 4950 6100 4950
Wire Wire Line
	6100 4950 6100 6200
Wire Wire Line
	4850 6200 6100 6200
Wire Wire Line
	7950 5050 6200 5050
Wire Wire Line
	6200 5050 6200 6550
Wire Wire Line
	4850 6550 6200 6550
Wire Wire Line
	6300 6750 6300 5150
Wire Wire Line
	6300 5150 7950 5150
Wire Wire Line
	4850 6750 6300 6750
Wire Wire Line
	7950 5250 6400 5250
Wire Wire Line
	6400 5250 6400 7100
Wire Wire Line
	4850 7100 6400 7100
Wire Wire Line
	7950 5350 6500 5350
Wire Wire Line
	6500 5350 6500 7300
Wire Wire Line
	4850 7300 6500 7300
Wire Wire Line
	7950 5450 6600 5450
Wire Wire Line
	6600 5450 6600 7650
Wire Wire Line
	4850 7650 6600 7650
Wire Wire Line
	6700 7850 6700 5550
Wire Wire Line
	6700 5550 7950 5550
Wire Wire Line
	4850 7850 6700 7850
Wire Wire Line
	7950 5650 6800 5650
Wire Wire Line
	6800 5650 6800 8200
Wire Wire Line
	4850 8200 6800 8200
Wire Wire Line
	6900 8400 6900 5750
Wire Wire Line
	6900 5750 7950 5750
Wire Wire Line
	4850 8400 6900 8400
Wire Wire Line
	7950 5850 7000 5850
Wire Wire Line
	7000 5850 7000 8750
Wire Wire Line
	4850 8750 7000 8750
Wire Wire Line
	7100 8950 7100 5950
Wire Wire Line
	7100 5950 7950 5950
Wire Wire Line
	4850 8950 7100 8950
Wire Wire Line
	7200 9300 7200 6050
Wire Wire Line
	7200 6050 7950 6050
Wire Wire Line
	4850 9300 7200 9300
Wire Wire Line
	8400 6500 8400 9500
Wire Wire Line
	4850 9500 8400 9500
Wire Wire Line
	8500 6500 8500 9850
Wire Wire Line
	4850 9850 8500 9850
Wire Wire Line
	8600 10050 8600 6500
Wire Wire Line
	4850 10050 8600 10050
Wire Wire Line
	8700 6500 8700 10400
Wire Wire Line
	4850 10400 8700 10400
Wire Wire Line
	8800 10600 8800 6500
Wire Wire Line
	4850 10600 8800 10600
Wire Wire Line
	8900 6500 8900 10950
Wire Wire Line
	4850 10950 8900 10950
Wire Wire Line
	9000 11150 9000 6500
Wire Wire Line
	4850 11150 9000 11150
Wire Wire Line
	9100 6500 9100 11500
Wire Wire Line
	4850 11500 9100 11500
Wire Wire Line
	9200 11700 9200 6500
Wire Wire Line
	4850 11700 9200 11700
Wire Wire Line
	9300 6500 9300 12050
Wire Wire Line
	4850 12050 9300 12050
Wire Wire Line
	9400 12250 9400 6500
Wire Wire Line
	4850 12250 9400 12250
Wire Wire Line
	9500 6500 9500 12600
Wire Wire Line
	4850 12600 9500 12600
Wire Wire Line
	9600 12800 9600 6500
Wire Wire Line
	4850 12800 9600 12800
Wire Wire Line
	9700 6500 9700 13150
Wire Wire Line
	4850 13150 9700 13150
Wire Wire Line
	9800 13350 9800 6500
Wire Wire Line
	4850 13350 9800 13350
Wire Wire Line
	9900 6500 9900 13700
Wire Wire Line
	4850 13700 9900 13700
Wire Wire Line
	10600 13900 10600 6050
Wire Wire Line
	10600 6050 10350 6050
Wire Wire Line
	4850 13900 10600 13900
Wire Wire Line
	10350 5950 10700 5950
Wire Wire Line
	10700 5950 10700 14250
Wire Wire Line
	4850 14250 10700 14250
Wire Wire Line
	10800 14450 10800 5850
Wire Wire Line
	10800 5850 10350 5850
Wire Wire Line
	4850 14450 10800 14450
Wire Wire Line
	10350 5750 10900 5750
Wire Wire Line
	10900 5750 10900 14800
Wire Wire Line
	4850 14800 10900 14800
Wire Wire Line
	10350 5650 11000 5650
Wire Wire Line
	11000 5650 11000 15000
Wire Wire Line
	4850 15000 11000 15000
Wire Wire Line
	8800 4100 8800 4000
Wire Wire Line
	8800 4000 8900 4000
Wire Wire Line
	8900 4000 8900 3700
$Comp
L Device:R R?
U 1 1 5B78DC84
P 6100 2850
F 0 "R?" V 5893 2850 50  0000 C CNN
F 1 "100" V 5984 2850 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.99x1.00mm_HandSolder" V 6030 2850 50  0001 C CNN
F 3 "~" H 6100 2850 50  0001 C CNN
	1    6100 2850
	0    1    1    0   
$EndComp
$Comp
L Device:C C?
U 1 1 5B78DD6A
P 6800 3000
F 0 "C?" H 6685 2954 50  0000 R CNN
F 1 "10nF" H 6685 3045 50  0000 R CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad0.99x1.00mm_HandSolder" H 6838 2850 50  0001 C CNN
F 3 "~" H 6800 3000 50  0001 C CNN
	1    6800 3000
	-1   0    0    1   
$EndComp
Wire Wire Line
	6250 2850 6800 2850
Connection ~ 6800 2850
Wire Wire Line
	4200 4550 4200 2850
Wire Wire Line
	4200 2850 5950 2850
Connection ~ 4200 4550
Wire Wire Line
	4200 4550 4400 4550
$Comp
L power:GND #PWR?
U 1 1 5B7A9A9A
P 6800 3250
F 0 "#PWR?" H 6800 3000 50  0001 C CNN
F 1 "GND" H 6805 3077 50  0000 C CNN
F 2 "" H 6800 3250 50  0001 C CNN
F 3 "" H 6800 3250 50  0001 C CNN
	1    6800 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6800 3150 6800 3250
$Comp
L Device:C C?
U 1 1 5B7B35E7
P 11100 3850
F 0 "C?" H 10985 3804 50  0000 R CNN
F 1 "1uF" H 10985 3895 50  0000 R CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad0.99x1.00mm_HandSolder" H 11138 3700 50  0001 C CNN
F 3 "~" H 11100 3850 50  0001 C CNN
	1    11100 3850
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5B7B35ED
P 11100 4150
F 0 "#PWR?" H 11100 3900 50  0001 C CNN
F 1 "GND" H 11105 3977 50  0000 C CNN
F 2 "" H 11100 4150 50  0001 C CNN
F 3 "" H 11100 4150 50  0001 C CNN
	1    11100 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	11100 4000 11100 4150
Wire Wire Line
	10350 4550 10700 4550
Wire Wire Line
	10700 3250 11100 3250
Wire Wire Line
	11100 3250 11100 3700
$Comp
L Device:Q_NPN_BCE Q?
U 1 1 5B7C76D5
P 10600 3050
F 0 "Q?" H 10791 3096 50  0000 L CNN
F 1 "DZT5551" H 10791 3005 50  0000 L CNN
F 2 "" H 10800 3150 50  0001 C CNN
F 3 "~" H 10600 3050 50  0001 C CNN
	1    10600 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6800 2850 10700 2850
Wire Wire Line
	10400 3050 9900 3050
Wire Wire Line
	9900 3050 9900 4100
$Comp
L power:GND #PWR?
U 1 1 5B7E6452
P 8900 2350
F 0 "#PWR?" H 8900 2100 50  0001 C CNN
F 1 "GND" H 8905 2177 50  0000 C CNN
F 2 "" H 8900 2350 50  0001 C CNN
F 3 "" H 8900 2350 50  0001 C CNN
	1    8900 2350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5B7FB6B8
P 8900 2100
F 0 "C?" H 8785 2054 50  0000 R CNN
F 1 "1uF" H 8785 2145 50  0000 R CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad0.99x1.00mm_HandSolder" H 8938 1950 50  0001 C CNN
F 3 "~" H 8900 2100 50  0001 C CNN
	1    8900 2100
	-1   0    0    1   
$EndComp
Wire Wire Line
	8900 2350 8900 2250
Connection ~ 10700 3250
Wire Wire Line
	10700 3250 10700 4550
$Comp
L power:GND #PWR?
U 1 1 5B842962
P 9450 2350
F 0 "#PWR?" H 9450 2100 50  0001 C CNN
F 1 "GND" H 9455 2177 50  0000 C CNN
F 2 "" H 9450 2350 50  0001 C CNN
F 3 "" H 9450 2350 50  0001 C CNN
	1    9450 2350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5B842968
P 9450 2100
F 0 "C?" H 9335 2054 50  0000 R CNN
F 1 "1uF" H 9335 2145 50  0000 R CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad0.99x1.00mm_HandSolder" H 9488 1950 50  0001 C CNN
F 3 "~" H 9450 2100 50  0001 C CNN
	1    9450 2100
	-1   0    0    1   
$EndComp
Wire Wire Line
	9450 2350 9450 2250
Wire Wire Line
	9050 1950 8900 1950
Wire Wire Line
	9450 1950 9600 1950
Text Label 9050 1950 0    50   ~ 0
VREF1
Text Label 9600 1950 0    50   ~ 0
VREF2
Text Label 9700 3950 1    50   ~ 0
VREF1
Text Label 9800 3950 1    50   ~ 0
VREF2
$Comp
L Device:R R?
U 1 1 5B8B85BD
P 15450 2200
F 0 "R?" V 15243 2200 50  0000 C CNN
F 1 "100" V 15334 2200 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.99x1.00mm_HandSolder" V 15380 2200 50  0001 C CNN
F 3 "~" H 15450 2200 50  0001 C CNN
	1    15450 2200
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 5B8D9E26
P 15550 3550
F 0 "R?" V 15343 3550 50  0000 C CNN
F 1 "100" V 15434 3550 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.99x1.00mm_HandSolder" V 15480 3550 50  0001 C CNN
F 3 "~" H 15550 3550 50  0001 C CNN
	1    15550 3550
	-1   0    0    1   
$EndComp
$Comp
L Device:Transformer_1P_1S T?
U 1 1 5B94C3D3
P 16050 2200
F 0 "T?" H 16050 1775 50  0000 C CNN
F 1 "TA" H 16050 1866 50  0000 C CNN
F 2 "" H 16050 2200 50  0001 C CNN
F 3 "~" H 16050 2200 50  0001 C CNN
	1    16050 2200
	-1   0    0    1   
$EndComp
$Comp
L Device:Transformer_1P_1S T?
U 1 1 5B94C4EE
P 16100 3550
F 0 "T?" H 16100 3125 50  0000 C CNN
F 1 "TB" H 16100 3216 50  0000 C CNN
F 2 "" H 16100 3550 50  0001 C CNN
F 3 "~" H 16100 3550 50  0001 C CNN
	1    16100 3550
	-1   0    0    1   
$EndComp
Wire Wire Line
	15650 2000 15450 2000
Wire Wire Line
	15450 2000 15450 2050
Wire Wire Line
	15650 2400 15450 2400
Wire Wire Line
	15450 2400 15450 2350
Connection ~ 8900 4000
Wire Wire Line
	2300 6600 2300 15550
Wire Wire Line
	2300 15550 11100 15550
Wire Wire Line
	11100 15550 11100 5550
Wire Wire Line
	10350 5550 11100 5550
Wire Wire Line
	15550 3400 15550 3350
Wire Wire Line
	15550 3350 15700 3350
Wire Wire Line
	15550 3700 15550 3750
Wire Wire Line
	15550 3750 15700 3750
Text Label 5100 7300 0    50   ~ 0
S15
Text Label 5100 7100 0    50   ~ 0
C15
Text Label 5100 6750 0    50   ~ 0
S16
Text Label 5100 6550 0    50   ~ 0
C16
Text Label 5100 6200 0    50   ~ 0
S17
Text Label 5100 6000 0    50   ~ 0
C17
Text Label 5100 5650 0    50   ~ 0
S18
Text Label 5100 5450 0    50   ~ 0
C18
Text Label 2200 6600 0    50   ~ 0
C0
Text Label 8400 3950 1    50   ~ 0
IPB
Wire Wire Line
	8400 4100 8400 3950
Text Label 8500 3950 1    50   ~ 0
IMB
Wire Wire Line
	8500 3950 8500 4100
Text Label 8600 3950 1    50   ~ 0
IPA
Wire Wire Line
	8600 4100 8600 3950
Text Label 8700 3950 1    50   ~ 0
IMA
Wire Wire Line
	8700 3950 8700 4100
Text Label 15150 3750 2    50   ~ 0
IPB
Text Label 15150 3350 2    50   ~ 0
IMB
Text Label 15100 2400 2    50   ~ 0
IPA
Text Label 15100 2000 2    50   ~ 0
IMA
Wire Wire Line
	15100 2000 15450 2000
Connection ~ 15450 2000
Wire Wire Line
	15450 2400 15100 2400
Connection ~ 15450 2400
Wire Wire Line
	15150 3350 15550 3350
Connection ~ 15550 3350
Wire Wire Line
	15150 3750 15550 3750
Connection ~ 15550 3750
Text Label 10650 4550 2    50   ~ 0
VREG
Text Label 9300 3950 1    50   ~ 0
VREG
Wire Wire Line
	9300 3950 9300 4100
Wire Wire Line
	9700 3950 9700 4100
Wire Wire Line
	9800 3950 9800 4100
$Comp
L Device:R R?
U 1 1 5BA809B4
P 9300 3250
F 0 "R?" V 9093 3250 50  0000 C CNN
F 1 "R" V 9184 3250 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.99x1.00mm_HandSolder" V 9230 3250 50  0001 C CNN
F 3 "~" H 9300 3250 50  0001 C CNN
	1    9300 3250
	0    1    1    0   
$EndComp
Wire Wire Line
	9100 4100 9100 3400
Wire Wire Line
	9100 3400 9450 3400
Wire Wire Line
	9450 3400 9450 3250
$Comp
L Device:R R?
U 1 1 5BA9A399
P 8750 3250
F 0 "R?" V 8543 3250 50  0000 C CNN
F 1 "R" V 8634 3250 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.99x1.00mm_HandSolder" V 8680 3250 50  0001 C CNN
F 3 "~" H 8750 3250 50  0001 C CNN
	1    8750 3250
	0    1    1    0   
$EndComp
Wire Wire Line
	8900 3250 9000 3250
Wire Wire Line
	9000 4100 9000 3250
Connection ~ 9000 3250
Wire Wire Line
	9000 3250 9150 3250
Wire Wire Line
	8800 3700 8800 3450
Wire Wire Line
	8800 3450 8550 3450
Wire Wire Line
	8550 3450 8550 3250
Wire Wire Line
	8550 3250 8600 3250
Connection ~ 8800 3700
Text HLabel 16600 2000 2    50   Input ~ 0
MA
Wire Wire Line
	16450 2000 16600 2000
Text HLabel 16600 2400 2    50   Input ~ 0
PA
Wire Wire Line
	16450 2400 16600 2400
Text HLabel 16650 3350 2    50   Input ~ 0
MB
Wire Wire Line
	16500 3350 16650 3350
Text HLabel 16650 3750 2    50   Input ~ 0
PB
Wire Wire Line
	16500 3750 16650 3750
Text HLabel 1500 4300 0    50   Input ~ 0
BAT+
Wire Wire Line
	1500 4300 1750 4300
Wire Wire Line
	1750 4300 1750 4650
Connection ~ 1750 4650
Wire Wire Line
	1750 4650 1500 4650
$EndSCHEMATC
