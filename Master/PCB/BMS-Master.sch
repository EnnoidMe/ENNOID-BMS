EESchema Schematic File Version 4
LIBS:BMS-Master-cache
EELAYER 29 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 1 8
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
L BMS-Master-rescue:USB_B_Micro-Connector_Specialized J12
U 1 1 5A966632
P 11850 6750
F 0 "J12" H 11906 7217 50  0000 C CNN
F 1 "USB" H 11906 7126 50  0000 C CNN
F 2 "Connectors_USB:USB_Micro-B_Molex_47346-0001" H 12000 6700 50  0001 C CNN
F 3 "~" H 12000 6700 50  0001 C CNN
	1    11850 6750
	-1   0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:GND-power #PWR0101
U 1 1 5A969833
P 11900 7200
F 0 "#PWR0101" H 11900 6950 50  0001 C CNN
F 1 "GND" H 11905 7027 50  0000 C CNN
F 2 "" H 11900 7200 50  0001 C CNN
F 3 "" H 11900 7200 50  0001 C CNN
	1    11900 7200
	1    0    0    -1  
$EndComp
Wire Wire Line
	11900 7200 11850 7200
Wire Wire Line
	11850 7200 11850 7150
Wire Wire Line
	11900 7200 11950 7200
Wire Wire Line
	11950 7200 11950 7150
Connection ~ 11900 7200
NoConn ~ 11550 6950
Wire Wire Line
	8700 7950 9250 7950
Wire Wire Line
	9250 8050 8700 8050
$Comp
L BMS-Master-rescue:LED-Device D2
U 1 1 5ABAB322
P 6800 10700
F 0 "D2" V 6838 10583 50  0000 R CNN
F 1 "STATUS" V 6747 10583 50  0000 R CNN
F 2 "LEDs:LED_0603_HandSoldering" H 6800 10700 50  0001 C CNN
F 3 "~" H 6800 10700 50  0001 C CNN
	1    6800 10700
	0    -1   -1   0   
$EndComp
$Comp
L BMS-Master-rescue:R-Device R3
U 1 1 5ABAB411
P 6800 10300
F 0 "R3" V 7000 10300 50  0000 C CNN
F 1 "470" V 6900 10300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 6730 10300 50  0001 C CNN
F 3 "~" H 6800 10300 50  0001 C CNN
	1    6800 10300
	-1   0    0    1   
$EndComp
$Comp
L BMS-Master-rescue:R-Device R1
U 1 1 5ABB20CA
P 3800 10250
F 0 "R1" V 4000 10250 50  0000 C CNN
F 1 "470" V 3900 10250 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3730 10250 50  0001 C CNN
F 3 "~" H 3800 10250 50  0001 C CNN
	1    3800 10250
	-1   0    0    1   
$EndComp
$Comp
L BMS-Master-rescue:LED-Device D1
U 1 1 5ABB20FE
P 3800 10650
F 0 "D1" V 3838 10533 50  0000 R CNN
F 1 "POWER" V 3747 10533 50  0000 R CNN
F 2 "LEDs:LED_0603_HandSoldering" H 3800 10650 50  0001 C CNN
F 3 "~" H 3800 10650 50  0001 C CNN
	1    3800 10650
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3800 10500 3800 10400
Wire Wire Line
	6800 10550 6800 10450
$Comp
L BMS-Master-rescue:GND-power #PWR0102
U 1 1 5ABB698D
P 3800 10900
F 0 "#PWR0102" H 3800 10650 50  0001 C CNN
F 1 "GND" H 3805 10727 50  0000 C CNN
F 2 "" H 3800 10900 50  0001 C CNN
F 3 "" H 3800 10900 50  0001 C CNN
	1    3800 10900
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:GND-power #PWR0103
U 1 1 5ABB69D7
P 6800 10950
F 0 "#PWR0103" H 6800 10700 50  0001 C CNN
F 1 "GND" H 6805 10777 50  0000 C CNN
F 2 "" H 6800 10950 50  0001 C CNN
F 3 "" H 6800 10950 50  0001 C CNN
	1    6800 10950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6800 10850 6800 10950
Wire Wire Line
	3800 10800 3800 10900
Wire Wire Line
	6800 10150 6800 9950
$Comp
L BMS-Master-rescue:+3.3V-power #PWR0104
U 1 1 5ABBDFF3
P 3800 10000
F 0 "#PWR0104" H 3800 9850 50  0001 C CNN
F 1 "+3.3V" H 3815 10173 50  0000 C CNN
F 2 "" H 3800 10000 50  0001 C CNN
F 3 "" H 3800 10000 50  0001 C CNN
	1    3800 10000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 10000 3800 10100
