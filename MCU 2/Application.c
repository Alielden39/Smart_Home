/* 
 * File:   Application.c
 * Author: Alielden
 *
 * Created on September 9, 2023, 6:23 PM
 */

#include "Application.h"

void RX_Callback(void);


usart_t Gas_Sensor ={
  .baudrate = 9600 ,
  .baudrate_eqn = ASYN_8BIT_LOW_SPEED ,
  
  .tx_cfg.tx_enable = USART_TX_ENABLE ,
  .tx_cfg.tx_interrupt = EUSART_TX_INTERRUPT_DISABLE ,
  .tx_cfg.tx_9bit = USART_TX_9BIT_DATA_DISABLE ,
  
  .rx_cfg.rx_enable = USART_RX_ENABLE ,
  .rx_cfg.rx_interrupt = EUSART_RX_INTERRUPT_ENABLE ,
  .rx_cfg.rx_9bit = USART_RX_9BIT_DATA_DISABLE ,
  
  .USART_TX_CALLBACK = NULL ,
  .USART_RX_CALLBACK = RX_Callback ,
  .USART_RX_FRAMING_ERROR_CALLBACK = NULL ,
  .USART_RX_OVERRUN_ERROR_CALLBACK = NULL ,
  
};

segment_config_t seg1 =
{
  .segment_type = SEGMENT_COMMON_CATHODE ,
  .segment_pins[0].port = GPIO_PORTC ,
  .segment_pins[0].pin = GPIO_PIN0 ,
  .segment_pins[0].direction =GPIO_OUTPUT ,
  .segment_pins[0].logic = GPIO_LOW ,
          
  .segment_pins[1].port = GPIO_PORTC ,
  .segment_pins[1].pin = GPIO_PIN1 ,
  .segment_pins[1].direction =GPIO_OUTPUT ,
  .segment_pins[1].logic = GPIO_LOW ,
          
  .segment_pins[2].port = GPIO_PORTC ,
  .segment_pins[2].pin = GPIO_PIN2 ,
  .segment_pins[2].direction =GPIO_OUTPUT ,
  .segment_pins[2].logic = GPIO_LOW ,
          
  .segment_pins[3].port = GPIO_PORTC ,
  .segment_pins[3].pin = GPIO_PIN3 ,
  .segment_pins[3].direction =GPIO_OUTPUT ,
  .segment_pins[3].logic = GPIO_LOW ,
};

segment_config_t seg2 =
{
  .segment_type = SEGMENT_COMMON_CATHODE ,
  .segment_pins[0].port = GPIO_PORTD ,
  .segment_pins[0].pin = GPIO_PIN0 ,
  .segment_pins[0].direction =GPIO_OUTPUT ,
  .segment_pins[0].logic = GPIO_LOW ,
          
  .segment_pins[1].port = GPIO_PORTD ,
  .segment_pins[1].pin = GPIO_PIN1 ,
  .segment_pins[1].direction =GPIO_OUTPUT ,
  .segment_pins[1].logic = GPIO_LOW ,
          
  .segment_pins[2].port = GPIO_PORTD ,
  .segment_pins[2].pin = GPIO_PIN2 ,
  .segment_pins[2].direction =GPIO_OUTPUT ,
  .segment_pins[2].logic = GPIO_LOW ,
          
  .segment_pins[3].port = GPIO_PORTD ,
  .segment_pins[3].pin = GPIO_PIN3 ,
  .segment_pins[3].direction =GPIO_OUTPUT ,
  .segment_pins[3].logic = GPIO_LOW ,
};

segment_config_t seg3 =
{
  .segment_type = SEGMENT_COMMON_CATHODE ,
  .segment_pins[0].port = GPIO_PORTA ,
  .segment_pins[0].pin = GPIO_PIN0 ,
  .segment_pins[0].direction =GPIO_OUTPUT ,
  .segment_pins[0].logic = GPIO_LOW ,
          
  .segment_pins[1].port = GPIO_PORTA ,
  .segment_pins[1].pin = GPIO_PIN1 ,
  .segment_pins[1].direction =GPIO_OUTPUT ,
  .segment_pins[1].logic = GPIO_LOW ,
          
  .segment_pins[2].port = GPIO_PORTA ,
  .segment_pins[2].pin = GPIO_PIN2 ,
  .segment_pins[2].direction =GPIO_OUTPUT ,
  .segment_pins[2].logic = GPIO_LOW ,
          
  .segment_pins[3].port = GPIO_PORTA ,
  .segment_pins[3].pin = GPIO_PIN3 ,
  .segment_pins[3].direction =GPIO_OUTPUT ,
  .segment_pins[3].logic = GPIO_LOW ,
};

