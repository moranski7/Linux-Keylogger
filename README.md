# Linux-KeyLogger
* Use for educational purpose only.
* An API keylogger that polls the keyboard handler for events and records them.

## Background Information
* The location of character drivers in userspace appears in the /dev/input.
* **proc/bus/input/devices** contains information on all devices except for USB multimedia/office keyboards
* Will need get additional information for USB multimedia/office keyboards using commands like lsusb.
* The command cat /proc/bus/input/devices gives the output:
 Symbol | Description
 --- | ---
| I: |  ID of the device. Divided into four separate parts: Bustype, Vendor, Product, Version
| N: |  Name of the device                                                                   
| P: |  The physical path to the device in the system hierarchy.                             
| S: |  The sysfs path.                                                                      
| U: |  A unique identification code for the device (if available)                           
| H: |  The list of input handles associated with the device.                                
| B: |  The Device-Dependent Bitmaps associated with the device.                             

ex.
| |
---|
| I: Bus=0011 Vendor=0001 Product=0001 Version=ab41 |
| N: Name="AT Translated Set 2 keyboard" |
| P: Phys=isa0060/serio0/input0 |
| S: Sysfs=/devices/platform/i8042/serio0/input/input1 |
| U: Uniq= |
| H: Handlers=sysrq kbd event1 leds |
| B: PROP=0 |
| B: EV=120013 |
| B: KEY=402000000 3803078f800d001 feffffdfffefffff fffffffffffffffe |
| B: MSC=10 |
| B: LED=7 |
* What I am interested is the handler to the keyboard.
* An event handler provides an interface to the user space by converting events into the necessary format.
	* handler: is a piece of code that manages program events. The handler catches the event and reacts to it.
	* Event: an action or occurrence that can be identified by a program and has significance for system hardware or software.
* I want to to captue the events to the keyboard handler inorder to see to the keypresses.
* To do that I need to access the event located in **/dev/input**.
* **/dev** directory contains all the device files for all the devices on the system while **/dev/input** is a subdirectory that holds the device files for various input devices.
* Data from **/dev/input/event#** in the form of the struct input_event. Information for this struct can be found in input.h.
* EV_KEY: Used to describe state changes of keyboards, buttons, or other key-like devices.
* Data from keyboards comes mainly in the form of scancodes. Scancode are in hexadecimal. Each key press/release will produce 0 to 6 scancodes. 
* Mapped the generic scancode to the corresponding key. Some scancodes are only available for differenct size/type/model of keyboards.


## Reference
* Andy Dalton. Is keyboard driver a character device driver? https://unix.stackexchange.com/questions/600461/is-keyboard-driver-a-character-device-driver
* dirkt. Is keyboard driver a character device driver? https://unix.stackexchange.com/questions/600461/is-keyboard-driver-a-character-device-driver
* Finding the real devices. https://tldp.org/HOWTO/XFree-Local-multi-user-HOWTO/tweak_input_devs-realdev.html
* Runium. Explain EV in /proc/bus/input/devices data. https://unix.stackexchange.com/questions/74903/explain-ev-in-proc-bus-input-devices-data
* nx5000. (linux) Handler. https://www.linuxquestions.org/questions/programming-9/linux-handler-381516/
* Sarah Lewis. What is Event? https://www.techtarget.com/searchapparchitecture/definition/event
* dirkt. https://unix.stackexchange.com/questions/545274/how-does-a-keyboard-press-get-processed-in-the-linux-kernel
* Linux USB input subsystem. https://sciencesoftcode.wordpress.com/2019/07/07/linux-usb-input-subsystem/
* Exploring /dev/input. https://thehackerdiary.wordpress.com/2017/04/21/exploring-devinput-1/
* input.h. https://github.com/torvalds/linux/blob/master/include/uapi/linux/input.h
* https://www.kernel.org/doc/Documentation/input/event-codes.txt
* Ricky Zhang. rickyzhang82. https://gist.github.com/rickyzhang82/8581a762c9f9fc6ddb8390872552c250
* Markus Kohlhase. https://github.com/flosse/linuxconsole/blob/master/utils/scancodes.h
* Keyboard Scancodes. https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html