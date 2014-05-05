/********************************************************************
 FileName:     	HardwareProfile.h
 Dependencies:	See INCLUDES section
 Processor:		PIC18 or PIC24 USB Microcontrollers
 Hardware:		The code is natively intended to be used on the following
 				hardware platforms: PICDEM™ FS USB Demo Board, 
 				PIC18F87J50 FS USB Plug-In Module, or
 				Explorer 16 + PIC24 USB PIM.  The firmware may be
 				modified for use on other USB platforms by editing this
 				file (HardwareProfile.h).
 Complier:  	Microchip C18 (for PIC18) or C30 (for PIC24)
 Company:		Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the “Company”) for its PIC® Microcontroller is intended and
 supplied to you, the Company’s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************
 File Description:

 Change History:
  Rev   Date         Description
  1.0   11/19/2004   Initial release
  2.1   02/26/2007   Updated for simplicity and to use common
                     coding style

********************************************************************/

#ifndef HARDWARE_PROFILE_H
#define HARDWARE_PROFILE_H

//#define DEMO_BOARD USER_DEFINED_BOARD

#if !defined(DEMO_BOARD)
    #if defined(__C30__)
        #if defined(__PIC24FJ256GB110__)
            #define DEMO_BOARD PIC24FJ256GB110_PIM
            #define EXPLORER_16
			#define PIC24FJ256GB110_PIM
            #define CLOCK_FREQ 32000000
        #endif
    #endif

    #if defined(__18CXX)
        #if defined(__18F4550)
            #define DEMO_BOARD PICDEM_FS_USB
            #define PICDEM_FS_USB
            #define CLOCK_FREQ 48000000
        #elif defined(__18F87J50)
            #define DEMO_BOARD PIC18F87J50_PIM
            #define PIC18F87J50_PIM
            #define CLOCK_FREQ 48000000
        #elif defined(__18F14K50)
            #define DEMO_BOARD PIC18F14K50_DEMO_BOARD
            #define PIC18F14K50_DEMO_BOARD
            #define CLOCK_FREQ 48000000
        #endif
    #endif
#endif

#if !defined(DEMO_BOARD)
    #error "Demo board not defined.  Either define DEMO_BOARD for a custom board or select the correct processor for the demo board."
#endif

//#define ENABLE_CONSOLE

/** TRIS ***********************************************************/
#define INPUT_PIN           1
#define OUTPUT_PIN          0

