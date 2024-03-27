/* 
 * File:   UART_LoggingDebudData.c
 * Author: Alielden
 *
 * Created on February 22, 2024, 12:49 AM
 */


#include "UART_LoggingDebudData.h"


void UART_Send_String(uint8_t *str , uint16_t length)
{
    uint16_t counter = 0 ;
    
    for( counter = 0 ; counter < length ; counter++ )
    {
       EUSART_Write(*str++) ; 
    }
 
}
