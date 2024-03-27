/* 
 * File:   TC74.c
 * Author: Alielden
 *
 * Created on February 24, 2024, 9:54 PM
 */

#include "TC74.h"

void TC74_Read_Data(uint8_t Slave_Address , uint8_t *data)
{
    *data = I2C_Read1ByteRegister(Slave_Address , 0x00 ) ;
}


