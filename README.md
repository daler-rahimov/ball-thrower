

AT Commands 
=======================

## Serial properties
```
BoudRate = 115200
Parity 	 = None 
DataBits = 8 
StopBits = None 
Handshake= None 
```

## Command Syntax  
### Computer parameter type commands:
```SOH|CMD|ETX``` 
```
Where 
	SOH is a control-A character (ASCII 01).
	ETX to be a Control-C chracter (ASCII 01). 

	CMD is 3 bytes character (ASCII).: 
		1st byte - command prefix. E.g 'g'-for get and 's'-for set  
		2nd byte - command name. E.g. 'p'- for shot power. Anything for other (e.g. version)
		3rd bype - command value. E.g. '100'- 100% 	
```

Examples: </br> 
```SOH|v0|ETX``` - get version number  

For RealTerm: </br>
<`0x01 0x73 0x70 0x60 0x03`> - set "shot power" to 0x60 


### Commands 
All commands are **Case sensetive**

| Command | Description |
| --- | --- |
| `v` | Get version. |
| `sm<mode_num>` | Set mode to <mode_num>. |
| `ss<spin>` | Set spin in presentage from -100 (100% backspin) +100 (100% topspin). <spin> in **BYTE** |
| `sp<shot_power>` | Set shot power (0 to 100 persentage). <shot_power> in **BYTE**| 
| `gs` | Get spin from -100 (100% backspin) +100 (100% topspin).| 
| `gp` | Get shot power (0 to 100 persentage).|

List of Modes
```
	FLAT_SHOT =		 		 1,
	TOPSPIN_SHOT =			 2,
	DROP_SHOT =				 3,
	PASSING_SHOT =			 4,
	SLICE_SHOT =			 5,
	LOB_SHOT =			 	 6,
	SERVE_SHOT =			 7,
	STOP_SHOT = 			 8, 

```

#### Result Codes. 
API return int8 (a byte) for all calls. All error are negative number and values are positive. If '0' it can be OK for setters or value for getters. 

| Numeric form | Verbose form             |
|--------------|--------------------------|
| 0            | OK                       |
|-1            | ERROR                    |
|-2            | NO_SUCK_MODE             |
|-3            | CMD_INC_FORM             |
|-4            | PARTIAL_CMD              |
|-5            | MODE_NOT_SET             |
|-6            | NO_SUCH_CMD              |
|-7            | NO_IMPLEMENT             |



Installation
=================

### Installation from deb repo 
```

```



Avrdude cmds 
=================

### Find serial USB port if connected  
```
ls /dev/serial/by-path/
```

### Reupload to arduino
```
sudo /opt/ball-thrower/bin//arduino-cli upload -p /dev/serial/by-path/pci-0000:00:15.0-usb-0:4:1.0-port0 --fqbn arduino:avr:nano:cpu=atmega328old /opt/ball-thrower/bin//ballThrower/
```

### Load to Mega from UP^2
```
avrdude -v -p atmega2560 -c wiring -P /dev/ttyS5 -b 115200 -D -U flash:w:/opt/ball-thrower/bin/ballThrower/ballThrower.arduino.avr.mega.with_bootloader.hex:i
```

### Load to Nano from UP^2
```
avrdude-original -v -p atmega2560 -c wiring -P /dev/ttyS5 -b 115200 -D -U flash:w:/opt/ball-thrower/bin/ballThrower/ballThrower.arduino.avr.mega.with_bootloader.hex:i
```


UP Square
=====================

### UART
#### To identify the tty device node number in Linux corresponding to a particular hardware uart open a terminal and execute the following command

```
$ ls /sys/bus/pci/devices/0000\:00\:18.?/dw-apb-uart.*/tty/ | grep tty

/sys/bus/pci/devices/0000:00:18.0/dw-apb-uart.8/tty/:
ttyS0
/sys/bus/pci/devices/0000:00:18.1/dw-apb-uart.9/tty/:
ttyS1
```
The first UART (associated to dw-apb-uart.8) is the uart on the M10 connector, and the one associated with dw-apb-uart.9 is the one on the HAT.

So to access the uart on the HAT on ubilinux I have to open the device file /devttyS1

```
sudo screen /dev/ttyS1 115200
```
### Changing cernal for GPIO pin to work 
Follow these manual to setup required component for UP2 board:<br /> 
https://wiki.up-community.org/Ubuntu#Install_Ubuntu_kernel_4.15.0_for_UP_from_PPA_on_Ubuntu_16.04<br /> 
https://wiki.up-community.org/Ubuntu#Enable_the_HAT_functionality_from_userspace<br /> 
https://wiki.up-community.org/Pinout_UP2#Installing_ACPI_overrides_to_enable_spi_in_userspace<br /> 


