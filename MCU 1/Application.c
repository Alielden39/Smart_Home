/* 
 * File:   Application.c
 * Author: Alielden
 *
 * Created on September 9, 2023, 6:23 PM
 */

#include "Application.h"

void ADC_CallBack(void);
void RB4_APP_ISR_High(void);
void RB5_APP_ISR_High(void);
void RB6_APP_ISR_High(void);

Std_ReturnType ret = 0 ;
volatile uint8 interrupt_PIR = PIR_ON ,flag = 0  ;
logic_t IR_read = 0 ;
uint16 Pressure_sen_val = 0 , Temp_sen_val = 0 , Gas_sen_val = 0    ;
float pressure = 0 , Celsuis = 0 ; 

usart_t Gas ={
  .baudrate = 9600 ,
  .baudrate_eqn = ASYN_8BIT_LOW_SPEED ,
  
  .tx_cfg.tx_enable = USART_TX_ENABLE ,
  .tx_cfg.tx_interrupt = EUSART_TX_INTERRUPT_ENABLE ,
  .tx_cfg.tx_9bit = USART_TX_9BIT_DATA_DISABLE ,
  
  .rx_cfg.rx_enable = USART_RX_ENABLE ,
  .rx_cfg.rx_interrupt = EUSART_RX_INTERRUPT_ENABLE ,
  .rx_cfg.rx_9bit = USART_RX_9BIT_DATA_DISABLE ,
  
  .USART_TX_CALLBACK = NULL ,
  .USART_RX_CALLBACK = NULL ,
  .USART_RX_FRAMING_ERROR_CALLBACK = NULL ,
  .USART_RX_OVERRUN_ERROR_CALLBACK = NULL ,
  
};

led_t Emergency =
{
  .port =  GPIO_PORTD ,
  .pin =   GPIO_PIN0 ,
  .led_status = led_OFF
};

led_t yellow_led =
{
  .port =  GPIO_PORTD ,
  .pin =   GPIO_PIN1 ,
  .led_status = led_OFF
};

led_t Room_leds =
{
  .port =  GPIO_PORTD ,
  .pin =   GPIO_PIN2 ,
  .led_status = led_OFF
};


pin_config_t IR =
{
  .direction = GPIO_INPUT ,
  .port = GPIO_PORTD ,
  .pin = GPIO_PIN3 ,
  .logic = GPIO_LOW  
};


mssp_i2c_t i2c_obj =
{
   .mode = I2C_SLAVE_MODE ,
   .sub_mode = I2C_SLAVE_MODE_7BIT_ADDRESS ,
   .i2c_clk = 100000 ,
   .SMBus = I2C_SMBUS_DISABLE ,
   .slew_rate = I2C_SLEW_RATE_DISABLE ,
   .general_call = I2C_SLAVE_MODE_GENERAL_CALL_DISABLE ,
   .slave_address = 0x20         
};

adc_t Pressure_Sensor =
{
  .adc_call_back = NULL ,
  .Format_Select = RIGHT_FORMAT ,
  .Taq = ADC_12_TAD ,
  .Voltage_Refrence = VSS_VDD ,
  .channel = ADC_CHANNEL_AN0 ,
  .clk = ADC_FOSC_DIVIDE_16
};

adc_t Temp_Sensor =
{
  .adc_call_back = NULL ,
  .Format_Select = RIGHT_FORMAT ,
  .Taq = ADC_12_TAD ,
  .Voltage_Refrence = VSS_VDD ,
  .channel = ADC_CHANNEL_AN1 ,
  .clk = ADC_FOSC_DIVIDE_16
};

adc_t Gas_Sensor =
{
  .adc_call_back = NULL ,
  .Format_Select = RIGHT_FORMAT ,
  .Taq = ADC_12_TAD ,
  .Voltage_Refrence = VSS_VDD ,
  .channel = ADC_CHANNEL_AN2 ,
  .clk = ADC_FOSC_DIVIDE_16
};

interrupt_RB_t int0 ={
  .interrupt_call_back_High = RB4_APP_ISR_High ,
  .interrupt_call_back_Low  = NULL ,
  .priority            = HIGH_PRIORITY  ,
  .pin.port            = GPIO_PORTB ,
  .pin.pin             = GPIO_PIN4 ,
  .pin.direction       = GPIO_INPUT
};

interrupt_RB_t int1 ={
  .interrupt_call_back_High = RB5_APP_ISR_High ,
  .interrupt_call_back_Low  = NULL ,
  .priority            = HIGH_PRIORITY  ,
  .pin.port            = GPIO_PORTB ,
  .pin.pin             = GPIO_PIN5 ,
  .pin.direction       = GPIO_INPUT
};

