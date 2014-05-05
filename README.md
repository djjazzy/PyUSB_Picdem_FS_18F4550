PyUSB_Picdem_FS_18F4550
=======================

Develop a Python USB host equivalent of the MSVC++ Microchip USB host Windows program using libusb 1.0 API that will communicate with the Microchip PICDEM FS USB development board.

Windows Environment:
Windows 7
LibUSB 1.0

Embedded Environment:
PIC18F4550
PICDEM FS UBB Development Kit
Microchip C18 C Compiler

HOST Program TODO:
Add parametric reporting of the firmware version on the py gui status bar
Correctly implement enumeration of multiple microchip embedded usb devices
Streamline code for efficiency

Deviations on stock PICDEM FS USB firmware:
Added code to supoort USART parameters (57600:8:N:1) and printf functionality.