$Comp
L BMS-Master-rescue:R-Device R4
U 1 1 5ABC0C8F
P 9300 10600
F 0 "R4" V 9500 10600 50  0000 C CNN
F 1 "1.5k" V 9400 10600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 9230 10600 50  0001 C CNN
F 3 "~" H 9300 10600 50  0001 C CNN
	1    9300 10600
	-1   0    0    1   
$EndComp
$Comp
L BMS-Master-rescue:GND-power #PWR0105
U 1 1 5ABC0CBB
P 9300 10900
F 0 "#PWR0105" H 9300 10650 50  0001 C CNN
F 1 "GND" H 9305 10727 50  0000 C CNN
F 2 "" H 9300 10900 50  0001 C CNN
F 3 "" H 9300 10900 50  0001 C CNN
	1    9300 10900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9300 10750 9300 10900
Wire Wire Line
	8700 9950 9300 9950
Wire Wire Line
	9300 9950 9300 10450
$Comp
L BMS-Master-rescue:Buzzer-Device BZ1
U 1 1 5ABC5C93
P 10450 10200
F 0 "BZ1" H 10603 10229 50  0000 L CNN
F 1 "Buzzer" H 10603 10138 50  0000 L CNN
F 2 "Buzzers_Beepers:PUIAudio_SMT_0825_S_4_R" V 10425 10300 50  0001 C CNN
F 3 "~" V 10425 10300 50  0001 C CNN
	1    10450 10200
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:+3.3V-power #PWR0106
U 1 1 5ABC5D22
P 10350 10000
F 0 "#PWR0106" H 10350 9850 50  0001 C CNN
F 1 "+3.3V" H 10365 10173 50  0000 C CNN
F 2 "" H 10350 10000 50  0001 C CNN
F 3 "" H 10350 10000 50  0001 C CNN
	1    10350 10000
	1    0    0    -1  
$EndComp
Wire Wire Line
	10350 10000 10350 10100
$Comp
L BMS-Master-rescue:GND-power #PWR0107
U 1 1 5ABCABB8
P 10350 10900
F 0 "#PWR0107" H 10350 10650 50  0001 C CNN
F 1 "GND" H 10355 10727 50  0000 C CNN
F 2 "" H 10350 10900 50  0001 C CNN
F 3 "" H 10350 10900 50  0001 C CNN
	1    10350 10900
	1    0    0    -1  
$EndComp
Wire Wire Line
	10350 10300 10350 10400
Wire Wire Line
	10350 10800 10350 10900
$Comp
L BMS-Master-rescue:Conn_01x05-Connector_Generic J6
U 1 1 5ABE6776
P 3500 4250
F 0 "J6" V 3700 4050 50  0000 C CNN
F 1 "Power Button" V 3600 4250 50  0000 C CNN
F 2 "Connectors_Molex:Molex_PicoBlade_53398-0571_05x1.25mm_Straight" H 3500 4250 50  0001 C CNN
F 3 "~" H 3500 4250 50  0001 C CNN
	1    3500 4250
	-1   0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:+3.3V-power #PWR0108
U 1 1 5ABE6960
P 3950 4000
F 0 "#PWR0108" H 3950 3850 50  0001 C CNN
F 1 "+3.3V" H 3965 4173 50  0000 C CNN
F 2 "" H 3950 4000 50  0001 C CNN
F 3 "" H 3950 4000 50  0001 C CNN
	1    3950 4000
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:R-Device R2
U 1 1 5ABEC8F7
P 4050 4350
F 0 "R2" V 4250 4350 50  0000 C CNN
F 1 "470" V 4150 4350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3980 4350 50  0001 C CNN
F 3 "~" H 4050 4350 50  0001 C CNN
	1    4050 4350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3700 4350 3900 4350
$Comp
L BMS-Master-rescue:GND-power #PWR0109
U 1 1 5ABF492B
P 3800 4550
F 0 "#PWR0109" H 3800 4300 50  0001 C CNN
F 1 "GND" H 3805 4377 50  0000 C CNN
F 2 "" H 3800 4550 50  0001 C CNN
F 3 "" H 3800 4550 50  0001 C CNN
	1    3800 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 4450 3800 4450
Wire Wire Line
	3800 4450 3800 4550
Wire Wire Line
	3800 4450 3800 4050
Wire Wire Line
	3800 4050 3700 4050