/** USB ************************************************************/
#if defined(PICDEM_FS_USB)
	//The PICDEM FS USB Demo Board platform supports the USE_SELF_POWER_SENSE_IO
	//and USE_USB_BUS_SENSE_IO features.  Uncomment the below line(s) if
	//it is desireable to use one or both of the features.
	//#define USE_SELF_POWER_SENSE_IO	
	//#define USE_USB_BUS_SENSE_IO

	//#define PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER	//Uncomment this to make the output HEX of this project work with the MCHPUSB Bootloader
	#define PROGRAMMABLE_WITH_USB_HID_BOOTLOADER		//Uncomment this to make the output HEX of this project work with the HID Bootloader
   
    #define tris_usb_bus_sense  TRISAbits.TRISA1    // Input
    
    #if defined(USE_USB_BUS_SENSE_IO)
    #define USB_BUS_SENSE       PORTAbits.RA1
    #else
    #define USB_BUS_SENSE       1
    #endif
    
    #define tris_self_power     TRISAbits.TRISA2    // Input
    
    #if defined(USE_SELF_POWER_SENSE_IO)
    #define self_power          PORTAbits.RA2
    #else
    #define self_power          1
    #endif
    
    // External Transceiver Interface
    #define tris_usb_vpo        TRISBbits.TRISB3    // Output
    #define tris_usb_vmo        TRISBbits.TRISB2    // Output
    #define tris_usb_rcv        TRISAbits.TRISA4    // Input
    #define tris_usb_vp         TRISCbits.TRISC5    // Input
    #define tris_usb_vm         TRISCbits.TRISC4    // Input
    #define tris_usb_oe         TRISCbits.TRISC1    // Output
    
    #define tris_usb_suspnd     TRISAbits.TRISA3    // Output
    
    /** LED ************************************************************/
    #define mInitAllLEDs()      LATD &= 0xF0; TRISD &= 0xF0;
    
    #define mLED_1              LATDbits.LATD0
    #define mLED_2              LATDbits.LATD1
    #define mLED_3              LATDbits.LATD2
    #define mLED_4              LATDbits.LATD3
    
    #define mLED_1_On()         mLED_1 = 1;
    #define mLED_2_On()         mLED_2 = 1;
    #define mLED_3_On()         mLED_3 = 1;
    #define mLED_4_On()         mLED_4 = 1;
    
    #define mLED_1_Off()        mLED_1 = 0;
    #define mLED_2_Off()        mLED_2 = 0;
    #define mLED_3_Off()        mLED_3 = 0;
    #define mLED_4_Off()        mLED_4 = 0;
    
    #define mLED_1_Toggle()     mLED_1 = !mLED_1;
    #define mLED_2_Toggle()     mLED_2 = !mLED_2;
    #define mLED_3_Toggle()     mLED_3 = !mLED_3;
    #define mLED_4_Toggle()     mLED_4 = !mLED_4;
    
    /** SWITCH *********************************************************/
    #define mInitAllSwitches()  TRISBbits.TRISB4=1;TRISBbits.TRISB5=1;
    #define mInitSwitch2()      TRISBbits.TRISB4=1;
    #define mInitSwitch3()      TRISBbits.TRISB5=1;
    #define sw2                 PORTBbits.RB4
    #define sw3                 PORTBbits.RB5
    
    /** POT ************************************************************/
    #define mInitPOT()          {TRISAbits.TRISA0=1;ADCON0=0x01;ADCON2=0x3C;ADCON2bits.ADFM = 1;}
    
    /** SPI : Chip Select Lines ****************************************/
    #define tris_cs_temp_sensor TRISBbits.TRISB2    // Output
    #define cs_temp_sensor      LATBbits.LATB2
    
    #define tris_cs_sdmmc       TRISBbits.TRISB3    // Output
    #define cs_sdmmc            LATBbits.LATB3
    
    /** SD/MMC *********************************************************/
    #define TRIS_CARD_DETECT    TRISBbits.TRISB4    // Input
    #define CARD_DETECT         PORTBbits.RB4
    
    #define TRIS_WRITE_DETECT   TRISAbits.TRISA4    // Input
    #define WRITE_DETECT        PORTAbits.RA4

#endif

#if defined(PIC18F87J50_PIM)
//The PIC18F87J50 FS USB Plug-In Module supports the USE_USB_BUS_SENSE_IO
//feature.  The USE_SELF_POWER_SENSE_IO feature is not implemented on the
//circuit board, so the USE_SELF_POWER_SENSE_IO define should always be
//commented for this hardware platform.

