/*
*
*
*
*/

#ifndef USART_H
#define USART_H

#include "Compiler.h"
#include "GenericTypeDefs.h"
/* PUBLIC PROTOTYPES ******************/
void USARTInit(void);
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
extern void  UARTPutChar(int8_t Ch);

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
extern BOOL UARTIsPresent();

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
extern int8_t UARTGetChar();

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
extern void  UARTPutDec(uint8_t Dec);

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
extern void UARTPutStr(uint8_t *);

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
extern void UARTGetStr(uint8_t *, uint16_t);

/*****************************************************************************
 * Function: getHex
 *
 * Precondition: UARTInit must be called before.
 *
 * Overview: Take ascii data and put it to hex
 *
 * Input: Pointer to ascii data array to be converted
 * Input: Length of the ascii data array to be converted
 * Output: Hex value
 *
 *****************************************************************************/
extern void getHex(uint8_t *, uint8_t);

/*****************************************************************************
 * Function: hex_ascii
 *
 * Precondition: UARTInit must be called before.
 *
 * Overview: Take ascii data and put it to hex
 *
 * Input: ascii to be converted
 *
 * Output: Hex value
 *
 *****************************************************************************/
extern void hex_ascii(uint8_t);

#endif
