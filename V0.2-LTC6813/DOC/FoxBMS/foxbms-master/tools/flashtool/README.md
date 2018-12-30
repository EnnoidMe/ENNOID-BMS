# foxBMS Flashing Tool

## Using the foxBMS Flashing Tool Wrapper `flash.py`

### Simple Useage
For the Eclipse workspace there is wrapper for calling the `foxflasher.py`
script named `flash.py`. If you are using Eclipse, you do not need change
anything. Just remeber to only plug only one mcu via USB to your PC.

### Details
`flash.py` reads the configuration from file `flashconfig.yml`. This
configuration is valid for both foxBMS hardwares, primary and secondary.
The `flash.py` wrapper script  has to be called from the top project directory,
which is normaly `foxbms-setup.`
When using AUTODETECT in `flashconfig.yml`. which is default, connect one mcu
with the PC and run:

```
D:\_sourcegate\foxbms-setup>python tools\flashtool\flash.py X
```

where `X` is either `0` for the primary MCU or `1` for the seocndary MCU.

**If the COM ports are hardcoded in the `flashconfig.yml` both MCUs can be
connected to the PC. But you have to make sure, that the COM ports are correct
for primary and secondary, otherwise you might flash the wrong binaries on the
wrong MCU.**

**Only use this feature if you are sure what you are doing.**

## Using the foxBMS Flashing Tool `foxflasher.py` directly

It is also possible to directly use the flashing tool:

```
python foxflasher.py -p <COMX> -e -w -y foxbms_<flashsection>.bin
```

where <COMX> is substituted with your COM port or ttyUSBx device.