//#define USE_SELF_POWER_SENSE_IO
//#define USE_USB_BUS_SENSE_IO		//JP1 must be in R-U position to use this feature on this board	

	#define PROGRAMMABLE_WITH_USB_HID_BOOTLOADER		//Uncomment this to make the output HEX of this project work with the HID Bootloader
    
    #define tris_usb_bus_sense  TRISBbits.TRISB5    // Input
    
    #if defined(USE_USB_BUS_SENSE_IO)
    #define USB_BUS_SENSE       PORTBbits.RB5
    #else
    #define USB_BUS_SENSE       1
    #endif
    
    #define tris_self_power     TRISAbits.TRISA2    // Input
    
    #if defined(USE_SELF_POWER_SENSE_IO)
    #define self_power          PORTAbits.RA2
    #else
    #define self_power          1
    #endif
    
    // External Transceiver Interface
    #define tris_usb_vpo        TRISBbits.TRISB3    // Output
    #define tris_usb_vmo        TRISBbits.TRISB2    // Output
    #define tris_usb_rcv        TRISAbits.TRISA4    // Input
    #define tris_usb_vp         TRISCbits.TRISC5    // Input
    #define tris_usb_vm         TRISCbits.TRISC4    // Input
    #define tris_usb_oe         TRISCbits.TRISC1    // Output
    
    #define tris_usb_suspnd     TRISAbits.TRISA3    // Output
    
    /** LED ************************************************************/
	#define mInitAllLEDs()      {LATE &= 0xFC; TRISE &= 0xFC; LATD &= 0xF3; TRISD &= 0xF3;}
    
    #define mLED_1              LATEbits.LATE0
    #define mLED_2              LATEbits.LATE1
    #define mLED_3              LATDbits.LATD2
    #define mLED_4              LATDbits.LATD3
    
    #define mLED_1_On()         mLED_1 = 1;
    #define mLED_2_On()         mLED_2 = 1;
    #define mLED_3_On()         mLED_3 = 1;
    #define mLED_4_On()         mLED_4 = 1;
    
    #define mLED_1_Off()        mLED_1 = 0;
    #define mLED_2_Off()        mLED_2 = 0;
    #define mLED_3_Off()        mLED_3 = 0;
    #define mLED_4_Off()        mLED_4 = 0;
    
    #define mLED_1_Toggle()     mLED_1 = !mLED_1;
    #define mLED_2_Toggle()     mLED_2 = !mLED_2;
    #define mLED_3_Toggle()     mLED_3 = !mLED_3;
    #define mLED_4_Toggle()     mLED_4 = !mLED_4;
    
    /** SWITCH *********************************************************/
    #define mInitAllSwitches()  TRISBbits.TRISB4=1;
    #define mInitSwitch2()      TRISBbits.TRISB4=1;
    #define mInitSwitch3()      TRISBbits.TRISB4=1;
    #define sw2                 PORTBbits.RB4
    #define sw3                 PORTBbits.RB4
    
    /** POT ************************************************************/
    #define mInitPOT()          {TRISAbits.TRISA0=1;			\
    							 WDTCONbits.ADSHR = 1;			\
    							 ANCON0bits.PCFG0 = 1;			\
    							 WDTCONbits.ADSHR = 0;			\
    							 ADCON0=0x01;					\
    							 ADCON1=0xBE;}		// POT on HPC Explorer				
    //#define mInitPOT()          ADCON0=0x01;ADCON1=0x06;WDTCONbits.ADSHR=1;ANCON0=0x9E;ANCON1=0xFE;WDTCONbits.ADSHR=0;
    
    /** I 2 C   T E M P   S E N S E *************************************/
    #define	mInitI2CPins()		TRISC |= 0x18;		// RC3 and RC4 are I2C

    
    
    /** SPI : Chip Select Lines ****************************************/
    #define tris_cs_temp_sensor TRISBbits.TRISB2    // Output
    #define cs_temp_sensor      LATBbits.LATB2
    
    #define tris_cs_sdmmc       TRISBbits.TRISB3    // Output
    #define cs_sdmmc            LATBbits.LATB3
    
    /** SD/MMC *********************************************************/
    #define TRIS_CARD_DETECT    TRISBbits.TRISB4    // Input
    #define CARD_DETECT         PORTBbits.RB4
    
    #define TRIS_WRITE_DETECT   TRISAbits.TRISA4    // Input
    #define WRITE_DETECT        PORTAbits.RA4

#endif

#if defined(PIC18F14K50_DEMO_BOARD)
	//The PICDEM FS USB Demo Board platform supports the USE_SELF_POWER_SENSE_IO
	//and USE_USB_BUS_SENSE_IO features.  Uncomment the below line(s) if
	//it is desireable to use one or both of the features.
	//#define USE_SELF_POWER_SENSE_IO	
	//#define USE_USB_BUS_SENSE_IO

	#define PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER	//Uncomment this to make the output HEX of this project work with the MCHPUSB Bootloader
   
    #define tris_usb_bus_sense  TRISAbits.TRISA1    // Input
    
    #if defined(USE_USB_BUS_SENSE_IO)
    #define USB_BUS_SENSE       PORTAbits.RA1
    #else
    #define USB_BUS_SENSE       1
    #endif
    
    #define tris_self_power     TRISAbits.TRISA2    // Input
    
    #if defined(USE_SELF_POWER_SENSE_IO)
    #define self_power          PORTAbits.RA2
    #else
    #define self_power          1
    #endif
    
    // External Transceiver Interface
    #define tris_usb_vpo        TRISBbits.TRISB3    // Output
    #define tris_usb_vmo        TRISBbits.TRISB2    // Output
    #define tris_usb_rcv        TRISAbits.TRISA4    // Input
    #define tris_usb_vp         TRISCbits.TRISC5    // Input
    #define tris_usb_vm         TRISCbits.TRISC4    // Input
    #define tris_usb_oe         TRISCbits.TRISC1    // Output
    
    #define tris_usb_suspnd     TRISAbits.TRISA3    // Output
    
    /** LED ************************************************************/
    #define mInitAllLEDs()      LATC &= 0xF0; TRISC &= 0xF0;
    
    #define mLED_1              LATCbits.LATC0
    #define mLED_2              LATCbits.LATC1
    #define mLED_3              LATCbits.LATC2
    #define mLED_4              LATCbits.LATC3
    
    #define mLED_1_On()         mLED_1 = 1;
    #define mLED_2_On()         mLED_2 = 1;
    #define mLED_3_On()         mLED_3 = 1;
    #define mLED_4_On()         mLED_4 = 1;
    
    #define mLED_1_Off()        mLED_1 = 0;
    #define mLED_2_Off()        mLED_2 = 0;
    #define mLED_3_Off()        mLED_3 = 0;
    #define mLED_4_Off()        mLED_4 = 0;
    
    #define mLED_1_Toggle()     mLED_1 = !mLED_1;
    #define mLED_2_Toggle()     mLED_2 = !mLED_2;
    #define mLED_3_Toggle()     mLED_3 = !mLED_3;
    #define mLED_4_Toggle()     mLED_4 = !mLED_4;
    
    /** SWITCH *********************************************************/
    #define mInitSwitch2()      //TRISAbits.TRISA3=1
        //only one switch available so double duty
    #define mInitSwitch3()      //TRISAbits.TRISA3=1
    #define sw2                 PORTAbits.RA3
    #define sw3                 PORTAbits.RA3
    #define mInitAllSwitches()  mInitSwitch2();
    
    /** POT ************************************************************/
    #define mInitPOT()          {TRISBbits.TRISB4=1;ADCON0=0x29;ADCON1=0;ADCON2=0x3E;ADCON2bits.ADFM = 1;}

