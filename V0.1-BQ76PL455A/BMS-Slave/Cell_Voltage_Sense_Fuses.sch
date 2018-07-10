EESchema Schematic File Version 4
LIBS:slave-cache
EELAYER 26 0
EELAYER END
$Descr User 17184 10717
encoding utf-8
Sheet 5 18
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 8400 1300 0    20   BiDi ~ 0
FUSED_VBAT+
Wire Wire Line
	8400 1300 9800 1300
Wire Wire Line
	9800 1300 9800 1200
Connection ~ 6600 1300
Text GLabel 6100 1200 0    20   BiDi ~ 0
VBAT+
Wire Wire Line
	6100 1200 6100 1300
Wire Wire Line
	6100 1300 6600 1300
Wire Wire Line
	6600 1300 8000 1300
Wire Wire Line
	6600 1500 6600 1300
Connection ~ 6600 6500
Text GLabel 6100 6500 0    20   BiDi ~ 0
VBAT-
Wire Wire Line
	6100 6500 6600 6500
Wire Wire Line
	6600 6500 8000 6500
Wire Wire Line
	6100 6600 6100 6500
Wire Wire Line
	6600 6500 6600 6300
Text GLabel 6300 5700 2    50   BiDi ~ 0
CELL_0-
Connection ~ 6600 5700
Wire Wire Line
	8000 5700 6600 5700
Wire Wire Line
	6600 5700 6300 5700
Wire Wire Line
	6600 5900 6600 5700
Text GLabel 6300 5400 2    50   BiDi ~ 0
CELL_0+
Wire Wire Line
	8000 5400 6300 5400
Text GLabel 6300 5100 0    50   BiDi ~ 0
CELL_1+
Wire Wire Line
	8000 5100 6300 5100
Text GLabel 6300 4800 2    50   BiDi ~ 0
CELL_2+
Wire Wire Line
	8000 4800 6300 4800
Text GLabel 6300 4500 0    50   BiDi ~ 0
CELL_3+
Wire Wire Line
	8000 4500 6300 4500
Text GLabel 6300 4200 2    50   BiDi ~ 0
CELL_4+
Wire Wire Line
	8000 4200 6300 4200
Text GLabel 6300 3900 0    50   BiDi ~ 0
CELL_5+
Wire Wire Line
	8000 3900 6300 3900
Text GLabel 6300 3600 2    50   BiDi ~ 0
CELL_6+
Wire Wire Line
	8000 3600 6300 3600
Text GLabel 6300 3300 0    50   BiDi ~ 0
CELL_7+
Wire Wire Line
	8000 3300 6300 3300
Text GLabel 6300 3000 2    50   BiDi ~ 0
CELL_8+
Wire Wire Line
	8000 3000 6300 3000
Text GLabel 6300 2700 0    50   BiDi ~ 0
CELL_9+
Wire Wire Line
	8000 2700 6300 2700
Text GLabel 6300 2400 2    50   BiDi ~ 0
CELL_10+
Wire Wire Line
	8000 2400 6300 2400
Text GLabel 6300 2100 0    50   BiDi ~ 0
CELL_11+
Connection ~ 6600 2100
Wire Wire Line
	8000 2100 6600 2100
Wire Wire Line
	6600 2100 6300 2100
Wire Wire Line
	6600 1900 6600 2100
Text GLabel 9300 5700 0    50   BiDi ~ 0
FUSED_CELL_0-
Wire Wire Line
	8400 5700 9300 5700
Text GLabel 9300 5400 0    50   BiDi ~ 0
FUSED_CELL_0+
Wire Wire Line
	8400 5400 9300 5400
Text GLabel 9300 5100 0    50   BiDi ~ 0
FUSED_CELL_1+
Wire Wire Line
	8400 5100 9300 5100
Text GLabel 9300 4800 0    50   BiDi ~ 0
FUSED_CELL_2+
Wire Wire Line
	8400 4800 9300 4800
