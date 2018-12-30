.. include:: ./macros.rst



==========================
The |foxbms| Documentation
==========================

Welcome to the `foxBMS <https://www.foxbms.org/>`_ documentation of the first
modular open source BMS development platform from the `Fraunhofer IISB
<http://www.iisb.fraunhofer.de/>`_. |foxBMS| is a free, open and flexible
development environment to design state-of-the-art complex battery management
systems.

.. note::
    The present version of the |foxbms| Sphinx documentation is |version|. It
    is available in HTML, PDF and EPUB formats through the bottom left menu on
    `Read the Docs <https://foxbms.readthedocs.io>`_. More detailed information
    on the version history can be found in the
    :ref:`general_information_releases` section. This documentation was
    generated on |timestamp|.


.. warning::
    The |foxbms| hardware and software is under permanent development. The free
    and open research and development platform |foxbms| as presented in the
    documentation hereafter is not provided to be used without any adaptions
    (e.g., to fulfill mandatory regulations) in consumer products, electric
    vehicles, production environments or any similar usages: it is only
    intended to be used by skilled professionals trained in designing battery
    system prototypes.


The documentation is divided in 4 parts:

 #. :ref:`general_information`
 #. :ref:`getting_started`
 #. :ref:`software_documentation`
 #. :ref:`hardware_documentation`

In the first part of the documentation, general information about |foxbms|
can be found (:ref:`general_information`):

 - Information about the versions and the related release notes of |foxbms|
 - Licenses of the hardware, software and document parts of |foxbms|
 - Motivation behind the |foxbms| initiative
 - Overview of the hardware and software of |foxbms|
 - Roadmap of the upcoming hardware and software components
 - Team involved in the development of |foxbms|

In the second part of the documentation, the :ref:`getting_started` guide can
be followed to commission |foxbms| rapidly. For getting |foxbms| desktop
software, obtaining and compiling the sources, and setting up a short test of
hard- and software the minium sections to be read are sections 1 to 4 of
:ref:`getting_started`

 - The location of the electronic schematic and layout files in the HTML
   documentation is indicated
 - The hardware guide describes how the hardware is structured and works
 - The software guide explains how to configure and flash the software
 - The CAN documentation shows how to communicate with the system via the CAN
   bus

To study and modify the software, the :ref:`software_documentation` presents
the structure of the software and of the most important modules, how the basic
tasks are running and how to call user-defined functions. Further, a FAQ is
provided to answers the most common questions. A detailed description of the
software functions and variables can be found in the |doxygen| documentation.

Finally, the :ref:`hardware_documentation` gives the details needed to
understand the |foxbms| hardware.


.. _general_information:

.. toctree::
    :titlesonly:
    :numbered:
    :maxdepth: 1
    :caption: General Information

    ./general_information/releases/releases.rst
    ./general_information/changelog/changelog.rst
    ./general_information/overview/overview.rst
    ./general_information/motivation/motivation.rst
    ./general_information/safety/safety.rst
    ./general_information/licenses/licenses.rst
    ./general_information/team/team.rst


.. _getting_started:

.. toctree::
    :titlesonly:
    :numbered:
    :maxdepth: 1
    :caption: Getting Started

    ./getting_started/general/general.rst
    ./getting_started/foxconda/foxconda.rst
    ./getting_started/build/build.rst
    ./getting_started/eclipse_workspace/eclipse_workspace.rst
    ./getting_started/cabling/cabling.rst
    ./getting_started/connectors/connectors.rst
    ./getting_started/communicating/communicating.rst
    ./getting_started/checking/checking.rst


.. _hardware_documentation:

.. toctree::
    :titlesonly:
    :numbered:
    :maxdepth: 1
    :caption: Hardware Documentation

    ./hardware_documentation/specifications/specifications.rst
    ./hardware_documentation/master-unit/master-unit.rst
    ./hardware_documentation/slaves/slaves.rst
    ./hardware_documentation/design_resources/design_resources.rst
    ./hardware_documentation/components/components.rst
    ./hardware_documentation/bjb/bjb.rst
    ./hardware_documentation/toolchain/toolchain.rst
    ./hardware_documentation/casing/casing.rst


.. _software_documentation:

.. toctree::
    :titlesonly:
    :numbered:
    :maxdepth: 1
    :caption: Software Documentation

    ./software_documentation/components/components.rst
    ./software_documentation/architecture/architecture.rst
    ./software_documentation/overview/overview.rst
    ./software_documentation/defines/defines.rst
    ./software_documentation/modules/modules.rst
    ./software_documentation/tools/tools.rst
    ./software_documentation/build-process/build-process.rst
    ./software_documentation/styleguide/styleguide.rst
    ./software_documentation/faq/faq.rst
