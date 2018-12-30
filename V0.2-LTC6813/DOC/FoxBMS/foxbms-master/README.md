# foxBMS

foxBMS is a free, open and flexible development environment to design battery
management systems. It is the first modular open source BMS development
platform.

The foxConda environment. This environment provides all the tools necessary
to generate the documentation, compile the code for the MCUs and flash the
generated binaries on the MCUs (e.g., Python, git, GCC). foxconda can be
downloaded [here](https://iisb-foxbms.iisb.fraunhofer.de/foxbms/).

The starting point to get foxBMS is the foxBMS repository
(https://github.com/foxBMS/foxbms), which contains all files for the foxBMS
project.

## foxBMS Repository

- In `foxbms\documentation` the configuration of the doxygen documentation for
  the primary and secondary mcu can be found.
- The software for the mcus is found in `foxbms\embedded-software`. This
  includes the Hardward Abstraction Layer (HAL), the real-time OS FreeRTOS
  and the BMS application itself.
- The layout and schematic files for the foxBMS hardware are found in the
  `foxbms\hardware`.
- The build toolchain and other tools related to foxBMS are found in
  `foxbms\tools`.

A generated version of the Sphinx documentation can be found at
http://docs.foxbms.org. It explains the structure of the foxBMS hardware, how
to install the foxConda environment and how to use foxConda to compile and
flash the sources.

## Building the Sources
For building the software, open a shell and type `python tools\waf --help`. All
available build options will be displayed. The top build directory is
`foxbms\build`.

https://foxbms.org/
