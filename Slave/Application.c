/* 
 * File:   Application.c
 * Author: Yossri
 *
 * Created on July 7, 2023, 8:09 PM
 */ 
#include <builtins.h>

#include "Application.h"
Std_ReturnType ret = E_NOT_OK;
void intialization (void);


int main(void) {
    intialization();
    while(1){
        
        
       ret = SPI_Read_Byte(&spi_read_master);
       switch(spi_read_master)
       {
           case ROOM1_ON: 
               ret = Led_Turn_On(&led_Room1);  
               ret = SPI_Send_Byte(1);
           break;
           case ROOM2_ON: 
               ret = Led_Turn_On(&led_Room2); 
               ret = SPI_Send_Byte(1);
               break;
           case ROOM3_ON:
               ret = Led_Turn_On(&led_Room3); 
               ret = SPI_Send_Byte(1);
               break;
           case ROOM4_ON: 
               ret = Led_Turn_On(&led_Room4);
               ret = SPI_Send_Byte(1);
               break;
           
           case ROOM1_OFF:
               ret = Led_Turn_Off(&led_Room1); 
               ret = SPI_Send_Byte(0);
               break;
           case ROOM2_OFF:
               ret = Led_Turn_Off(&led_Room2);
               ret = SPI_Send_Byte(0);
               break;
           case ROOM3_OFF: 
               ret = Led_Turn_Off(&led_Room3); 
               ret = SPI_Send_Byte(0);
               break;
           case ROOM4_OFF: 
               ret = Led_Turn_Off(&led_Room4); 
               ret = SPI_Send_Byte(0);
               break;
           
           case TV_ON:
               ret = Led_Turn_On(&led_TV); break;
               ret = SPI_Send_Byte(1);
           case TV_OFF:
               ret = Led_Turn_Off(&led_TV); break;
               ret = SPI_Send_Byte(0);
               
            case AirCon_ON:
                ret = SPI_Read_Byte(&spi_read_master);
                while(1)
                {
                    ret = ADC_Start_Conversion_Interrupt_Mode(&adc_1  ,ADC_CHANNEL_AN0);
                    temperature = 4.88f * conv_result_temp;
                    temperature /= 10;
                    if(temperature > spi_read_master){
                        ret = Led_Turn_On(&led_Air_con); 
                        //ret = SPI_Send_Byte(1);
                    }else{
                        ret = Led_Turn_Off(&led_Air_con); 
                        //ret = SPI_Send_Byte(0);
                    }
                    
                    if(temperature == 30)
                    {
                        break;
                    }
                }

                break;
           case AirCon_OFF:
               ret = Led_Turn_Off(&led_Air_con); 
               ret = SPI_Send_Byte(0);
               break;
           
           default:/*Nothing*/ ;
       }
    }
    return (EXIT_SUCCESS);
}
void intialization (void){
 ret = SPI_Init(&spi_slave); 
 ret = Led_Intialize(&led_Room1);
 ret = Led_Intialize(&led_Room2);
 ret = Led_Intialize(&led_Room3);
 ret = Led_Intialize(&led_Room4);
 ret = Led_Intialize(&led_TV);
 ret = Led_Intialize(&led_Air_con);
 
 ret = ADC_Intialization(&adc_1);
}

void ADC1_ISR(void)
{
    ret = ADC_Get_Conversion_Result(&adc_1 ,&conv_result_temp);
}