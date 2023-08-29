/* 
 * File:   Application.c
 * Author: Yossri
 *
 * Created on July 7, 2023, 8:09 PM
 */ 
#include <builtins.h>
#include <pic18.h>

#include "Application.h"
Std_ReturnType ret = E_NOT_OK;
void intialization (void);

int main(void) {
    intialization();
    ret = lcd_send_command(&lcd1 , LCD_CLEAR);
    ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 7 , "Welcome");
    ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "To Smart Home System");
    __delay_ms(1000);
    ret = lcd_send_command(&lcd1 , LCD_CLEAR);
    /*check if this is the first time to set password*/
    ret = EEPROM_Data_ReadByte(0X100 , &Admin_pass_status); 
    ret = EEPROM_Data_ReadByte(0X101 , &Guest_pass_status);
    if(PASS_NOT_SET == Admin_pass_status || PASS_NOT_SET ==  Guest_pass_status)
    {
        ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Set Admin Password");
        ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "Admin Pass:");
        
/*====================== set admin for the first time ======================*/        
        while(pass_counter <= PASS_SIZE)
        {
            do{
                ret = Key_Pad_Get_Value(&key_pad1 ,&key_status);
            }while(NOT_PRESSED == key_status);
            ret = EEPROM_Data_WriteByte(Admin_Start_address ,key_status);/*Write digits in eeprom*/
            ret = lcd_send_ATpos_char_data(&lcd1 , 2 , cursor, key_status);/*Write digits in LCD*/
            __delay_ms(500);
            ret = lcd_send_command(&lcd1 , LCD_MOVE_CURSOR_LEFT_SHIFT);/*move cursor to write '*' */
            ret = lcd_send_ATpos_char_data(&lcd1 , 2 , cursor, '*');
            ret = lcd_send_command(&lcd1 , LCD_DISPLAY_ON_CURSORO_ON_BLOCKCURSOR_OFF);/*write '-' */
            key_status = NOT_PRESSED; /*make key_status NOT_PRESSED for next iteration*/
            cursor++; /*move cursor for next step*/
            Admin_Start_address++; /*move location of eeprom one byte */
            pass_counter++; 
        }
        ret = lcd_send_command(&lcd1 , LCD_DISPLAY_ON_CURSORO_oFF_BLOCKCURSOR_OFF);
        ret = lcd_send_ATpos_string_data(&lcd1 , 3 , 1 , "successfully added");
        __delay_ms(500);
        ret = lcd_send_command(&lcd1 , LCD_CLEAR);
        ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Set Guest Password");
        ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "Guest Pass:");
        
/*====================== set Guest for the first time ======================*/  
        /*reinitialize variables to set guest pass to*/
        pass_counter = 1;
        cursor =Start_Pass_Cursor;
        while(pass_counter <= PASS_SIZE)
        {
            do{
                ret = Key_Pad_Get_Value(&key_pad1 ,&key_status);
            }while(NOT_PRESSED == key_status);
            ret = EEPROM_Data_WriteByte(Guest_Start_address ,key_status);/*Write digits in eeprom*/
            ret = lcd_send_ATpos_char_data(&lcd1 , 2 , cursor, key_status);/*Write digits in LCD*/
            __delay_ms(500);
            ret = lcd_send_command(&lcd1 , LCD_MOVE_CURSOR_LEFT_SHIFT);/*move cursor to write '*' */
            ret = lcd_send_ATpos_char_data(&lcd1 , 2 , cursor, PASSWORD_SYMBOL);
            ret = lcd_send_command(&lcd1 , LCD_DISPLAY_ON_CURSORO_ON_BLOCKCURSOR_OFF);/*write '-' */
            key_status = NOT_PRESSED;/*make key_status NOT_PRESSED for next iteration*/
            cursor++;/*move cursor for next step*/
            Guest_Start_address++;/*move location of eeprom one byte */
            pass_counter++;
        }
        __delay_ms(200);
        ret = lcd_send_command(&lcd1 , LCD_DISPLAY_ON_CURSORO_oFF_BLOCKCURSOR_OFF);
        ret = lcd_send_ATpos_string_data(&lcd1 , 3 , 1 , "successfully added");
        __delay_ms(500);
        ret = lcd_send_command(&lcd1 , LCD_CLEAR);
        Admin_pass_status = PASS_SET;
        Guest_pass_status = PASS_SET;
        ret = EEPROM_Data_WriteByte(ADMIN_PASS_STATUS_ADDRESS ,Admin_pass_status);/*Write admin pass status in eeprom*/
        ret = EEPROM_Data_WriteByte(GUEST_PASS_STATUS_ADDRESS ,Guest_pass_status);/*Write Guest pass status in eeprom*/
    }
    /*if the password is set before*/
    else if(PASS_SET == Admin_pass_status || PASS_SET ==  Guest_pass_status)
    {
        ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Select Mode :"); /*two modes in home admin mode and guest mode*/
        ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "1: Admin");  /*press one to admin mode */
        ret = lcd_send_ATpos_string_data(&lcd1 , 3 , 1 , "2: Guest"); /*press two to guest mode */
      
        do {
            ret = Key_Pad_Get_Value(&key_pad1, &key_status);
        } while (NOT_PRESSED == key_status);
