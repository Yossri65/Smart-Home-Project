/* 
 * File:   Application.h
 * Author: Yossri
 *
 * Created on July 7, 2023, 8:08 PM
 */

#ifndef APPLICATION_H
#define	APPLICATION_H


/* ------------------------------------Includes--------------------------------------*/
#include "ECU_Layer/ecu_intialize.h"
#include "MCAL_Layer/Interrupt/mcal_external_interrupt.h"
#include "MCAL_Layer/Interrupt/mcal_internal_interrupt.h"
#include "MCAL_Layer/EEPROM/mcal_eeprom.h"
#include "MCAL_Layer/MCAL_ADC/mcal_adc.h"

#include "MCAL_Layer/Timer0/mcal_timer0.h"
#include "MCAL_Layer/Timer1/mcal_timer1.h"
#include "MCAL_Layer/Timer2/mcal_timer2.h"
#include "MCAL_Layer/Timer3/mcal_timer3.h"
#include "MCAL_Layer/CCP1/hal_ccp.h"
#include "MCAL_Layer/ECCP/hal_eccp.h"
#include "MCAL_Layer/USART/hal_usart.h"
#include "MCAL_Layer/SPI/hal_spi.h"
#include "MCAL_Layer/I2C/hal_i2c.h"
/* ------------------------------------Macro Declarations--------------------------- */

#define ROOM1_ON    (uint8)1
#define ROOM2_ON    (uint8)1
#define ROOM3_ON    (uint8)1
#define ROOM4_ON    (uint8)1

#define ROOM1_OFF   (uint8)0
#define ROOM2_OFF   (uint8)0
#define ROOM3_OFF   (uint8)0
#define ROOM4_OFF   (uint8)0

#define TV_ON    (uint8)1
#define TV_OFF   (uint8)0
/* ------------------------------------Macro Function Declarations------------------ */
#define ROOM1_ON    (uint8)1
#define ROOM2_ON    (uint8)2
#define ROOM3_ON    (uint8)3
#define ROOM4_ON    (uint8)4

#define ROOM1_OFF   (uint8)5
#define ROOM2_OFF   (uint8)6
#define ROOM3_OFF   (uint8)7
#define ROOM4_OFF   (uint8)8

#define TV_ON    (uint8)9
#define TV_OFF   (uint8)0

#define AirCon_ON    (uint8)10
#define AirCon_OFF   (uint8)11
/* ------------------------------------Data Type Declarations----------------------- */
volatile uint8 spi_read_master = 0;
uint16 temperature = 0;
volatile uint16 conv_result_temp  = 0;

SPI_Config spi_slave =
{ 
  .spi_control.ClockPolarity = SPI_IDLE_STATE_HIGH_LEVEL ,
  .spi_control.ClockSelect  = SPI_TRANSMIT_ACTIVE_TO_IDLE ,
  .spi_control.SampleSelect =SPI_DATA_SAMPLE_MIDDLE,
  .spi_serial_clk_mod_select =  SPI_SLAVE_SS_DISABLE,
};

Led_t led_Room1 =
{
  .Port_Name = PortD_Index,
  .Pin_Name = pin0 ,
  .Led_Status = LED_OFF
};
Led_t led_Room2 =
{
  .Port_Name = PortD_Index,
  .Pin_Name = pin1 ,
  .Led_Status = LED_OFF
};
Led_t led_Room3 =
{
  .Port_Name = PortD_Index,
  .Pin_Name = pin2 ,
  .Led_Status = LED_OFF
};
Led_t led_Room4 =
{
  .Port_Name = PortD_Index,
  .Pin_Name = pin3 ,
  .Led_Status = LED_OFF
};
Led_t led_TV =
{
  .Port_Name = PortB_Index,
  .Pin_Name = pin6 ,
  .Led_Status = LED_OFF
};
Led_t led_Air_con =
{
  .Port_Name = PortB_Index,
  .Pin_Name = pin7 ,
  .Led_Status = LED_OFF
};

void ADC1_ISR(void);
ADC_Conf_t adc_1 =
{
  .ADC_IntterruptHandeler = ADC1_ISR ,
  .ADC_Acquisition_Time =  ADC_12_TAD ,
  .ADC_Conversion_Clock = ADC_CONVERSION_CLOCK_FOSC_DIV_16 ,
  .ADC_Channel = ADC_CHANNEL_AN0,
  .format_status = ADC_RIGHT_JUSTIFIED ,
  .voltage_ref = ADC_VOLTAGE_REF_DISABLE
};
/* ------------------------------------Software Interface Declarations-------------- */

#endif	/* APPLICATION_H */