Connection ~ 3800 4450
Wire Wire Line
	3950 4000 3950 4250
Wire Wire Line
	3950 4250 3700 4250
$Comp
L BMS-Master-rescue:Conn_01x04-Connector_Generic J7
U 1 1 5AB6317C
P 3800 2100
F 0 "J7" H 3880 2092 50  0000 L CNN
F 1 "Current" H 3880 2001 50  0000 L CNN
F 2 "Connectors_Molex:Molex_KK-6410-04_04x2.54mm_Straight" H 3800 2100 50  0001 C CNN
F 3 "~" H 3800 2100 50  0001 C CNN
	1    3800 2100
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4000 2300 4100 2300
Wire Wire Line
	4100 2300 4100 2350
Wire Wire Line
	9650 10600 10050 10600
$Sheet
S 7150 1600 1550 1400
U 5AC7323C
F0 "switches" 50
F1 "switches.sch" 50
F2 "DischargeEnable" I L 7150 2400 50 
F3 "ChargeEnable" I L 7150 2200 50 
F4 "PrechargeEnable" I L 7150 2500 50 
F5 "Discharge" I R 8700 1800 50 
F6 "Precharge" I R 8700 1950 50 
F7 "Charge" I R 8700 2100 50 
F8 "Enable" I L 7150 2300 50 
F9 "Safety" I L 7150 2600 50 
F10 "Vpack" I L 7150 1850 50 
F11 "Vbat" I L 7150 1750 50 
F12 "24V" I L 7150 2850 50 
F13 "VoutSense" I R 8700 2900 50 
F14 "NTC_Switch" I R 8700 2800 50 
$EndSheet
$Comp
L BMS-Master-rescue:Conn_01x02-Connector_Generic J3
U 1 1 5AB7AB5E
P 3450 3600
F 0 "J3" H 3529 3592 50  0000 L CNN
F 1 "ExtPower" H 3529 3501 50  0000 L CNN
F 2 "Connectors_Molex:Molex_KK-6410-02_02x2.54mm_Straight" H 3450 3600 50  0001 C CNN
F 3 "~" H 3450 3600 50  0001 C CNN
	1    3450 3600
	-1   0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:GND-power #PWR0112
U 1 1 5AB80FEA
P 3700 3750
F 0 "#PWR0112" H 3700 3500 50  0001 C CNN
F 1 "GND" H 3705 3577 50  0000 C CNN
F 2 "" H 3700 3750 50  0001 C CNN
F 3 "" H 3700 3750 50  0001 C CNN
	1    3700 3750
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3700 3750 3700 3700
Wire Wire Line
	3700 3700 3650 3700
$Comp
L BMS-Master-rescue:Conn_01x02-Connector_Generic J8
U 1 1 5ABBEF45
P 10300 1600
F 0 "J8" H 10380 1592 50  0000 L CNN
F 1 "Discharge" H 10380 1501 50  0000 L CNN
F 2 "Connectors_Molex:Molex_KK-6410-02_02x2.54mm_Straight" H 10300 1600 50  0001 C CNN
F 3 "~" H 10300 1600 50  0001 C CNN
	1    10300 1600
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:Conn_01x02-Connector_Generic J9
U 1 1 5ABBF131
P 10300 1950
F 0 "J9" H 10379 1942 50  0000 L CNN
F 1 "Precharge" H 10379 1851 50  0000 L CNN
F 2 "Connectors_Molex:Molex_KK-6410-02_02x2.54mm_Straight" H 10300 1950 50  0001 C CNN
F 3 "~" H 10300 1950 50  0001 C CNN
	1    10300 1950
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:Conn_01x02-Connector_Generic J10
U 1 1 5ABBF293
P 10300 2300
F 0 "J10" H 10379 2292 50  0000 L CNN
F 1 "Charge" H 10379 2201 50  0000 L CNN
F 2 "Connectors_Molex:Molex_KK-6410-02_02x2.54mm_Straight" H 10300 2300 50  0001 C CNN
F 3 "~" H 10300 2300 50  0001 C CNN
	1    10300 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9800 2500 9800 2400
Wire Wire Line
	9800 2400 10100 2400
Wire Wire Line
	9800 2050 10100 2050
Connection ~ 9800 2400
Wire Wire Line
	9800 1700 10100 1700
Wire Wire Line
	8700 1800 9500 1800
Wire Wire Line
	9500 1800 9500 1600
Wire Wire Line
	9500 1600 10100 1600
Wire Wire Line
	8700 2100 9500 2100
Wire Wire Line
	9500 2300 10100 2300
