/* 
 * File:   EEPROM_24C02C.h
 * Author: Alielden
 *
 * Created on February 22, 2024, 2:17 AM
 */

#ifndef EEPROM_24C02C_H
#define	EEPROM_24C02C_H

/* Section :  Includes */

#include "../../mcc_generated_files/mcc.h"


 /* Section : Macros */

 

/* Section : Macros Functions */


/* Section : Data type Declaration */




/* Section : Functions Declaration */

void EEPRM_24C02C_Write_data(uint8_t EEPROM_Address , uint8_t Data_Address , uint8_t Data);

void EEPRM_24C02C_Read_data(uint8_t EEPROM_Address , uint8_t Data_Address , uint8_t *Data);



#endif	/* EEPROM_24C02C_H */