#endif

#if defined(PIC24FJ256GB110_PIM)
	//#define USE_SELF_POWER_SENSE_IO
	//#define USE_USB_BUS_SENSE_IO

    #define tris_usb_bus_sense  TRISBbits.TRISB5    // Input

    #define USB_BUS_SENSE       1
   
    #define tris_self_power     TRISAbits.TRISA2    // Input

    #define self_power          1

    
    
    /** LED ************************************************************/
    #define mInitAllLEDs()      LATA &= 0xF0; TRISA &= 0xF0;
    
    #define mLED_1              LATAbits.LATA0
    #define mLED_2              LATAbits.LATA1
    #define mLED_3              LATAbits.LATA2
    #define mLED_4              LATAbits.LATA3
    
    #define mLED_1_On()         mLED_1 = 1;
    #define mLED_2_On()         mLED_2 = 1;
    #define mLED_3_On()         mLED_3 = 1;
    #define mLED_4_On()         mLED_4 = 1;
    
    #define mLED_1_Off()        mLED_1 = 0;
    #define mLED_2_Off()        mLED_2 = 0;
    #define mLED_3_Off()        mLED_3 = 0;
    #define mLED_4_Off()        mLED_4 = 0;
    
    #define mLED_1_Toggle()     mLED_1 = !mLED_1;
    #define mLED_2_Toggle()     mLED_2 = !mLED_2;
    #define mLED_3_Toggle()     mLED_3 = !mLED_3;
    #define mLED_4_Toggle()     mLED_4 = !mLED_4;
    
    /** SWITCH *********************************************************/
    #define mInitSwitch2()      TRISDbits.TRISD6=1;
    #define mInitSwitch3()      TRISDbits.TRISD7=1;
    #define mInitAllSwitches()  mInitSwitch2();mInitSwitch3();
    #define sw2                 PORTDbits.RD6
    #define sw3                 PORTDbits.RD7
    
    /** POT ************************************************************/
    #define mInitPOT()  {AD1PCFGLbits.PCFG5 = 0;    AD1CON2bits.VCFG = 0x0;    AD1CON3bits.ADCS = 0xFF;    AD1CON1bits.SSRC = 0x0;    AD1CON3bits.SAMC = 0b10000;    AD1CON1bits.FORM = 0b00;    AD1CON2bits.SMPI = 0x0;    AD1CON1bits.ADON = 1;}
    
    /** SPI : Chip Select Lines ****************************************/
    #define tris_cs_temp_sensor TRISBbits.TRISB2    // Output
    #define cs_temp_sensor      LATBbits.LATB2
    
    #define tris_cs_sdmmc       TRISBbits.TRISB3    // Output
    #define cs_sdmmc            LATBbits.LATB3
    
    /** SD/MMC *********************************************************/
    #define TRIS_CARD_DETECT    TRISBbits.TRISB4    // Input
    #define CARD_DETECT         PORTBbits.RB4
    
    #define TRIS_WRITE_DETECT   TRISAbits.TRISA4    // Input
    #define WRITE_DETECT        PORTAbits.RA4

#endif

#endif  //HARDWARE_PROFILE_H