$Sheet
S 9250 6450 1600 750 
U 5C246CA0
F0 "USB" 50
F1 "USB.sch" 50
F2 "RXD" I L 9250 6900 50 
F3 "TXD" I L 9250 6800 50 
F4 "D-" I R 10850 6850 50 
F5 "D+" I R 10850 6750 50 
F6 "Vusb" I R 10850 6650 50 
F7 "EnableRequest" I L 9250 6700 50 
$EndSheet
$Sheet
S 9250 7650 1600 700 
U 5A91E16C
F0 "CAN" 50
F1 "CAN.sch" 50
F2 "CAN_RX" O L 9250 7950 50 
F3 "CAN_TX" I L 9250 8050 50 
F4 "CANH" I R 10850 7950 50 
F5 "CANL" I R 10850 8050 50 
F6 "5V_CAN" I R 10850 7850 50 
F7 "CAN_GND" I R 10850 8150 50 
F8 "EnableRequest" I L 9250 7800 50 
F9 "Enable_CAN" I R 10850 7750 50 
$EndSheet
$Comp
L BMS-Master-rescue:Conn_01x05-Connector_Generic J11
U 1 1 5C2681C9
P 11600 7950
F 0 "J11" H 11680 7992 50  0000 L CNN
F 1 "CAN" H 11680 7901 50  0000 L CNN
F 2 "Connectors_Molex:Molex_PicoBlade_53261-0571_05x1.25mm_Angled" H 11600 7950 50  0001 C CNN
F 3 "" H 11600 7950 50  0001 C CNN
	1    11600 7950
	1    0    0    -1  
$EndComp
Wire Wire Line
	10850 7750 11400 7750
Wire Wire Line
	11400 7850 10850 7850
Wire Wire Line
	10850 7950 11400 7950
Wire Wire Line
	11400 8050 10850 8050
Wire Wire Line
	10850 8150 11400 8150
Wire Wire Line
	1950 1000 1950 1100
Wire Wire Line
	8700 6800 9250 6800
Wire Wire Line
	8700 6900 9250 6900
Wire Wire Line
	10850 6750 11550 6750
Wire Wire Line
	10850 6850 11550 6850
Wire Wire Line
	11550 6550 11200 6550
Wire Wire Line
	11200 6550 11200 6650
Wire Wire Line
	11200 6650 10850 6650
$Comp
L Transistor_FET:BSS138 Q1
U 1 1 5C30885C
P 10250 10600
F 0 "Q1" H 10456 10646 50  0000 L CNN
F 1 "BSS138" H 10456 10555 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 10450 10525 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/BS/BSS138.pdf" H 10250 10600 50  0001 L CNN
	1    10250 10600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 10600 9650 9950
Wire Wire Line
	9650 9950 9300 9950
Connection ~ 9300 9950
$Sheet
S 4350 800  1750 1700
U 5C519BA0
F0 "PowerMonitor" 50
F1 "PowerMonitor.sch" 50
F2 "I+" I L 4350 2100 50 
F3 "SCL" I R 6100 2050 50 
F4 "SDA" I R 6100 2150 50 
F5 "BAT+" I L 4350 900 50 
F6 "Charge+" I R 6100 950 50 
F7 "Discharge+" I R 6100 1250 50 
F8 "Vbat" I R 6100 1750 50 
F9 "Vpack" I R 6100 1850 50 
F10 "ChargeDetect" B R 6100 2350 50 
F11 "24V" I R 6100 2450 50 
F12 "I-" I L 4350 2200 50 
$EndSheet
$Comp
L power:GNDA #PWR0115
U 1 1 5C8C87E3
P 1950 1100
F 0 "#PWR0115" H 1950 850 50  0001 C CNN
F 1 "GNDA" H 1955 927 50  0000 C CNN
F 2 "" H 1950 1100 50  0001 C CNN
F 3 "" H 1950 1100 50  0001 C CNN
	1    1950 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 3600 4200 3600
$Comp
L power:GNDA #PWR0116
U 1 1 5C377052
P 13600 1700
F 0 "#PWR0116" H 13600 1450 50  0001 C CNN
F 1 "GNDA" H 13605 1527 50  0000 C CNN
F 2 "" H 13600 1700 50  0001 C CNN
F 3 "" H 13600 1700 50  0001 C CNN
	1    13600 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 3700 6950 3700
Wire Wire Line
	6950 3700 6950 2600
Wire Wire Line
	6950 2600 7150 2600
Wire Wire Line
	7150 2500 6900 2500