Text GLabel 9300 4500 0    50   BiDi ~ 0
FUSED_CELL_3+
Wire Wire Line
	8400 4500 9300 4500
Text GLabel 9300 4200 0    50   BiDi ~ 0
FUSED_CELL_4+
Wire Wire Line
	8400 4200 9300 4200
Text GLabel 9300 3900 0    50   BiDi ~ 0
FUSED_CELL_5+
Wire Wire Line
	8400 3900 9300 3900
Text GLabel 9300 3600 0    50   BiDi ~ 0
FUSED_CELL_6+
Wire Wire Line
	8400 3600 9300 3600
Text GLabel 9300 3300 0    50   BiDi ~ 0
FUSED_CELL_7+
Wire Wire Line
	8400 3300 9300 3300
Text GLabel 9300 3000 0    50   BiDi ~ 0
FUSED_CELL_8+
Wire Wire Line
	8400 3000 9300 3000
Text GLabel 9300 2700 0    50   BiDi ~ 0
FUSED_CELL_9+
Wire Wire Line
	8400 2700 9300 2700
Text GLabel 9300 2400 0    50   BiDi ~ 0
FUSED_CELL_10+
Wire Wire Line
	8400 2400 9300 2400
Text GLabel 9300 2100 0    50   BiDi ~ 0
FUSED_CELL_11+
Wire Wire Line
	8400 2100 9300 2100
Text GLabel 8400 6500 0    20   BiDi ~ 0
FUSED_VBAT-
Wire Wire Line
	8400 6500 9800 6500
Wire Wire Line
	9800 6500 9800 6600
$Comp
L slave-eagle-import:0,250A_LITTELFUSE_466_1206 F414
U 1 0 771E7428005B6F9B
P 8200 5700
AR Path="/771E7428005B6F9B" Ref="F414"  Part="1" 
AR Path="/5A91CCAE/771E7428005B6F9B" Ref="F414"  Part="1" 
F 0 "F414" H 8200 5750 56  0000 C BNN
F 1 "FUSE: 0.250A, 32V, 0.57R" H 8200 5700 50  0001 C CNN
F 2 "R_SMD_CHIP_1206_R_LITTLEFUSE_REV_01" H 8200 5700 50  0001 C CNN
F 3 "" H 8200 5700 50  0001 C CNN
	1    8200 5700
	1    0    0    1   
$EndComp
$Comp
L slave-eagle-import:0,250A_LITTELFUSE_466_1206 F413
U 1 0 F62B1CCDDAC3436C
P 8200 5400
AR Path="/F62B1CCDDAC3436C" Ref="F413"  Part="1" 
AR Path="/5A91CCAE/F62B1CCDDAC3436C" Ref="F413"  Part="1" 
F 0 "F413" H 8200 5450 56  0000 C BNN
F 1 "FUSE: 0.250A, 32V, 0.57R" H 8200 5400 50  0001 C CNN
F 2 "R_SMD_CHIP_1206_R_LITTLEFUSE_REV_01" H 8200 5400 50  0001 C CNN
F 3 "" H 8200 5400 50  0001 C CNN
	1    8200 5400
	1    0    0    1   
$EndComp
$Comp
L slave-eagle-import:0,250A_LITTELFUSE_466_1206 F412
U 1 0 F3D0EBCD149DF961
P 8200 5100
AR Path="/F3D0EBCD149DF961" Ref="F412"  Part="1" 
AR Path="/5A91CCAE/F3D0EBCD149DF961" Ref="F412"  Part="1" 
F 0 "F412" H 8200 5150 56  0000 C BNN
F 1 "FUSE: 0.250A, 32V, 0.57R" H 8200 5100 50  0001 C CNN
F 2 "R_SMD_CHIP_1206_R_LITTLEFUSE_REV_01" H 8200 5100 50  0001 C CNN
F 3 "" H 8200 5100 50  0001 C CNN
	1    8200 5100
	1    0    0    1   
