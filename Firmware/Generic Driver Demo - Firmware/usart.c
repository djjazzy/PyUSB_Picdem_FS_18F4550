/*******************************************************************************************************************
*
*   Test Software for PIC18F4550
*
********************************************************************************************************************
* Filename: uart.c
*
* Dependencies: uart.h
* Compiler: C18
* Linker: 
*
* Company: Arthrex, Inc.
* 
* uart driver 
*
* Author | Rev | Date | Comments
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Edwin Floyd | Rev 1.01 | Date: | 
**********************************************************************************************************************/

/* INCLUDES ************************/
#include "usart.h"

/* PRIVATE PROTOTYPES **************/

/* INITIALIZATION *****************/
void USARTInit(){
	//PIR1 has RCIF and TXIF interrupt flags
	//IPR1 has RCIP and TXIP interrupt priorities
	//PIE has RCIE and TXIE interrupt enable toggles
	RCSTAbits.SPEN = _SET;
	TRISCbits.TRISC7 = _INPUT;
	TRISCbits.TRISC6 = _INPUT;
	TXSTAbits.TXEN = _SET;
	TXSTAbits.SYNC = _CLEAR;
	BAUDCONbits.BRG16 = _CLEAR;
	SPBRGH = _CLEAR;
	SPBRG = 12;	
}


                         

/*****************************************************************************
 * Function: UARTPutChar
 *
 * Precondition: UARTInit must be called before.
 *
 * Overview: Wait for free UART transmission buffer and send a byte.
 *
 * Input: Byte to be sent.
 *
 * Output: None.
 *
 *****************************************************************************/
void  UARTPutChar(int8_t Ch){
    // wait for empty buffer
    
    #if defined(__PIC24FJ256GB106__)
        while(U3STAbits.UTXBF);       //Wait for room in buffer
        //U1TXREG = Ch;
        //U2TXREG = Ch;
        U3TXREG = Ch;
    #else
        while(PIR1bits.TXIF == _CLEAR);
          TXREG = Ch;
    #endif
}

/*****************************************************************************
 * Function: UARTIsPressed
 *
 * Precondition: UARTInit must be called before.
 *
 * Overview: Check if there's a new byte in UART reception buffer.
 *
 * Input: None.
 *
 * Output: Zero if there's no new data received.
 *
 *****************************************************************************/
BOOL UARTIsPresent()
{
    #if defined(__PIC24FJ256GB106__)
        if(U3STAbits.URXDA == _SET)
            return _TRUE;
        else
            return _FALSE;
    #else
        if(PIR1bits.RCIF == _SET)
            return _TRUE;
        
        return _FALSE;
    #endif
}

/*****************************************************************************
 * Function: UARTGetChar
 *
 * Precondition: UARTInit must be called before.
 *
 * Overview: Wait for a byte.
 *
 * Input: None.
 *
 * Output: Byte received.
 *
 *****************************************************************************/
int8_t UARTGetChar(){
    
    int8_t Temp;
    
    #if defined(__PIC24FJ256GB106__)
        while(U3STAbits.URXDA == _CLEAR); //(IFS5bits.U3RXIF; == _CLEAR);// CHANGED WHILE TO IF IN ORDER TO GET SINGLE CLICK PRINT
        Temp = U3RXREG;
        U3STAbits.URXDA = _CLEAR; // IFS5bits.U3RXIF = _CLEAR; //U3STAbits.URXDA = _CLEAR;
        IFS5bits.U3RXIF = _CLEAR;
        U3STAbits.OERR      = 0;                   //Clear any error
        return Temp;
    #else
        while(PIR1bits.RCIF == 0);    
        Temp = RCREG;   
        PIR1bits.RCIF = _CLEAR;    
        return Temp;
    #endif
}

/*****************************************************************************
 * Function: UARTPutDec
 *
 * Precondition: UARTInit must be called before.
 *
 * Overview: This function converts decimal data into a string
 * and outputs it into UART.
 *
 * Input: Binary data.
 *
 * Output: None.
 *
 *****************************************************************************/
void  UARTPutDec(uint8_t Dec){
    
    uint8_t Res;
    Res = Dec;

    if(Res/100) 
        UARTPutChar(Res/100+'0');
    Res = Res - (Res/100)*100;

    if(Res/10) 
        UARTPutChar(Res/10+'0');
    Res = Res - (Res/10)*10;
 
    UARTPutChar(Res+'0');
}

/*****************************************************************************
 * Function: UARTPutStr
 *
 * Precondition: UARTInit must be called before.
 *
 * Overview: Send a string out the Tx port
 *
 * Input: Ptr to string to send
 *
 * Output: None,
 *
 *****************************************************************************/
void UARTPutStr(uint8_t *str){
    
    while(*str != 0){
        
        UARTPutChar(*str);
        ++str;
    }
}

/*****************************************************************************
 * Function: UARTGetStr
 *
 * Precondition: UARTInit must be called before.
 *
 * Overview: Read a sereis of Chars into a string buffer.
 *
 * Input: String Buffer Ptr, String Length
 *
 * Output: String
 *
 *****************************************************************************/
void UARTGetStr(uint8_t *str, uint16_t len){        //uint16_t
    uint16_t i = 0;
    uint8_t c = 0;
    
    while(i < len){// && c != 0x0D){
        
        c = UARTGetChar();
        
 //       if(c != 0x0A && c != 0x0D)
            str[i++] = c;
    }
    
    str[i] = 0;
    
    //if(c== 0x0D && serStrCnt > 0)
    //    --serStrCnt;
        
     //return i;
}

/**********************************************************************
* Function:        void hex_ascii(unsigned char display_data)
* PreCondition:    None
* Input:		   ascii Value 
* Output:		   None	 
* Side Effects:
* Overview:		 Convert ascii value to hex and send to PC;
***********************************************************************/
void hex_ascii(uint8_t data)            // Example data = d45 = 0b0010 1101 ... hibyte 0010 = 2 ... lobyte 1101 = 13 or 'D' (BCD hex) so should return 2D
{
	uint8_t temp;
	
	// High Nibble
	temp = ((data & 0xF0)>>4);          // 0b0010 1101  & 0b1111 0000 = 0b0010 0000 ... >> 4 = 0b0000 0010 = dec2
	
	if (temp <= 0x09)                   // dec2 < dec9? True
		UARTPutChar(temp+'0');	        // put dec2 + '0' ... ['0' = dec48] == put dec50 == 0x32 == '2' so putting character 2 from decimal 2
	else
		UARTPutChar(temp+'0'+0x07);
		//UARTPutChar(temp-10+'A');	

    // Low Nibble
	temp = data & 0x0F;                 // 0b0010 1101 & 0000 1111 = 0b0000 1101 = dec13
	
	if (temp <= 0x09)                   // dec13 < dec9? False
		UARTPutChar(temp+'0');	
	else
		UARTPutChar(temp+'0'+0x07);	    //put dec13 + dec48 (='0') + dec7 = put dec68 == 0x44 == 'D' so putting D from decimal 13 
	    //UARTPutChar(temp-10+'A');
	    
	//UARTPutChar(' ');                   // so result is 2D for the BCD (hex) value = 2D
	//UARTPutChar('\r');
	//UARTPutChar('\n');
}

/*****************************************************************************
 * EOF
 *****************************************************************************/
 