/*======================check if the user choose the admin mode======================*/
        while('1' == key_status && admin_pass_tries > 0){
            ret = lcd_send_command(&lcd1 , LCD_CLEAR);
            ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Admin Mode:-");
            ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "Write pass:"); 
            key_status = NOT_PRESSED;
            __delay_ms(300);
            while(pass_counter <= 4)
            {
                do{
                    ret = Key_Pad_Get_Value(&key_pad1 ,&key_status);
                }while(NOT_PRESSED == key_status);
                ret = EEPROM_Data_ReadByte(Admin_Start_address ,&eeprom_digit_read);/*read digits in eeprom*/
                if(eeprom_digit_read == key_status){ /*check if entered digit equal pass digit or not if equal plus pass_flag*/
                    Admin_pass_flag++;
                }else {/*Nothing*/}
                ret = lcd_send_ATpos_char_data(&lcd1 , 2 , cursor, key_status);/*Write digits in LCD*/
                __delay_ms(500);
                ret = lcd_send_command(&lcd1 , LCD_MOVE_CURSOR_LEFT_SHIFT);/*move cursor to write '*' */
                ret = lcd_send_ATpos_char_data(&lcd1 , 2 , cursor, PASSWORD_SYMBOL);
                ret = lcd_send_command(&lcd1 , LCD_DISPLAY_ON_CURSORO_ON_BLOCKCURSOR_OFF);/*write '-' */
                key_status = NOT_PRESSED;/*make key_status NOT_PRESSED for next iteration*/
                cursor++;/*move cursor for next step*/
                Admin_Start_address++;/*move location of eeprom one byte */
                pass_counter++;
            }
            if(Admin_PASS_TRUE == Admin_pass_flag){ /*check if the password is true*/
                ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                ret = lcd_send_command(&lcd1 , LCD_DISPLAY_ON_CURSORO_oFF_BLOCKCURSOR_OFF);
                ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "Success Login..");
                key_status = NOT_PRESSED;/*make key_status NOT_PRESSED to exit from loop*/
                __delay_ms(300);
            }else{
                key_status = '1';/*make key_status NOT_PRESSED to enter the loop again*/
                ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                ret = lcd_send_command(&lcd1 , LCD_DISPLAY_ON_CURSORO_oFF_BLOCKCURSOR_OFF);
                ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Wrong input.");
                ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "Tries left:");
                ret = lcd_send_ATpos_char_data(&lcd1 , 2 , 12 , (admin_pass_tries -1) +ASCII_ZERO);
                __delay_ms(500);
                /*reset initialization of variables to new try to write password*/
                Admin_pass_flag = 1;
                pass_counter = 1;
                Admin_Start_address = EEPROM_ADMIN_ADDRESS;
                cursor =Start_Pass_Cursor;
                admin_pass_tries--; /*decrement pass_tries by one*/
            }
        }
