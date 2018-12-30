.. include:: ../../macros.rst

.. _software_documentation_architecture:

=====================
Software Architecture
=====================

This section describes the |foxbms| software architecture. The architecture
was chosen to enable a processor independent programming as far as possible.
Fig. :numref:`foxbms_software_architecture` shows the software architecture
of |foxbms|.

..  graphviz::
    :caption: |foxbms| software architecture
    :name: foxbms_software_architecture

        digraph {
            rankdir=TB;
            graph [fontname = "monospace"]
            node [fontname = "monospace"]
            edge [fontname = "monospace"];
            subgraph cluster_architecture {
                label="foxBMS (runs within OS context)";
                app [label="application"];
                engine [label="engine"];
                modules [label="foxBMS-Modules", target="_bottom"];
                modules -> app;
                app -> modules;
                app -> engine;
                engine -> app;
                engine -> modules;
            }
            subgraph cluster_foxbms_hal {
                label="";
                drivers [label="Drivers"];
            }
            subgraph cluster_hal {
                label="";
                target="_bottom";
                hal     [label="HAL"];
            }
            modules -> drivers;
            drivers -> hal;
     }


The software consists of a base hardware abstraction layer of the
microcontroller. The driver layer on top if it is used to configure and
initialize the peripherals of the microcontroller. These hardware dependent
layers resemble the base of the software architecture. The remaining modules
are built on top of it to allow an easy port of the software to different
microcontrollers.

Hardware abstraction layer
==========================

The hardware abstraction layer (HAL) consists of the STM32F4xx HAL Drivers
|version-hal|. Additionaly the Cortex Microcontroller Software Interface Standard
(CMSIS) |version-cmsis| is used as an abstraction layer for the Cortex M4F processor.


Drivers
=======

The driver layer is responsible for the configuration and the initialization of
the controller peripherals. These drivers are dependent on the specific used
microcontroller and the used HAL. Drivers are only allowed to include files
from driver or the lower hardware abstraction layer (i.e. SPI modules includes
DMA module to configure the respective DMA stream).

|foxbms|-Modules
================

|foxbms|-modules initialize, configure and control hardware connected to the
|master|. These modules are only allowed to access the hardware through the drivers
and directly via the HAL. No direct function calls to the HAL should be make
the modules as independent as possible from the used microcontroller. They may
include engine modules (i.e. database, diag), applications (com) or even the
operating system if they need to ensure some runtime specific behaviour. These
modules are called within the operating system to ensure a real-time behaviour.


Operating System
================

The real-time operating system FreeRTOS |version-freertos| is used to manage
the foxBMS-engine and applications. Tasks with different priorities are used to
process the different algorithms and state machines and to ensure a real-time
behaviour of the different modules.

|foxbms|-Engine
===============

Important hardware independent software modules are bundled to the
foxBMS-engine. This involves a database for storing important parameters, a
diagnosis module for error detection and handling as well as the
startup/initialization behaviour of the BMS. The operating system engine tasks
are the tasks with the highest priorites and used to ensure a real-time
behaviour of the safety-critical modules. Engine modules are allowed to include
|foxbms|-Modules, drivers, applications and of course the operating system.

|foxbms|-Application
====================

The application layer gathers all hardware independent modules that are
responsible for the actual BMS functionality (i.e. SOC, SOH, balancing...).
The most important module is the |mod_bms| that monitors the behaviour of the
battery system to ensure an adherence of the safe-operating area and controls
the BMS state (i.e. charging/discharging). The operating system applications
tasks are the tasks with the lowest priorites to ensure a real-time behaviour
of the safety-critical modules. Applications are allowed to include |foxbms|-Modules, drivers, engine-modules and of course
the operating system.
