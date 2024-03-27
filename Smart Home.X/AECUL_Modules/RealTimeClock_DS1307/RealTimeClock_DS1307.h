/* 
 * File:   RealTimeClock_DS1307.h
 * Author: Alielden
 *
 * Created on February 21, 2024, 2:01 AM
 */

#ifndef REALTIMECLOCK_DS1307_H
#define	REALTIMECLOCK_DS1307_H


/* Section :  Includes */

#include "../../mcc_generated_files/examples/i2c_master_example.h"
#include "../../mcc_generated_files/mcc.h"
#include "../UART_LoggingDebudData/UART_LoggingDebudData.h"



 /* Section : Macros */

#define SLAVE_ADDRESS   0xD0            

#define SECONDS_REGISTER_ADDRESS    0x00 
#define MINUTESS_REGISTER_ADDRESS   0x01 
#define HOURS_REGISTER_ADDRESS      0x02 

#define DAYS_REGISTER_ADDRESS       0x04 
#define MONTHS_REGISTER_ADDRESS     0x05 
#define YEARS_REGISTER_ADDRESS      0x06 

/* Section : Macros Functions */


/* Section : Data type Declaration */
typedef struct 
{
    uint8_t Hours ;
    uint8_t Minutes ;
    uint8_t Seconds ;
    
    uint8_t Year ;
    uint8_t  Month ;
    uint8_t  Day ;
    
}RealTimeClockDS1307_t;



/* Section : Functions Declaration */

RealTimeClockDS1307_t RealTimeClockDS1307_Get_Date_Time(void);
void Print_RealTimeClockDS1307_Pack(void);

#endif	/* REALTIMECLOCK_DS1307_H */

