

# AT Commands 

-----------------
NOTE:**Any mode plays for 1 min. After than it switches to waiting mode.**  

### Serial properties
BoudRate = 115200
Parity 	 = None 
DataBits = 8 
StopBits = None 
Handshake= None 

## Command Syntax  
### Computer parameter type commands:
```SOH|CMD|ETX``` 

NOTE: computer parameter type commands are exactly 4 byes. 2 for start/end 2 for commands.
and returns exactly 1 byte. 

Where 
	SOH is a control-A character (ASCII 01).
	ETX to be a Control-C chracter (ASCII 01). 

	CMD is 2 bytes character (ASCII).: 
		1st byte - command name. E.g m for mode 
		2nd byte - command number. 

E.g. 
```SOH|m2|ETX``` -	Set mode to 2 
```SOH|v0|ETX``` - get version number  

For RealTerm 
```0x01 0x6d 0x07 0x03``` 
0x07 is the mode number 

### Display parameter type commands: 

```AT+CMD<CR>```
 
Where 
		AT  is the command line prefix, 
		MD is the body of a basic command 
		<CR>  is  the  command  line  terminator character (New line \n)
	 
```AT+CMD=10<CR>``` 

Where 
		10 is a subparameter


#### Commands 
AT+M<Num>	Set mode to <Num> 
AT+V        Get version
List of Modes
```
		GREETING_MODE =		 	 1,
		BOOTING_MODE =			 2,
		WAITING_MODE =			 3,
		LISTENING_MODE =		 4,
		PROCESSING_MODE =		 5,
		SETUP_MODE =			 6,
		TALKING_MODE =			 7,
		ACTIVE_MODE =			 8,
		ALERT_MODE =			 9,
		ERROR_MODE =			 10,
		SUCCESS_MODE =			 11,
		UPDATING_MODE =			 12,
		FACE_REGISTRATION_MODE = 13,
		ACTION_REQUIRED_MODE =	 14,
		LEVEL_MODE =			 15,
		MUTED_MODE =			 16,
		WAITING_MODE2 =			 17,
		TALKING_MODE_NEGATIVE =  18,
		TALKING_MODE_POSITIVE =	 19,
		INTERMEDIATE_MODE =		 20, 

```

#### Result Codes. For "Numeric form" one byte with result code returned

| Numeric form | Verbose form             |
|--------------|--------------------------|
| 0            | OK                       |
| 1            | ERROR                    |
| 2            | NO_SUCK_MODE             |
| 3            | CMD_INC_FORM             |
| 4            | PARTIAL_CMD              |
| 5            | MODE_NOT_SET             |
| 6            | NO_SUCH_CMD              |
| 7            | NO_IMPLEMENT             |



# Installation

-----------------
### Installation from deb repo 
```
sudo -s
echo "deb [arch=amd64] https://dcapp:0pend0wn@apt-petrosoftinc.com/dcapp/ xenial depends devel" > /etc/apt/sources.list.d/act_modules.list
wget -O - https://apt-petrosoftinc.com/repo-data/dcapp.gpg | apt-key add -
apt-get update
apt-get install ball-thrower 
```



# Avrdude cmds 

-------------
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


# UP Square 

-----------
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