/*======================check if the user choose the guest mode======================*/
        while('2' == key_status && Guest_pass_tries > 0){
            ret = lcd_send_command(&lcd1 , LCD_CLEAR);
            ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Guest Mode:-");
            ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "Write pass:"); 
            key_status = NOT_PRESSED;
            __delay_ms(300);
            while(pass_counter <= 4)
            {
                do{
                    ret = Key_Pad_Get_Value(&key_pad1 ,&key_status);
                }while(NOT_PRESSED == key_status);
                ret = EEPROM_Data_ReadByte(Guest_Start_address ,&eeprom_digit_read);/*read digits in eeprom*/
                if(eeprom_digit_read == key_status){ /*check if entered digit equal pass digit or not if equal plus pass_flag*/
                    Guest_pass_flag++;
                }else {/*Nothing*/}
                ret = lcd_send_ATpos_char_data(&lcd1 , 2 , cursor, key_status);/*Write digits in LCD*/
                __delay_ms(500);
                ret = lcd_send_command(&lcd1 , LCD_MOVE_CURSOR_LEFT_SHIFT);/*move cursor to write '*' */
                ret = lcd_send_ATpos_char_data(&lcd1 , 2 , cursor, PASSWORD_SYMBOL);
                ret = lcd_send_command(&lcd1 , LCD_DISPLAY_ON_CURSORO_ON_BLOCKCURSOR_OFF);/*write '-' */
                key_status = NOT_PRESSED;/*make key_status NOT_PRESSED for next iteration*/
                cursor++;/*move cursor for next step*/
                Guest_Start_address++;/*move location of eeprom one byte */
                pass_counter++;
            }
            if(Guest_PASS_TRUE == Guest_pass_flag){ /*check if the password is true*/
                ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                ret = lcd_send_command(&lcd1 , LCD_DISPLAY_ON_CURSORO_oFF_BLOCKCURSOR_OFF);
                ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "Success Login..");
                key_status = NOT_PRESSED;/*make key_status NOT_PRESSED to exit from loop*/
                __delay_ms(1000);
                ret = lcd_send_command(&lcd1 , LCD_CLEAR);
            }else{
                key_status = '2';/*make key_status NOT_PRESSED to enter the loop again*/
                ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                ret = lcd_send_command(&lcd1 , LCD_DISPLAY_ON_CURSORO_oFF_BLOCKCURSOR_OFF);
                ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Wrong input.");
                ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "Tries left:");
                ret = lcd_send_ATpos_char_data(&lcd1 , 2 , 12 , (Guest_pass_tries -1) +ASCII_ZERO);
                __delay_ms(500);
                /*reset initialization of variables to new try to write password*/
                Guest_pass_flag = 0;
                Guest_Start_address = EEPROM_GUEST_ADDRESS;
                pass_counter = 1;
                cursor =Start_Pass_Cursor;
                Guest_pass_tries--; /*decrement pass_tries by one*/
            }
            
            
    }
    
    ret = Timer0_Intialization(&timer0);
    
    
/********************************************************************************************************/ 
    
    while(1)
    {
        /*check if the user finished his tries to enter the pass ,enter the block mode and reset the system again*/
        if (0 == admin_pass_tries) {
            ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "loading blocked..");
            ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "wait  10 sec");
            ret = Led_Turn_On(&led_Block);
            __delay_ms(BLOCK_MODE_TIME);
            break;  
        }else{/*Nothing*/}
        /*check if the user finished his tries to enter the pass ,enter the block mode and reset the system again*/
        if (0 == Guest_pass_tries) {
            count_sec_timer_flag = 0;
            ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "loading blocked..");
            ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "wait  10 sec");
            ret = Led_Turn_On(&led_Block);
            __delay_ms(BLOCK_MODE_TIME);
            break;
        }else{/*Nothing*/}