$EndComp
$Comp
L slave-eagle-import:0,250A_LITTELFUSE_466_1206 F411
U 1 0 C20CC309CB043878
P 8200 4800
AR Path="/C20CC309CB043878" Ref="F411"  Part="1" 
AR Path="/5A91CCAE/C20CC309CB043878" Ref="F411"  Part="1" 
F 0 "F411" H 8200 4850 56  0000 C BNN
F 1 "FUSE: 0.250A, 32V, 0.57R" H 8200 4800 50  0001 C CNN
F 2 "R_SMD_CHIP_1206_R_LITTLEFUSE_REV_01" H 8200 4800 50  0001 C CNN
F 3 "" H 8200 4800 50  0001 C CNN
	1    8200 4800
	1    0    0    1   
$EndComp
$Comp
L slave-eagle-import:0,250A_LITTELFUSE_466_1206 F410
U 1 0 DD2CB9DFC6953DC7
P 8200 4500
AR Path="/DD2CB9DFC6953DC7" Ref="F410"  Part="1" 
AR Path="/5A91CCAE/DD2CB9DFC6953DC7" Ref="F410"  Part="1" 
F 0 "F410" H 8200 4550 56  0000 C BNN
F 1 "FUSE: 0.250A, 32V, 0.57R" H 8200 4500 50  0001 C CNN
F 2 "R_SMD_CHIP_1206_R_LITTLEFUSE_REV_01" H 8200 4500 50  0001 C CNN
F 3 "" H 8200 4500 50  0001 C CNN
	1    8200 4500
	1    0    0    1   
$EndComp
$Comp
L slave-eagle-import:0,250A_LITTELFUSE_466_1206 F409
U 1 0 68F7E0852B02309B
P 8200 4200
AR Path="/68F7E0852B02309B" Ref="F409"  Part="1" 
AR Path="/5A91CCAE/68F7E0852B02309B" Ref="F409"  Part="1" 
F 0 "F409" H 8200 4250 56  0000 C BNN
F 1 "FUSE: 0.250A, 32V, 0.57R" H 8200 4200 50  0001 C CNN
F 2 "R_SMD_CHIP_1206_R_LITTLEFUSE_REV_01" H 8200 4200 50  0001 C CNN
F 3 "" H 8200 4200 50  0001 C CNN
	1    8200 4200
	1    0    0    1   
$EndComp
$Comp
L slave-eagle-import:0,250A_LITTELFUSE_466_1206 F408
U 1 0 7986316527ABA20B
P 8200 3900
AR Path="/7986316527ABA20B" Ref="F408"  Part="1" 
AR Path="/5A91CCAE/7986316527ABA20B" Ref="F408"  Part="1" 
F 0 "F408" H 8200 3950 56  0000 C BNN
F 1 "FUSE: 0.250A, 32V, 0.57R" H 8200 3900 50  0001 C CNN
F 2 "R_SMD_CHIP_1206_R_LITTLEFUSE_REV_01" H 8200 3900 50  0001 C CNN
F 3 "" H 8200 3900 50  0001 C CNN
	1    8200 3900
	1    0    0    1   
$EndComp
$Comp
L slave-eagle-import:0,250A_LITTELFUSE_466_1206 F407
U 1 0 3A2EF09ABB9C9487
P 8200 3600
AR Path="/3A2EF09ABB9C9487" Ref="F407"  Part="1" 
AR Path="/5A91CCAE/3A2EF09ABB9C9487" Ref="F407"  Part="1" 
F 0 "F407" H 8200 3650 56  0000 C BNN
F 1 "FUSE: 0.250A, 32V, 0.57R" H 8200 3600 50  0001 C CNN
F 2 "R_SMD_CHIP_1206_R_LITTLEFUSE_REV_01" H 8200 3600 50  0001 C CNN
F 3 "" H 8200 3600 50  0001 C CNN
	1    8200 3600
	1    0    0    1   