Wire Wire Line
	6900 2500 6900 3800
Wire Wire Line
	6900 3800 7150 3800
Wire Wire Line
	6850 3900 6850 2400
Wire Wire Line
	6850 2400 7150 2400
Wire Wire Line
	6850 3900 7150 3900
Wire Wire Line
	7150 4000 6800 4000
Wire Wire Line
	6800 4000 6800 2300
Wire Wire Line
	6800 2300 7150 2300
Wire Wire Line
	6750 4100 6750 2200
Wire Wire Line
	6750 2200 7150 2200
Wire Wire Line
	6750 4100 7150 4100
Wire Wire Line
	6100 2050 6550 2050
Wire Wire Line
	6550 2050 6550 4300
Wire Wire Line
	6550 4300 7150 4300
Wire Wire Line
	6100 1750 7150 1750
Wire Wire Line
	6100 1850 7150 1850
Wire Wire Line
	14200 950  6100 950 
Wire Wire Line
	6100 1250 14200 1250
Wire Wire Line
	1550 1000 1950 1000
Wire Wire Line
	6500 2150 6100 2150
Wire Wire Line
	6500 2150 6500 4400
Wire Wire Line
	6100 2350 6400 2350
Wire Wire Line
	6400 2350 6400 3800
Wire Wire Line
	6400 4550 7150 4550
Wire Wire Line
	6100 3800 6400 3800
Connection ~ 6400 3800
Wire Wire Line
	6400 3800 6400 4550
Wire Wire Line
	6300 4700 7150 4700
Wire Wire Line
	7150 4800 6250 4800
Wire Wire Line
	6650 3300 6650 2850
Wire Wire Line
	6650 2850 7150 2850
Wire Wire Line
	6150 4950 7150 4950
Wire Wire Line
	6100 3500 9150 3500
Wire Wire Line
	9150 3500 9150 6700
Wire Wire Line
	9150 6700 9250 6700
Wire Wire Line
	9150 6700 9150 7800
Wire Wire Line
	9150 7800 9250 7800
Connection ~ 9150 6700
Wire Wire Line
	8700 4100 8800 4100
Wire Wire Line
	8800 4100 8800 2900
Wire Wire Line
	8800 2900 8700 2900
Wire Wire Line
	8700 2800 8850 2800
Wire Wire Line
	8850 2800 8850 4200
Wire Wire Line
	8850 4200 8700 4200
$Sheet
S 4350 8950 1750 850 
U 5C8805C1
F0 "wireless" 50
F1 "wireless.sch" 50
F2 "MISO" I R 6100 9300 50 
F3 "MOSI" I R 6100 9400 50 
F4 "SCK" I R 6100 9200 50 
F5 "~SS~" I R 6100 9500 50 
F6 "GDO0" I R 6100 9600 50 
$EndSheet
Wire Wire Line
	6800 9950 7150 9950
Wire Wire Line
	6100 9600 7150 9600
Wire Wire Line
	7150 9500 6100 9500
Wire Wire Line
	4000 2100 4350 2100
Wire Wire Line
	3700 4150 4200 4150
Wire Notes Line width 20
	14950 600  14950 1700
Wire Wire Line
	6500 4400 7150 4400
Wire Wire Line
	4200 4150 4200 3900
Wire Wire Line
	4200 3900 4350 3900
Wire Wire Line
	6150 4350 4200 4350
Wire Wire Line
	6150 4350 6150 4950
Text Notes 1650 2050 0    200  ~ 40
ISOLATED
Wire Notes Line width 20
	950  600  14950 600 
Wire Wire Line
	8700 1950 10100 1950
Wire Wire Line
	9800 1700 9800 2050
Wire Wire Line
	9500 2100 9500 2300
Wire Wire Line
	9800 2050 9800 2400
Connection ~ 9800 2050
$Comp
L BMS-Master-rescue:Conn_01x05-Connector_Generic J18
U 1 1 5C37653F
P 11150 9250
F 0 "J18" H 11230 9292 50  0000 L CNN
F 1 "OLED" H 11230 9201 50  0000 L CNN
F 2 "Connectors_Molex:Molex_PicoBlade_53398-0571_05x1.25mm_Straight" H 11150 9250 50  0001 C CNN
F 3 "" H 11150 9250 50  0001 C CNN
	1    11150 9250
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:+3.3V-power #PWR0275
U 1 1 5C37728C
P 10500 8650
F 0 "#PWR0275" H 10500 8500 50  0001 C CNN
F 1 "+3.3V" H 10515 8823 50  0000 C CNN
F 2 "" H 10500 8650 50  0001 C CNN
F 3 "" H 10500 8650 50  0001 C CNN
	1    10500 8650
	1    0    0    -1  