/*===============================if the user enter the right password in guest mode or admin mode===============================*/
        while(Guest_PASS_TRUE == Guest_pass_flag || Admin_PASS_TRUE == Admin_pass_flag){
            if(Guest_PASS_TRUE == Guest_pass_flag){
                ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Guest Mode:-");
            }else if(Admin_PASS_TRUE == Admin_pass_flag){
                ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Admin Mode:-");
            }else{/*Nothing*/}
            
            ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "1:Room1"); /*press '1' for room1*/
            ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 11 , "2:Room2");/*press '2' for room2*/
            ret = lcd_send_ATpos_string_data(&lcd1 , 3 , 1 , "3:Room3"); /*press '3' for room3*/
            if(Guest_PASS_TRUE == Guest_pass_flag){
                ret = lcd_send_ATpos_string_data(&lcd1 , 3 , 11 , "4:Room4");/*press '4' for room4*/
                ret = Led_Turn_On(&led_Guest);
            }else if(Admin_PASS_TRUE == Admin_pass_flag){
                ret = lcd_send_ATpos_string_data(&lcd1 , 3 , 11 , "4:More");
                ret = Led_Turn_On(&led_Admin);
            }else{/*Nothing*/}
            ret = lcd_send_ATpos_string_data(&lcd1 , 4 , 1 , "Press'ON/C'to reset");
            key_status = NOT_PRESSED;
            do {
                ret = Key_Pad_Get_Value(&key_pad1, &key_status);
            } while (NOT_PRESSED == key_status);
            __delay_ms(300);
            
            /*to reset the systm press @ that equal on/c on keypad*/
            if('@' == key_status){
                softwareReset();
            }
                
            /*validation*/
            while('1' != key_status &&'2' != key_status &&'3' != key_status &&'4' != key_status && '@' != key_status){
                ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                    ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Wrong choice");
                    __delay_ms(500);
                    break;
            }
/*==============================room1 functionality==============================*/
            while('1' == key_status){/*if room1 is choosen*/
                ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                
                
                ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Room1 S:");
                switch(spi_slave_reader)
                {
                    case 1: ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 9 , "ON "); break;
                    case 0: ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 9 , "OFF"); break;
                }
                ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "1:led on");
                ret = lcd_send_ATpos_string_data(&lcd1 , 3 , 1 , "2:led off");
                ret = lcd_send_ATpos_string_data(&lcd1 , 4 , 1 , "0:Ret");
                key_status = NOT_PRESSED;
                do {
                    ret = Key_Pad_Get_Value(&key_pad1, &key_status);
                } while (NOT_PRESSED == key_status);
                __delay_ms(300);
                switch(key_status)
                {
                    case '1': 
                        ret = SPI_Send_Byte(ROOM1_ON);
                        spi_slave_reader = 1;
                        break;
                    case '2': 
                        ret = SPI_Send_Byte(ROOM1_OFF);
                        spi_slave_reader = 0;
                        break;
                }
                if('0' == key_status){
                    ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                    break;
                }else if('1' != key_status && '2' != key_status){
                    ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                    ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Wrong choice");
                    __delay_ms(500);
                }else{}
                key_status = '1';
                __delay_ms(500);
            }
/*==============================room2 functionality==============================*/
            while('2' == key_status){/*if room2 is choosen*/
                ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Room2 S:");
                switch(spi_slave_reader)
                {
                    case 1: ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 9 , "ON "); break;
                    case 0: ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 9 , "OFF"); break;
                }
                ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "1:led on");
                ret = lcd_send_ATpos_string_data(&lcd1 , 3 , 1 , "2:led off");
                ret = lcd_send_ATpos_string_data(&lcd1 , 4 , 1 , "0:Ret");
                key_status = NOT_PRESSED;
                do {
                    ret = Key_Pad_Get_Value(&key_pad1, &key_status);
                } while (NOT_PRESSED == key_status);
                __delay_ms(300);
                switch(key_status)
                {
                    case '1': ret = SPI_Send_Byte(ROOM2_ON) ; spi_slave_reader = 1; break;
                    case '2': ret = SPI_Send_Byte(ROOM2_OFF); spi_slave_reader = 0; break;
                }
                if('0' == key_status){
                    ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                    break;
                }else if('1' != key_status && '2' != key_status){
                    ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                    ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Wrong choice");
                    __delay_ms(1000);
                }
                key_status = '2';
                __delay_ms(500);
            }