interrupt_RB_t Room_btn ={
  .interrupt_call_back_High = RB6_APP_ISR_High ,
  .interrupt_call_back_Low  = NULL ,
  .priority            = HIGH_PRIORITY  ,
  .pin.port            = GPIO_PORTB ,
  .pin.pin             = GPIO_PIN6 ,
  .pin.direction       = GPIO_INPUT
};


int main() {
    
    APPLICATION_INTIALIZE();
    while(1)
    {
        ADC_GetConversion(&Pressure_Sensor , ADC_CHANNEL_AN0 , &Pressure_sen_val ) ;
        pressure = (Pressure_sen_val * 5.0) / 1023; // Calculate pressure
        pressure = (pressure + 0.475) / 0.0475; // Adjust pressure value
////////////////////////////////////////////////////////////////////////////////////////////////// 
        ADC_GetConversion(&Temp_Sensor , ADC_CHANNEL_AN1 , &Temp_sen_val ) ;
        Celsuis = Temp_sen_val * 4.88f ;
        Celsuis /= 10 ;
        
        if( Celsuis > 50 )
        {
            if( pressure < 101 )
            {
                yellow_led_turn_on() ;
                Emergency_turn_off() ;
            }
            else if( pressure > 101 )
            {
                yellow_led_turn_off() ;
                Emergency_turn_on() ;
            }
        }
        else if( Celsuis < 50 )
        {
            yellow_led_turn_off() ;
            Emergency_turn_off() ;
        }
////////////////////////////////////////////////////////////////////////////////////////////////
       ADC_GetConversion(&Gas_Sensor , ADC_CHANNEL_AN2 , &Gas_sen_val ) ;
       ret = Usart_Async_WriteByte(Gas_sen_val);
       ret = Usart_Async_WriteByte(Gas_sen_val>>8);
       __delay_ms(100);
////////////////////////////////////////////////////////////////////////////////////////////////
        if( interrupt_PIR == PIR_ON)
        {
            ret = gpio_pin_read_logic(&IR , &IR_read );
            switch( IR_read )
            {
                case FULL :
                    Room_leds_turn_on();
                    __delay_ms(75) ;
                    break;
                
                case EMPTY :
                    Room_leds_turn_off();
                    break;
            }
            
        }
        else if( interrupt_PIR == PIR_OFF )
        {
            Room_leds_turn_off(); 
        } 
///////////////////////////////////////////////////////////////////////////////////////////////////        
        if( flag == 1 )
        {
            Room_leds_turn_on() ;
            __delay_ms(50) ;
        }
        else if( flag == 2 )
        {
            Room_leds_turn_off();
            flag = 0 ;
        }

    }


    return (EXIT_SUCCESS);
}


void APPLICATION_INTIALIZE(void)
{
    ret = MSSP_I2C_init(&i2c_obj) ;
    ret = Usart_Async_init(&Gas);
    ret = ADC_Init(&Pressure_Sensor) ;
    ret = ADC_Init(&Temp_Sensor) ;
    ret = ADC_Init(&Gas_Sensor) ;
    ret = led_intialize(&yellow_led);
    ret = led_intialize(&Emergency);
    ret = led_intialize(&Room_leds);
    ret = Interrupt_RB_Intializing(&int0);
    ret = Interrupt_RB_Intializing(&int1);
    ret = Interrupt_RB_Intializing(&Room_btn);
}

void yellow_led_turn_on(void)
{
    ret = led_turn_on(&yellow_led);
}

void yellow_led_turn_off(void)
{
    ret = led_turn_off(&yellow_led);
}

void Room_leds_turn_on(void)
{
    ret = led_turn_on(&Room_leds);
}

void Room_leds_turn_off(void)
{
    ret = led_turn_off(&Room_leds);
}

void Emergency_turn_on(void)
{
    ret = led_turn_on(&Emergency);
}

void Emergency_turn_off(void)
{
    ret = led_turn_off(&Emergency);
}

void RB4_APP_ISR_High(void)
{
    interrupt_PIR = PIR_OFF ;
}

void RB5_APP_ISR_High(void)
{
    interrupt_PIR = PIR_ON ;
}

void RB6_APP_ISR_High(void)
{
    if( flag == 0 )
    {
        flag = 1 ;
    }
    else if( flag == 1 )
    {
        flag = 2 ;
    }
}