led_t green_led =
{
  .port =  GPIO_PORTB ,
  .pin =   GPIO_PIN1 ,
  .led_status = led_OFF
};

led_t red_led =
{
  .port =  GPIO_PORTB ,
  .pin =   GPIO_PIN0 ,
  .led_status = led_OFF
};

led_t yellow_led =
{
  .port =  GPIO_PORTB ,
  .pin =   GPIO_PIN2 ,
  .led_status = led_OFF
};


Std_ReturnType ret = 0 ;
volatile uint8 data_l = 0 , data_h = 0 , flag = 0 , tens = 0 , hundreds = 0 ; 
uint16 data = 0 ;

int main() {
    
    APPLICATION_INTIALIZE();
    ret = lcd_4bit_send_string_data(&Gas_Sensor , "Welcome");
    while(1)
    {
        
        if( flag == 2 )
        {
            data = data_l + (data_h << 8 ) ;
            tens = data / 10 ;
            hundreds = tens / 10 ;
            flag = 0 ;
        }
        gpio_pin_write_logic(& (seg1.segment_pins[0]) , ((data % 10)>>0) & 0x01 ) ;
        gpio_pin_write_logic(& (seg1.segment_pins[1]) , ((data % 10)>>1) & 0x01 ) ;
        gpio_pin_write_logic(& (seg1.segment_pins[2]) , ((data % 10)>>2) & 0x01 ) ;
        gpio_pin_write_logic(& (seg1.segment_pins[3]) , ((data % 10)>>3) & 0x01 ) ;
        
        gpio_pin_write_logic(& (seg2.segment_pins[0]) , ((tens % 10)>>0) & 0x01 ) ;
        gpio_pin_write_logic(& (seg2.segment_pins[1]) , ((tens % 10)>>1) & 0x01 ) ;
        gpio_pin_write_logic(& (seg2.segment_pins[2]) , ((tens % 10)>>2) & 0x01 ) ;
        gpio_pin_write_logic(& (seg2.segment_pins[3]) , ((tens % 10)>>3) & 0x01 ) ;
        
        gpio_pin_write_logic(& (seg3.segment_pins[0]) , (hundreds>>0) & 0x01 ) ;
        gpio_pin_write_logic(& (seg3.segment_pins[1]) , (hundreds>>1) & 0x01 ) ;
        gpio_pin_write_logic(& (seg3.segment_pins[2]) , (hundreds>>2) & 0x01 ) ;
        gpio_pin_write_logic(& (seg3.segment_pins[3]) , (hundreds>>3) & 0x01 ) ;
        
        if( ( data > 400 )  &&  ( data < 700 ) )
        {
            red_led_turn_off() ;
            green_led_turn_off();
            yellow_led_turn_on();
        }
        else if( data < 400  )
        {
            red_led_turn_off() ;
            green_led_turn_on();
            yellow_led_turn_off();
        }
        else if( data > 700  )
        {
            red_led_turn_on() ;
            green_led_turn_off();
            yellow_led_turn_off();
        }
                
    }


    return (EXIT_SUCCESS);
}


void APPLICATION_INTIALIZE(void)
{
    ret = Usart_Async_init(&Gas_Sensor);
    ret = seven_segment_intialize(&seg1);
    ret = seven_segment_intialize(&seg2);
    ret = seven_segment_intialize(&seg3);
    ret = led_intialize(&green_led);
    ret = led_intialize(&red_led);
    ret = led_intialize(&yellow_led);

}


void RX_Callback(void)
{
    if( flag == 0 )
    {
        ret = Usart_Async_ReadByte(&data_l) ;
        flag = 1 ;
    }
    else if(flag == 1)
    {
        ret = Usart_Async_ReadByte(&data_h) ;
        flag = 2 ;
    }
    
}

void green_led_turn_on(void)
{
    ret = led_turn_on(&green_led);
}

void green_led_turn_off(void)
{
    ret = led_turn_off(&green_led);
}

void red_led_turn_on(void)
{
    ret = led_turn_on(&red_led);
}

void red_led_turn_off(void)
{
    ret = led_turn_off(&red_led);
}

void yellow_led_turn_on(void)
{
    ret = led_turn_on(&yellow_led);
}

void yellow_led_turn_off(void)
{
    ret = led_turn_off(&yellow_led);
}