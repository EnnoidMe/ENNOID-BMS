.. include:: ../../macros.rst



.. _hardware_documentation_specifications:

==============
Specifications
==============

The following specifications must be met to ensure a safe and optimal work with the |foxbms| hardware.

------------------------------
Electrical Ratings Master Unit
------------------------------

==================================================  =======     =======     =======     ====
Description                                         Minimum     Typical     Maximum     Unit
==================================================  =======     =======     =======     ====
Supply Voltage DC                                   10          --          26          V
Contactor Continuous Current                        --          --          4           A
Contactor Feedback Supply Voltage                   --          5.0         --          V
Analog Input                                        --          --          3.3         V
Isolated Contacts Continuous Current                --          --          4           A
Interlock Circuit Sink Current                      --          10          --          mA
Idle Supply Current at 12V supply                   --          150         --          mA
Idle Supply Current at 24V supply                   --          110         --          mA
==================================================  =======     =======     =======     ====

--------------------------------
Electrical Ratings 12-Cell Slave
--------------------------------

==================================================  =======     =======     =======     ====
Description                                         Minimum     Typical     Maximum     Unit
==================================================  =======     =======     =======     ====
Battery Module Voltage                              11          --          60          V
Single Battery Cell Voltage                         0           --          5           V
Temperature Sensor Inputs                           --          10k         --          Ω
Analog Inputs (pin headers)                         0           --          5           V
Digital Inputs/Outputs (pin headers)                0           --          5           V
External DC Supply (HW version 2.1.0 and above)     8           12          24          V
==================================================  =======     =======     =======     ====

--------------------------------
Electrical Ratings 18-Cell Slave
--------------------------------

==================================================  =======     =======     =======     ====
Description                                         Minimum     Typical     Maximum     Unit
==================================================  =======     =======     =======     ====
Battery Module Voltage                              16          --          90          V
Single Battery Cell Voltage                         0           --          5           V
Temperature Sensor Inputs                           --          10k         --          Ω
Analog Inputs (pin headers)                         0           --          5           V
Digital Inputs/Outputs (pin headers)                0           --          5           V
External DC Supply                                  8           12          24          V
==================================================  =======     =======     =======     ====