/*==============================room3 functionality==============================*/
            while('3' == key_status){/*if room3 is choosen*/
                ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Room3 S:");
                switch(spi_slave_reader)
                {
                    case 1: ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 9 , "ON "); break;
                    case 0: ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 9 , "OFF"); break;
                }
                ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "1:led on");
                ret = lcd_send_ATpos_string_data(&lcd1 , 3 , 1 , "2:led off");
                ret = lcd_send_ATpos_string_data(&lcd1 , 4 , 1 , "0:Ret");
                key_status = NOT_PRESSED;
                do {
                    ret = Key_Pad_Get_Value(&key_pad1, &key_status);
                } while (NOT_PRESSED == key_status);
                __delay_ms(300);
                switch(key_status)
                {
                    case '1': ret = SPI_Send_Byte(ROOM3_ON);  spi_slave_reader = 1; break;
                    case '2': ret = SPI_Send_Byte(ROOM3_OFF); spi_slave_reader = 0; break;
                }
                if('0' == key_status){
                    ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                    break;
                }else if('1' != key_status && '2' != key_status){
                    ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                    ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Wrong choice");
                    __delay_ms(500);
                }else{
                    
                }
                key_status = '3';
                __delay_ms(500);
            }
            if(Guest_PASS_TRUE == Guest_pass_flag){
                    while('4' == key_status){/*if room4 is choosen*/
                    ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                    ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Room4 S:");
                    switch(spi_slave_reader)
                {
                    case 1: ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 9 , "ON "); break;
                    case 0: ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 9 , "OFF"); break;
                }
                    ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "1:led on");
                    ret = lcd_send_ATpos_string_data(&lcd1 , 3 , 1 , "2:led off");
                    ret = lcd_send_ATpos_string_data(&lcd1 , 4 , 1 , "0:Ret");
                    key_status = NOT_PRESSED;
                    do {
                        ret = Key_Pad_Get_Value(&key_pad1, &key_status);
                    } while (NOT_PRESSED == key_status);
                    __delay_ms(300);
                    switch (key_status) {
                            case '1': ret = SPI_Send_Byte(ROOM4_ON);
                            spi_slave_reader = 1;
                                break;
                            case '2': ret = SPI_Send_Byte(ROOM4_OFF);
                            spi_slave_reader = 0;
                                break;
                        }
                    if('0' == key_status){
                        ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                        break;
                    }else if('1' != key_status && '2' != key_status){
                        ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                        ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Wrong choice");
                        __delay_ms(500);
                    }
                    key_status = '4';
                    __delay_ms(500);
                }
            }else if(Admin_PASS_TRUE == Admin_pass_flag){
                    while('4' == key_status){/*if more is choosen*/
                    ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                    ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Admin Mode:-");
                    ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "1:Room4");
                    ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 11 , "2:TV");
                    ret = lcd_send_ATpos_string_data(&lcd1 , 3 , 1 , "3:Air con.");
                    ret = lcd_send_ATpos_string_data(&lcd1 , 3 , 11 , "0:Ret");
                    key_status = NOT_PRESSED;
                    do {
                        ret = Key_Pad_Get_Value(&key_pad1, &key_status);
                    } while (NOT_PRESSED == key_status);    
                    __delay_ms(300);
/*==============================ROOM4 functionality==============================*/
                    while ('1' == key_status) {/*if room4 is choosen*/
                            ret = lcd_send_command(&lcd1, LCD_CLEAR);
                            ret = lcd_send_ATpos_string_data(&lcd1, 1, 1, "Room4 S:");
                            switch (spi_slave_reader) {
                                case 1: ret = lcd_send_ATpos_string_data(&lcd1, 1, 9, "ON ");
                                    break;
                                case 0: ret = lcd_send_ATpos_string_data(&lcd1, 1, 9, "OFF");
                                    break;
                            }
                            ret = lcd_send_ATpos_string_data(&lcd1, 2, 1, "1:led on");
                            ret = lcd_send_ATpos_string_data(&lcd1, 3, 1, "2:led off");
                            ret = lcd_send_ATpos_string_data(&lcd1, 4, 1, "0:Ret");
                            key_status = NOT_PRESSED;
                            do {
                                ret = Key_Pad_Get_Value(&key_pad1, &key_status);
                            } while (NOT_PRESSED == key_status);
                            __delay_ms(300);
                        switch (key_status) {
                                case '1': ret = SPI_Send_Byte(ROOM4_ON);
                                spi_slave_reader = 1;
                                    break;
                                case '2': ret = SPI_Send_Byte(ROOM4_OFF);
                                spi_slave_reader = 0;
                                    break;
                            }
                            if ('0' == key_status) {
                                ret = lcd_send_command(&lcd1, LCD_CLEAR);
                                break;
                            } else if('1' != key_status && '2' != key_status){
                                ret = lcd_send_command(&lcd1, LCD_CLEAR);
                                ret = lcd_send_ATpos_string_data(&lcd1, 1, 1, "Wrong choice");
                                __delay_ms(500);
                            }
                            key_status = '1';
                            __delay_ms(500);
                        }