$EndComp
$Comp
L slave-eagle-import:0,250A_LITTELFUSE_466_1206 F406
U 1 0 12D243FFE38616AA
P 8200 3300
AR Path="/12D243FFE38616AA" Ref="F406"  Part="1" 
AR Path="/5A91CCAE/12D243FFE38616AA" Ref="F406"  Part="1" 
F 0 "F406" H 8200 3350 56  0000 C BNN
F 1 "FUSE: 0.250A, 32V, 0.57R" H 8200 3300 50  0001 C CNN
F 2 "R_SMD_CHIP_1206_R_LITTLEFUSE_REV_01" H 8200 3300 50  0001 C CNN
F 3 "" H 8200 3300 50  0001 C CNN
	1    8200 3300
	1    0    0    1   
$EndComp
$Comp
L slave-eagle-import:0,250A_LITTELFUSE_466_1206 F405
U 1 0 88AD788DBB33D681
P 8200 3000
AR Path="/88AD788DBB33D681" Ref="F405"  Part="1" 
AR Path="/5A91CCAE/88AD788DBB33D681" Ref="F405"  Part="1" 
F 0 "F405" H 8200 3050 56  0000 C BNN
F 1 "FUSE: 0.250A, 32V, 0.57R" H 8200 3000 50  0001 C CNN
F 2 "R_SMD_CHIP_1206_R_LITTLEFUSE_REV_01" H 8200 3000 50  0001 C CNN
F 3 "" H 8200 3000 50  0001 C CNN
	1    8200 3000
	1    0    0    1   
$EndComp
$Comp
L slave-eagle-import:0,250A_LITTELFUSE_466_1206 F404
U 1 0 5FD2802B249757DE
P 8200 2700
AR Path="/5FD2802B249757DE" Ref="F404"  Part="1" 
AR Path="/5A91CCAE/5FD2802B249757DE" Ref="F404"  Part="1" 
F 0 "F404" H 8200 2750 56  0000 C BNN
F 1 "FUSE: 0.250A, 32V, 0.57R" H 8200 2700 50  0001 C CNN
F 2 "R_SMD_CHIP_1206_R_LITTLEFUSE_REV_01" H 8200 2700 50  0001 C CNN
F 3 "" H 8200 2700 50  0001 C CNN
	1    8200 2700
	1    0    0    1   
$EndComp
$Comp
L slave-eagle-import:0,250A_LITTELFUSE_466_1206 F403
U 1 0 BC5EEC84F3D00073
P 8200 2400
AR Path="/BC5EEC84F3D00073" Ref="F403"  Part="1" 
AR Path="/5A91CCAE/BC5EEC84F3D00073" Ref="F403"  Part="1" 
F 0 "F403" H 8200 2450 56  0000 C BNN
F 1 "FUSE: 0.250A, 32V, 0.57R" H 8200 2400 50  0001 C CNN
F 2 "R_SMD_CHIP_1206_R_LITTLEFUSE_REV_01" H 8200 2400 50  0001 C CNN
F 3 "" H 8200 2400 50  0001 C CNN
	1    8200 2400
	1    0    0    1   
$EndComp
$Comp
L slave-eagle-import:0,250A_LITTELFUSE_466_1206 F402
U 1 0 47D0CB3BCCC55B52
P 8200 2100
AR Path="/47D0CB3BCCC55B52" Ref="F402"  Part="1" 
AR Path="/5A91CCAE/47D0CB3BCCC55B52" Ref="F402"  Part="1" 
F 0 "F402" H 8200 2150 56  0000 C BNN
F 1 "FUSE: 0.250A, 32V, 0.57R" H 8200 2100 50  0001 C CNN
F 2 "R_SMD_CHIP_1206_R_LITTLEFUSE_REV_01" H 8200 2100 50  0001 C CNN
F 3 "" H 8200 2100 50  0001 C CNN
	1    8200 2100
	1    0    0    1   
