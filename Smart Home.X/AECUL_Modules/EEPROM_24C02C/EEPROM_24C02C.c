/* 
 * File:   EEPROM_24C02C.C
 * Author: Alielden
 *
 * Created on February 22, 2024, 2:17 AM
 */

#include "EEPROM_24C02C.h"


void EEPRM_24C02C_Write_data(uint8_t EEPROM_Address , uint8_t Data_Address , uint8_t Data)
{
    I2C_Write1ByteRegister( EEPROM_Address , Data_Address , Data ) ;
}

void EEPRM_24C02C_Read_data(uint8_t EEPROM_Address , uint8_t Data_Address , uint8_t *Data)
{
    *Data = I2C_Read1ByteRegister( EEPROM_Address , Data_Address ) ;
}