$EndComp
Wire Wire Line
	10500 9050 10950 9050
$Comp
L BMS-Master-rescue:GND-power #PWR0276
U 1 1 5C383756
P 10850 9650
F 0 "#PWR0276" H 10850 9400 50  0001 C CNN
F 1 "GND" H 10855 9477 50  0000 C CNN
F 2 "" H 10850 9650 50  0001 C CNN
F 3 "" H 10850 9650 50  0001 C CNN
	1    10850 9650
	1    0    0    -1  
$EndComp
Wire Wire Line
	10950 9450 10850 9450
Wire Wire Line
	10850 9450 10850 9650
$Comp
L BMS-Master-rescue:R-Device R50
U 1 1 5C38FDF3
P 10250 8850
F 0 "R50" V 10450 8850 50  0000 C CNN
F 1 "4.75k" V 10350 8850 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 10180 8850 50  0001 C CNN
F 3 "~" H 10250 8850 50  0001 C CNN
	1    10250 8850
	-1   0    0    1   
$EndComp
$Comp
L BMS-Master-rescue:R-Device R42
U 1 1 5C46EF66
P 9850 8850
F 0 "R42" V 10050 8850 50  0000 C CNN
F 1 "4.75k" V 9950 8850 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 9780 8850 50  0001 C CNN
F 3 "~" H 9850 8850 50  0001 C CNN
	1    9850 8850
	-1   0    0    1   
$EndComp
Wire Wire Line
	8700 9150 10000 9150
Wire Wire Line
	10950 9250 9850 9250
Wire Wire Line
	8700 9350 10250 9350
$Comp
L BMS-Master-rescue:R-Device R46
U 1 1 5C4A29DA
P 10000 9600
F 0 "R46" V 10200 9600 50  0000 C CNN
F 1 "4.75k" V 10100 9600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 9930 9600 50  0001 C CNN
F 3 "~" H 10000 9600 50  0001 C CNN
	1    10000 9600
	-1   0    0    1   
$EndComp
Wire Wire Line
	10500 8650 10500 9050
$Comp
L BMS-Master-rescue:+3.3V-power #PWR0277
U 1 1 5C4A2CC0
P 10250 8650
F 0 "#PWR0277" H 10250 8500 50  0001 C CNN
F 1 "+3.3V" H 10265 8823 50  0000 C CNN
F 2 "" H 10250 8650 50  0001 C CNN
F 3 "" H 10250 8650 50  0001 C CNN
	1    10250 8650
	1    0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:+3.3V-power #PWR0278
U 1 1 5C4A2EC8
P 9850 8650
F 0 "#PWR0278" H 9850 8500 50  0001 C CNN
F 1 "+3.3V" H 9865 8823 50  0000 C CNN
F 2 "" H 9850 8650 50  0001 C CNN
F 3 "" H 9850 8650 50  0001 C CNN
	1    9850 8650
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 8650 9850 8700
Wire Wire Line
	10250 8650 10250 8700
Wire Wire Line
	9850 9000 9850 9250
Connection ~ 9850 9250
Wire Wire Line
	9850 9250 8700 9250
Wire Wire Line
	10250 9000 10250 9350
Connection ~ 10250 9350
Wire Wire Line
	10250 9350 10950 9350
Wire Wire Line
	10000 9450 10000 9150
Connection ~ 10000 9150
Wire Wire Line
	10000 9150 10950 9150
$Comp
L BMS-Master-rescue:GND-power #PWR0279
U 1 1 5C4E36D2
P 10000 9850
F 0 "#PWR0279" H 10000 9600 50  0001 C CNN
F 1 "GND" H 10005 9677 50  0000 C CNN
F 2 "" H 10000 9850 50  0001 C CNN
F 3 "" H 10000 9850 50  0001 C CNN
	1    10000 9850
	1    0    0    -1  
$EndComp
Wire Wire Line
	10000 9750 10000 9850
Wire Notes Line width 20
	9950 7550 9950 8400
Wire Notes Line width 20
	10100 7550 10100 8400
Wire Wire Line
	6250 4800 6250 4000
Wire Wire Line
	6250 4000 6100 4000
Wire Wire Line
	6100 3900 6300 3900
Wire Wire Line
	6300 3900 6300 4700