$EndComp
$Comp
L slave-eagle-import:0,500A_LITTELFUSE_448 F415
U 1 0 33E5F72F4B92D102
P 8200 6500
AR Path="/33E5F72F4B92D102" Ref="F415"  Part="1" 
AR Path="/5A91CCAE/33E5F72F4B92D102" Ref="F415"  Part="1" 
F 0 "F415" H 8200 6550 56  0000 C BNN
F 1 "0.5A, 63V, 0.25R" H 8200 6500 50  0001 C CNN
F 2 "448_SERIES_LITTELFUSE" H 8200 6500 50  0001 C CNN
F 3 "" H 8200 6500 50  0001 C CNN
	1    8200 6500
	1    0    0    1   
$EndComp
$Comp
L slave-eagle-import:0,500A_LITTELFUSE_448 F401
U 1 0 36D8FDF4E7D71210
P 8200 1300
AR Path="/36D8FDF4E7D71210" Ref="F401"  Part="1" 
AR Path="/5A91CCAE/36D8FDF4E7D71210" Ref="F401"  Part="1" 
F 0 "F401" H 8200 1350 56  0000 C BNN
F 1 "0.5A, 63V, 0.25R" H 8200 1300 50  0001 C CNN
F 2 "448_SERIES_LITTELFUSE" H 8200 1300 50  0001 C CNN
F 3 "" H 8200 1300 50  0001 C CNN
	1    8200 1300
	1    0    0    1   
$EndComp
$Comp
L slave-eagle-import:FUSED_VBAT- U$13
U 1 0 32A94806777B985C
P 9800 6700
AR Path="/32A94806777B985C" Ref="U$13"  Part="1" 
AR Path="/5A91CCAE/32A94806777B985C" Ref="#U$013"  Part="1" 
F 0 "#U$013" H 9800 6700 50  0001 C CNN
F 1 "FUSED_VBAT-" H 9700 6600 59  0000 L BNN
F 2 "" H 9800 6700 50  0001 C CNN
F 3 "" H 9800 6700 50  0001 C CNN
	1    9800 6700
	-1   0    0    -1  
$EndComp
$Comp
L slave-eagle-import:FUSED_VBAT+ U$14
U 1 0 9F7E67D4491AF09A
P 9800 1100
AR Path="/9F7E67D4491AF09A" Ref="U$14"  Part="1" 
AR Path="/5A91CCAE/9F7E67D4491AF09A" Ref="#U$014"  Part="1" 
F 0 "#U$014" H 9800 1100 50  0001 C CNN
F 1 "FUSED_VBAT+" H 9700 1150 59  0000 L BNN
F 2 "" H 9800 1100 50  0001 C CNN
F 3 "" H 9800 1100 50  0001 C CNN
	1    9800 1100
	-1   0    0    -1  
$EndComp
$Comp
L slave-eagle-import:VBAT- VBAT-
U 1 0 EFAF3944DD5E86D2
P 6100 6700
AR Path="/EFAF3944DD5E86D2" Ref="VBAT-"  Part="1" 
AR Path="/5A91CCAE/EFAF3944DD5E86D2" Ref="#VBAT-0101"  Part="1" 
F 0 "#VBAT-0101" H 6100 6700 50  0001 C CNN
F 1 "VBAT-" H 6000 6600 59  0000 L BNN
F 2 "" H 6100 6700 50  0001 C CNN
F 3 "" H 6100 6700 50  0001 C CNN
	1    6100 6700
	-1   0    0    -1  
$EndComp
$Comp
L slave-eagle-import:VBAT+ U$10
U 1 0 47A4CA199B2F7804
P 6100 1100
AR Path="/47A4CA199B2F7804" Ref="U$10"  Part="1" 
AR Path="/5A91CCAE/47A4CA199B2F7804" Ref="#U$010"  Part="1" 
F 0 "#U$010" H 6100 1100 50  0001 C CNN
F 1 "VBAT+" H 6000 1150 59  0000 L BNN
F 2 "" H 6100 1100 50  0001 C CNN
F 3 "" H 6100 1100 50  0001 C CNN
	1    6100 1100
	-1   0    0    -1  
