EESchema Schematic File Version 4
LIBS:LTC6811-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 2
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
L Battery_Management:LTC6803-2 U?
U 1 1 5C5553BF
P 5050 3550
F 0 "U?" H 5050 5131 50  0000 C CNN
F 1 "LTC6803-2" H 5050 5040 50  0000 C CNN
F 2 "Package_SO:SSOP-44_5.3x12.8mm_P0.5mm" H 5050 3550 50  0001 C CNN
F 3 "http://cds.linear.com/docs/en/datasheet/680324fa.pdf" H 5600 4900 50  0001 C CNN
	1    5050 3550
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x16 J?
U 1 1 5C556888
P 8550 3350
F 0 "J?" H 8468 2325 50  0000 C CNN
F 1 "Cells" H 8468 2416 50  0000 C CNN
F 2 "" H 8550 3350 50  0001 C CNN
F 3 "~" H 8550 3350 50  0001 C CNN
	1    8550 3350
	1    0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 5C55795A
P 2900 2700
F 0 "R?" H 2970 2746 50  0000 L CNN
F 1 "1M" H 2970 2655 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2830 2700 50  0001 C CNN
F 3 "~" H 2900 2700 50  0001 C CNN
	1    2900 2700
	1    0    0    -1  
$EndComp
$Sheet
S 2250 3800 1250 900 
U 5C4F59F4
F0 "cell" 50
F1 "cell.sch" 50
$EndSheet
$EndSCHEMATC
