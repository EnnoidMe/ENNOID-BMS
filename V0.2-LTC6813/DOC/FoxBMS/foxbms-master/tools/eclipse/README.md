# Eclipse Settings

## Setting Up An Eclipse Workspace

See TODO on how to setup an Eclipse workspace to work with foxBMS.

It is assumed that foxconda is installed to ``C:\foxconda3\``.

## Settings

### C/C++ Build
    Enviroment
        FOXCONDA3:  C:\foxconda3;
                    C:\foxconda3\Lib;
                    C:\foxconda3\Library\bin;
                    C:\foxconda3\Scripts;
        PATH:       ${FOXCONDA3};

- When adding the ``PATH`` environment variable it is recommended to remove
  the parts that are automatically added by Eclipse.
- Choose ``Append variables to native environment``.

### C/C++ General
    Paths and Symbols
        Includes:   C:\foxconda3\Library\arm-none-eabi\include
                    C:\foxconda3\Library\lib\gcc\arm-none-eabi\4.9.3\include