$Comp
L power:GND #PWR0113
U 1 1 5C686A2F
P 9800 2500
F 0 "#PWR0113" H 9800 2250 50  0001 C CNN
F 1 "GND" H 9805 2327 50  0000 C CNN
F 2 "" H 9800 2500 50  0001 C CNN
F 3 "" H 9800 2500 50  0001 C CNN
	1    9800 2500
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J13
U 1 1 5C382B94
P 14400 1250
F 0 "J13" H 14480 1242 50  0000 L CNN
F 1 "Discharge" H 14480 1151 50  0000 L CNN
F 2 "TerminalBlock_RND:TerminalBlock_RND_205-00001_Pitch5.00mm" H 14400 1250 50  0001 C CNN
F 3 "~" H 14400 1250 50  0001 C CNN
	1    14400 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	13600 1350 14200 1350
Wire Wire Line
	13600 1350 13600 1700
Wire Wire Line
	13600 1350 13600 1050
Wire Wire Line
	13600 1050 14200 1050
Connection ~ 13600 1350
$Comp
L Connector_Generic:Conn_01x02 J1
U 1 1 5C39EE7F
P 1350 900
F 0 "J1" H 1268 1117 50  0000 C CNN
F 1 "BAT" H 1268 1026 50  0000 C CNN
F 2 "TerminalBlock_RND:TerminalBlock_RND_205-00001_Pitch5.00mm" H 1350 900 50  0001 C CNN
F 3 "~" H 1350 900 50  0001 C CNN
	1    1350 900 
	-1   0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:Conn_01x02-Connector_Generic J19
U 1 1 5C6B096E
P 6900 5800
F 0 "J19" H 6979 5792 50  0000 L CNN
F 1 "Master" H 6979 5701 50  0000 L CNN
F 2 "Connectors_Molex:Molex_KK-6410-02_02x2.54mm_Straight" H 6900 5800 50  0001 C CNN
F 3 "~" H 6900 5800 50  0001 C CNN
	1    6900 5800
	-1   0    0    -1  
$EndComp
$Sheet
S 7150 3650 1550 6450
U 5AB72D4D
F0 "MCU" 50
F1 "MCU.sch" 50
F2 "ChargeDetect" B L 7150 4550 50 
F3 "PreChargeEnable" I L 7150 3800 50 
F4 "DischargeEnable" I L 7150 3900 50 
F5 "PowerEnable" I L 7150 4700 50 
F6 "PowerButton" I L 7150 4800 50 
F7 "PowerLED" I L 7150 4950 50 
F8 "LED0" I L 7150 9950 50 
F9 "ChargeEnable" I L 7150 4100 50 
F10 "CAN_RX" I R 8700 7950 50 
F11 "CAN_TX" I R 8700 8050 50 
F12 "BUZZER" I R 8700 9950 50 
F13 "TX_USB" I R 8700 6800 50 
F14 "RX_USB" I R 8700 6900 50 
F15 "Safety" I L 7150 3700 50 
F16 "VoutSense" B R 8700 4100 50 
F17 "SCK" I L 7150 9200 50 
F18 "MISO" I L 7150 9300 50 
F19 "MOSI" I L 7150 9400 50 
F20 "SPI_WT_SS" I L 7150 9500 50 
F21 "SCL" I L 7150 4300 50 
F22 "SDA" I L 7150 4400 50 
F23 "OLED_SCL" I R 8700 9350 50 
F24 "OLED_SDA" I R 8700 9250 50 
F25 "OLED_RST" I R 8700 9150 50 
F26 "Wireless_GDO0" I L 7150 9600 50 
F27 "SwitchEnable" I L 7150 4000 50 
F28 "NTC_Switch" B R 8700 4200 50 
F29 "MA" I L 7150 5800 50 
F30 "PA" I L 7150 5900 50 
$EndSheet
Wire Wire Line
	7100 5800 7150 5800
Wire Wire Line
	7100 5900 7150 5900
Wire Notes Line width 20
	950  600  950  1650
Wire Wire Line
	6100 9400 7150 9400
Wire Wire Line
	6100 9300 7150 9300
Wire Wire Line
	6100 9200 7150 9200
Wire Wire Line
	1550 900  4350 900 
Wire Notes Line width 20
	950  1650 14950 1700
