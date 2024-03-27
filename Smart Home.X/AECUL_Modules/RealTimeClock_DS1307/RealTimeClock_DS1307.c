/* 
 * File:   RealTimeClock_DS1307.c
 * Author: Alielden
 *
 * Created on February 21, 2024, 2:01 AM
 */

#include "RealTimeClock_DS1307.h"


static RealTimeClockDS1307_t Clock ;

/* [ Year : Month : Day  *  Hour : Minute : Second ]*/

static uint8_t RealTimeClockArr[20] ;


RealTimeClockDS1307_t RealTimeClockDS1307_Get_Date_Time(void)
{
    Clock.Seconds = I2C_Read1ByteRegister( SLAVE_ADDRESS , SECONDS_REGISTER_ADDRESS ) ;
    Clock.Minutes = I2C_Read1ByteRegister( SLAVE_ADDRESS , MINUTESS_REGISTER_ADDRESS ) ;
    Clock.Hours   = I2C_Read1ByteRegister( SLAVE_ADDRESS , HOURS_REGISTER_ADDRESS ) ;
    
    
    Clock.Day     = I2C_Read1ByteRegister( SLAVE_ADDRESS , DAYS_REGISTER_ADDRESS ) ;
    Clock.Month   = I2C_Read1ByteRegister( SLAVE_ADDRESS , MONTHS_REGISTER_ADDRESS ) ;
    Clock.Year    = I2C_Read1ByteRegister( SLAVE_ADDRESS , YEARS_REGISTER_ADDRESS ) ;
    
    return Clock ;
}

void Print_RealTimeClockDS1307_Pack(void)
{
    RealTimeClockArr[0] = (Clock.Year >> 4 ) + '0' ;    //  shifting 4 bit because data of clock is BCD  
    RealTimeClockArr[1] = (Clock.Year & 0x0F ) + '0' ;
    RealTimeClockArr[2] = ':' ;
    RealTimeClockArr[3] = (Clock.Month >> 4  ) + '0' ;
    RealTimeClockArr[4] = (Clock.Month & 0x0F ) + '0' ;
    RealTimeClockArr[5] = ':' ;
    RealTimeClockArr[6] = (Clock.Day >> 4  ) + '0' ;
    RealTimeClockArr[7] = (Clock.Day & 0x0F ) + '0' ;
    RealTimeClockArr[8] = ' ' ;
    RealTimeClockArr[9] = '*' ;
    RealTimeClockArr[10] = ' ' ;
    RealTimeClockArr[11] = (Clock.Hours >> 4  ) + '0' ;
    RealTimeClockArr[12] = (Clock.Hours & 0x0F ) + '0' ;
    RealTimeClockArr[13] = ':' ;
    RealTimeClockArr[14] = (Clock.Minutes >> 4  ) + '0' ;
    RealTimeClockArr[15] = (Clock.Minutes & 0x0F ) + '0' ;
    RealTimeClockArr[16] = ':' ;
    RealTimeClockArr[17] = (Clock.Seconds >> 4  ) + '0' ;
    RealTimeClockArr[18] = (Clock.Seconds & 0x0F ) + '0' ;
    RealTimeClockArr[19] = '\r' ;
         
    
    UART_Send_String("Date : \r" , 8) ;
    UART_Send_String("20" , 2) ;
    UART_Send_String(RealTimeClockArr , 20 ) ;
    
}


