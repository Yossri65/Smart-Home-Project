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
#define NOT_PRESSED 0
#define PRESSED     1

#define Start_Pass_Cursor 12

#define NOT_STORED   0xFF
#define NOT_SELECTED 0xFF

#define BLOCK_MODE_TIME		   (uint32)10000
#define CHARACTER_PREVIEW_TIME (uint16)300
#define DEGREES_SYMBOL		   (uint8)0xDF

/*********************************** PIN Configuration ***********************************/
#define PASS_SIZE       (uint8)4
#define TRIES_ALLOWED   (uint8)3
#define PASSWORD_SYMBOL (uint8)'*'
#define ASCII_ZERO      (uint8)'0'

#define PASS_NOT_SET (uint8)0xFF
#define PASS_SET     (uint8)0x01

#define ADMIN_PASS_STATUS_ADDRESS (uint16)0X100
#define GUEST_PASS_STATUS_ADDRESS (uint16)0X101
#define EEPROM_ADMIN_ADDRESS      (uint16)0X102
#define EEPROM_GUEST_ADDRESS      (uint16)0X106
#define LOGIN_BLOCKED_ADDRESS     (uint16)0X10A
/*****************************************************************************************/
/************************************   Login configurations *****************************/
#define NO_MODE (uint8)0
#define ADMIN   (uint8)1
#define GUEST   (uint8)2
/*****************************************************************************************/

/************************************   Logic values *************************************/
#define FALSE   (uint8)0
#define TRUE    (uint8)1

#define Guest_PASS_TRUE (uint8)4
#define Admin_PASS_TRUE (uint8)4
/*****************************************************************************************/

#define CHECK_ADMIN_MODE        (uint8)'0'
#define CHECK_GUEST_MODE        (uint8)'1'

#define SELECT_ROOM1            (uint8)'1'
#define SELECT_ROOM2            (uint8)'2'
#define SELECT_ROOM3            (uint8)'3'
#define SELECT_ROOM4            (uint8)'4'
#define ADMIN_MORE_OPTION       (uint8)'4'

#define SELECT_ROOM4_ADMIN      (uint8)'1'
#define SELECT_TV               (uint8)'2'
#define SELECT_AIR_CONDITIONING (uint8)'3'
#define ADMIN_RET_OPTION        (uint8)'4'

#define SELECT_SET_TEMPERATURE  (uint8)'1'
#define SELECT_AIR_COND_CTRL    (uint8)'2'
#define SELECT_AIR_COND_RET     (uint8)'0'


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
/* ------------------------------------Macro Function Declarations------------------ */

/* ------------------------------------Data Type Declarations----------------------- */
extern Ch_LCD lcd1;
extern Key_Pad_t key_pad1;

extern Led_t led_Block;
extern Led_t led_Guest;
extern Led_t led_Admin;

/*----------------------pass varibles----------------------*/
uint8 Admin_pass_status = PASS_NOT_SET;
uint8 Guest_pass_status = PASS_NOT_SET;

uint8 Admin_Start_address = EEPROM_ADMIN_ADDRESS;
uint8 Guest_Start_address = EEPROM_GUEST_ADDRESS;

uint8 key_status = NOT_PRESSED;
uint8 cursor = Start_Pass_Cursor;

uint8 pass_counter = 1;
uint8 admin_pass_tries = TRIES_ALLOWED;
uint8 Guest_pass_tries = TRIES_ALLOWED;

uint8 eeprom_digit_read = NOT_STORED;
uint8 Admin_pass_flag = FALSE;

uint8 Guest_pass_flag = FALSE;

volatile uint8 spi_slave_reader = 0;
volatile uint8 count_sec_timer_flag = 0;

void timer0_isr(void);
Timer0_conf_t timer0 =
{
    .TIMER0_IntterruptHandeler = timer0_isr ,
    .Timer0_SELECT_BIT_Mode = TIMER0_16_BIT_MODE_,
    .Timer0_Mode = TIMER0_TIMER_MODE_,
    .Timer0_Source_Edge = TIMER0_RAISING_EDGE_,
    .TIMER0_PRESCALER_STATUS = TIMER0_PRESCALER_ENABLE_,
    .TIMER0_PRE_LOAD_VALUE = 26474,
    .Prescaler_Select = TIMER0_PRESCALER_DIV_BY_128
};

SPI_Config spi_master =
{
  .spi_control.ClockPolarity = SPI_IDLE_STATE_HIGH_LEVEL ,
  .spi_control.ClockSelect  = SPI_TRANSMIT_ACTIVE_TO_IDLE ,
  .spi_control.SampleSelect =SPI_DATA_SAMPLE_MIDDLE,
  .spi_serial_clk_mod_select = SPI_MASTER_FOSC_DIV4 ,
   
};
/* ------------------------------------Software Interface Declarations-------------- */
    
void softwareReset();

#endif	/* APPLICATION_H */