/*==============================TV functionality==============================*/
                    while ('2' == key_status){
                        ret = lcd_send_command(&lcd1, LCD_CLEAR);
                            ret = lcd_send_ATpos_string_data(&lcd1, 1, 1, "TV S:");
                            switch (spi_slave_reader) {
                                case 1: ret = lcd_send_ATpos_string_data(&lcd1, 1, 9, "ON ");
                                    break;
                                case 0: ret = lcd_send_ATpos_string_data(&lcd1, 1, 9, "OFF");
                                    break;
                            }
                            ret = lcd_send_ATpos_string_data(&lcd1, 2, 1, "1:TV on");
                            ret = lcd_send_ATpos_string_data(&lcd1, 3, 1, "2:TV off");
                            ret = lcd_send_ATpos_string_data(&lcd1, 4, 1, "0:Ret");
                            key_status = NOT_PRESSED;
                            do {
                                ret = Key_Pad_Get_Value(&key_pad1, &key_status);
                            } while (NOT_PRESSED == key_status);
                            __delay_ms(300);
                            switch (key_status) {
                                case '1': ret = SPI_Send_Byte(TV_ON);  spi_slave_reader = 1; break;
                                case '2': ret = SPI_Send_Byte(TV_OFF); spi_slave_reader = 0; break;
                            }
                            if ('0' == key_status) {
                                ret = lcd_send_command(&lcd1, LCD_CLEAR);
                                break;
                            } else if('1' != key_status && '2' != key_status){
                                ret = lcd_send_command(&lcd1, LCD_CLEAR);
                                ret = lcd_send_ATpos_string_data(&lcd1, 1, 1, "Wrong choice");
                                __delay_ms(500);
                            }
                            key_status = '2';
                            __delay_ms(500);
                    }