$Comp
L Connector_Generic:Conn_01x01 J4
U 1 1 5C3F429D
P 650 800
F 0 "J4" H 568 575 50  0000 C CNN
F 1 "Corner" H 568 666 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.7mm_M2.5_Pad_Via" H 650 800 50  0001 C CNN
F 3 "~" H 650 800 50  0001 C CNN
	1    650  800 
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J5
U 1 1 5C3F5593
P 650 1150
F 0 "J5" H 568 925 50  0000 C CNN
F 1 "Corner" H 568 1016 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.7mm_M2.5_Pad_Via" H 650 1150 50  0001 C CNN
F 3 "~" H 650 1150 50  0001 C CNN
	1    650  1150
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J14
U 1 1 5C3F5D5A
P 650 1450
F 0 "J14" H 568 1225 50  0000 C CNN
F 1 "Corner" H 568 1316 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.7mm_M2.5_Pad_Via" H 650 1450 50  0001 C CNN
F 3 "~" H 650 1450 50  0001 C CNN
	1    650  1450
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J15
U 1 1 5C3F5F67
P 650 1750
F 0 "J15" H 568 1525 50  0000 C CNN
F 1 "Corner" H 568 1616 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_2.7mm_M2.5_Pad_Via" H 650 1750 50  0001 C CNN
F 3 "~" H 650 1750 50  0001 C CNN
	1    650  1750
	-1   0    0    1   
$EndComp
NoConn ~ 850  800 
NoConn ~ 850  1150
NoConn ~ 850  1450
NoConn ~ 850  1750
$Comp
L BMS-Master-rescue:GND-power #PWR0110
U 1 1 5C44BDEC
P 4100 2350
F 0 "#PWR0110" H 4100 2100 50  0001 C CNN
F 1 "GND" H 4105 2177 50  0000 C CNN
F 2 "" H 4100 2350 50  0001 C CNN
F 3 "" H 4100 2350 50  0001 C CNN
	1    4100 2350
	-1   0    0    -1  
$EndComp
$Comp
L BMS-Master-rescue:+3.3V-power #PWR0123
U 1 1 5C44C08D
P 4100 1950
F 0 "#PWR0123" H 4100 1800 50  0001 C CNN
F 1 "+3.3V" H 4115 2123 50  0000 C CNN
F 2 "" H 4100 1950 50  0001 C CNN
F 3 "" H 4100 1950 50  0001 C CNN
	1    4100 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 2000 4100 2000
Wire Wire Line
	4100 2000 4100 1950
$Comp
L Graphic:Logo_Open_Hardware_Small LOGO1
U 1 1 5C3FA689
P 12550 10250
F 0 "LOGO1" H 12550 10525 50  0001 C CNN
F 1 "Logo_Open_Hardware_Small" H 12550 10025 50  0001 C CNN
F 2 "ENNOID:ENNOID-LOGO" H 12550 10250 50  0001 C CNN
F 3 "~" H 12550 10250 50  0001 C CNN
	1    12550 10250
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J2
U 1 1 5C3822E8
P 14400 950
F 0 "J2" H 14480 942 50  0000 L CNN
F 1 "Charge" H 14480 851 50  0000 L CNN
F 2 "TerminalBlock_RND:TerminalBlock_RND_205-00001_Pitch5.00mm" H 14400 950 50  0001 C CNN
F 3 "~" H 14400 950 50  0001 C CNN
	1    14400 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 2450 6250 2450
Wire Wire Line
	6250 3300 6650 3300
Wire Wire Line
	4000 2200 4350 2200
$Comp
L BMS-Master-rescue:Fuse-Device F1
U 1 1 5ACF057B
P 4050 3600
F 0 "F1" V 3800 3600 50  0000 C CNN
F 1 "5A" V 3900 3600 50  0000 C CNN
F 2 "Fuse_Holders_and_Fuses:Fuse_SMD1206_HandSoldering" V 3980 3600 50  0001 C CNN
F 3 "~" H 4050 3600 50  0001 C CNN
	1    4050 3600
	0    -1   1    0   
$EndComp
Connection ~ 6250 3300
Wire Wire Line
	6100 3300 6250 3300
$Sheet
S 4350 3200 1750 950 
U 5AC18063
F0 "PowerSupply" 50
F1 "PowerSupply.sch" 50
F2 "PushButton" I L 4350 3900 50 
F3 "PowerButton" I R 6100 4000 50 
F4 "PowerEnable" I R 6100 3900 50 
F5 "ExternalPower" I L 4350 3600 50 
F6 "ChargeDetect" I R 6100 3800 50 
F7 "24V" O R 6100 3300 50 
F8 "EnableFromExt" I R 6100 3500 50 
$EndSheet
Wire Wire Line
	6250 2450 6250 3300
Wire Wire Line
	3900 3600 3650 3600
$EndSCHEMATC