$EndComp
$Comp
L slave-eagle-import:SJ SJ402
U 1 0 7C6F906D811C4BB
P 6600 6100
AR Path="/7C6F906D811C4BB" Ref="SJ402"  Part="1" 
AR Path="/5A91CCAE/7C6F906D811C4BB" Ref="SJ402"  Part="1" 
F 0 "SJ402" H 6500 6200 59  0000 L BNN
F 1 "SJ" H 6500 5950 59  0000 L BNN
F 2 "SJ" H 6600 6100 50  0001 C CNN
F 3 "" H 6600 6100 50  0001 C CNN
	1    6600 6100
	0    -1   1    0   
$EndComp
$Comp
L slave-eagle-import:SJ SJ401
U 1 0 15412CDE9D4CFCAD
P 6600 1700
AR Path="/15412CDE9D4CFCAD" Ref="SJ401"  Part="1" 
AR Path="/5A91CCAE/15412CDE9D4CFCAD" Ref="SJ401"  Part="1" 
F 0 "SJ401" H 6500 1800 59  0000 L BNN
F 1 "SJ" H 6500 1550 59  0000 L BNN
F 2 "SJ" H 6600 1700 50  0001 C CNN
F 3 "" H 6600 1700 50  0001 C CNN
	1    6600 1700
	0    -1   1    0   
$EndComp
$Comp
L slave-eagle-import:FOXBMS_BMS-MASTER_V1.00_A_FRAME_B_L FRAME15
U 1 0 58AC7C9027B48A6A
P 1500 9700
AR Path="/58AC7C9027B48A6A" Ref="FRAME15"  Part="1" 
AR Path="/5A91CCAE/58AC7C9027B48A6A" Ref="FRAME15"  Part="1" 
F 0 "FRAME15" H 1500 9700 50  0001 C CNN
F 1 "FOXBMS_BMS-MASTER_V1.00_A_FRAME_B_L" H 1500 9700 50  0001 C CNN
F 2 "" H 1500 9700 50  0001 C CNN
F 3 "" H 1500 9700 50  0001 C CNN
	1    1500 9700
	1    0    0    -1  
$EndComp
$Comp
L slave-eagle-import:FOXBMS_BMS-MASTER_V1.00_A_FRAME_B_L FRAME15
U 2 0 58AC7C9027B48A66
P 13600 9600
AR Path="/58AC7C9027B48A66" Ref="FRAME15"  Part="2" 
AR Path="/5A91CCAE/58AC7C9027B48A66" Ref="FRAME15"  Part="2" 
F 0 "FRAME15" H 13600 9600 50  0001 C CNN
F 1 "FOXBMS_BMS-MASTER_V1.00_A_FRAME_B_L" H 13600 9600 50  0001 C CNN
F 2 "" H 13600 9600 50  0001 C CNN
F 3 "" H 13600 9600 50  0001 C CNN
	2    13600 9600
	1    0    0    -1  
$EndComp
Text Notes 7000 7600 0    59   ~ 0
For Cell Balancing/ Measuring Inputs: Max. 0.250A Balancing Current \nReaction Time: \n200% Load (0.500A) = ca. 20ms\n300% Load (0.750A) = <10ms
Text Notes 7000 8200 0    59   ~ 0
For Power Supply Inputs: Max. 0.500A Current \nReaction Time:\n200% Load (1.000A) = ca. 200ms\n300% Load (1.500A) = ca. 70ms
Wire Notes Line
	6900 6900 6900 900 
Wire Notes Line
	6900 900  8900 900 
Wire Notes Line
	8900 900  8900 6900
Wire Notes Line
	8900 6900 6900 6900
Text Notes 7000 700  0    59   ~ 0
Fast-Acting Surface Mount Fuse 
Text Notes 14730 8740 0    85   ~ 0
Cell Voltage Sense Fuses
$EndSCHEMATC