/*==============================air condition functionality==============================*/
                    while('3' == key_status){ 
                        ret = lcd_send_command(&lcd1, LCD_CLEAR);
                      ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "air conditioner:-");
                      ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "1:Control");
                      ret = lcd_send_ATpos_string_data(&lcd1 , 3 , 1 , "2:Set Temperature");
                      ret = lcd_send_ATpos_string_data(&lcd1, 4, 1, "0:Ret");
                      key_status = NOT_PRESSED;
                      do {
                            ret = Key_Pad_Get_Value(&key_pad1, &key_status);
                         } while (NOT_PRESSED == key_status);
                      __delay_ms(300);
                      ret = lcd_send_command(&lcd1, LCD_CLEAR);
                      /*control is selected*/
                      while('1' == key_status){ 
                          ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "air conditioner:-");
                          ret = lcd_send_ATpos_string_data(&lcd1, 2, 1, "1:air conditioner on");
                          ret = lcd_send_ATpos_string_data(&lcd1, 3, 1, "2:air conditioner off");
                          ret = lcd_send_ATpos_string_data(&lcd1, 4, 1, "0:Ret");
                          key_status = NOT_PRESSED;
                          do {
                                ret = Key_Pad_Get_Value(&key_pad1, &key_status);
                             } while (NOT_PRESSED == key_status);
                          __delay_ms(300);
                          switch (key_status) {
                                case '1':
                                    ret = SPI_Send_Byte(AirCon_ON); 
                                    spi_slave_reader = 1; 
                                    break;
                                case '2': ret = SPI_Send_Byte(AirCon_OFF); spi_slave_reader = 0; break;
                            }
                          
                          if ('0' == key_status) {
                                ret = lcd_send_command(&lcd1, LCD_CLEAR);
                                break;
                            } else if('1' != key_status && '2' != key_status){
                                ret = lcd_send_command(&lcd1, LCD_CLEAR);
                                ret = lcd_send_ATpos_string_data(&lcd1, 1, 1, "Wrong choice");
                                __delay_ms(500);
                            }
                            key_status = '1';
                            __delay_ms(500);
                      }
                      /*Set Temperature is selected*/
                      if('2' == key_status){ 
                          ret = lcd_send_command(&lcd1, LCD_CLEAR);
                          ret = lcd_send_ATpos_string_data(&lcd1, 2, 1, "Set Temperature:--");
                          ret = lcd_send_ATpos_string_data(&lcd1, 3, 1, "set value less than 35");
                          /*set first digit in degree*/
                          key_status = NOT_PRESSED;
                          do {
                                ret = Key_Pad_Get_Value(&key_pad1, &key_status);
                             } while (NOT_PRESSED == key_status);
                          __delay_ms(300);
                          ret = lcd_send_command(&lcd1, LCD_MOVE_CURSOR_LEFT_SHIFT);
                          ret = lcd_send_command(&lcd1, LCD_MOVE_CURSOR_LEFT_SHIFT);
                          ret = lcd_send_char_data(&lcd1, key_status);
                          Degree_sent = (key_status - ASCII_ZERO) * 10;
                          
                          /*set second digit in degree*/
                          key_status = NOT_PRESSED;
                          do {
                                ret = Key_Pad_Get_Value(&key_pad1, &key_status);
                             } while (NOT_PRESSED == key_status);
                          __delay_ms(300);
                          ret = lcd_send_char_data(&lcd1, key_status);
                          Degree_sent += (key_status - ASCII_ZERO);
                          
                          if(Degree_sent >35)
                          {
                              Degree_sent = 35;
                              ret = lcd_send_command(&lcd1, LCD_MOVE_CURSOR_LEFT_SHIFT);
                              ret = lcd_send_command(&lcd1, LCD_MOVE_CURSOR_LEFT_SHIFT);
                              ret = lcd_send_char_data(&lcd1,(Degree_sent /10)+ASCII_ZERO);
                              ret = lcd_send_char_data(&lcd1,(Degree_sent %10)+ASCII_ZERO);
                          }
                          __delay_ms(1000);
                      }
                      key_status = '3';
                      if ('0' == key_status) {
                                ret = lcd_send_command(&lcd1, LCD_CLEAR);
                                break;
                            } else if('1' != key_status && '2' != key_status && '3' != key_status){
                                ret = lcd_send_command(&lcd1, LCD_CLEAR);
                                ret = lcd_send_ATpos_string_data(&lcd1, 1, 1, "Wrong choice");
                                __delay_ms(500);
                            }
                            __delay_ms(500);
                    }
                    if('0' == key_status){
                        ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                        break;
                    }else if('1' != key_status && '2' != key_status && '3' != key_status ){
                        ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                        ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Wrong choice");
                        __delay_ms(500);
                    }else{/*Nothing*/}
                    key_status = '4';
                    __delay_ms(500);
                }
            }else{/*Nothing*/}
        }
      }
    return (EXIT_SUCCESS);
    }
}

void intialization (void)
{
    ecu_intialize( );
    ret = SPI_Init(&spi_master);
    
}
void timer0_isr(void){
    count_sec_timer_flag++;
    if(2 == count_sec_timer_flag && Guest_PASS_TRUE == Guest_pass_flag)
    {
        ret = lcd_send_command(&lcd1 , LCD_CLEAR);
        ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "session timeout");
        __delay_ms(1000);
        softwareReset();
    }
}
void softwareReset()
{
     // Configure the Watchdog Timer
    WDTCONbits.SWDTEN = 1;   // Enable the WDT
    CLRWDT();
    // Perform a software reset by letting the WDT time out
    while (1);
}