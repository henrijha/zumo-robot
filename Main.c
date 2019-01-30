/*
 @mainpage ZumoBot Project
 @brief    You can make your own ZumoBot with various sensors.
 @details  <br><br>
    <p>
    <B>General</B><br>
    You will use Pololu Zumo Shields for your robot project with CY8CKIT-059(PSoC 5LP) from Cypress semiconductor.This 
    library has basic methods of various sensors and communications so that you can make what you want with them. <br> 
    <br><br>
    </p>
    
    <p>
    <B>Sensors</B><br>
    &nbsp;Included: <br>
        &nbsp;&nbsp;&nbsp;&nbsp;LSM303D: Accelerometer & Magnetometer<br>
        &nbsp;&nbsp;&nbsp;&nbsp;L3GD20H: Gyroscope<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Reflectance sensor<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Motors
    &nbsp;Wii nunchuck<br>
    &nbsp;TSOP-2236: IR Receiver<br>
    &nbsp;HC-SR04: Ultrasonic sensor<br>
    &nbsp;APDS-9301: Ambient light sensor<br>
    &nbsp;IR LED <br><br><br>
    </p>
    
    <p>
    <B>Communication</B><br>
    I2C, UART, Serial<br>
    </p>
*/

#include <project.h>
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "Motor.h"
#include "Ultra.h"
#include "Nunchuk.h"
#include "Reflectance.h"
#include "Gyro.h"
#include "Accel_magnet.h"
#include "LSM303D.h"
#include "IR.h"
#include "Beep.h"
#include "mqtt_sender.h"
#include <time.h>
#include <sys/time.h>
#include "serial1.h"
#include <unistd.h>

/**
 * @file    main.c
 * @brief   
 * @details  ** Enable global interrupt since Zumo library uses interrupts. **<br>&nbsp;&nbsp;&nbsp;CyGlobalIntEnable;<br>
*/

#if 0
float checkBat();
void m_forward(uint8, uint32);
void tank_turn(uint8, uint32, uint8);
void search_turn(void);

void zmain(void *p)
{
    (void) p;
    ADC_Battery_Start();
    Ultra_Start();
    motor_start();
    motor_forward(0,0);
    uint8 d = 0;
    float bat = 0;
    
    while(1) {
        bat = checkBat();
        d = Ultra_GetDistance();
        //printf("distance = %d\t battery = %f\r\n", d, bat);
        vTaskDelay(100);
        d = Ultra_GetDistance();
        if(d > 90)
            m_forward(150, 0);
        else if (d < 35)
            m_forward(90, 0);
        else 
            m_forward(200,0);
        search_turn();
    }
}
#endif
#if 1
void tank_turn(uint8 spd, uint32 delay, uint8 side)
{
    uint8 spd_l, spd_r; // combine the speed for both motors into one parameter^
    spd_l = spd; 
    spd_r = spd;
        // uint8 side should be either 1 or 0 for clarity's sake
    if (side == 1) {
        MotorDirRight_Write(0); // put one motor in reverse (1) and the other in normal mode (0)
        MotorDirLeft_Write(1);
    }
    else {
        MotorDirLeft_Write(0);
        MotorDirRight_Write(1);
    }
    motor_turn(spd_l, spd_r, delay);
    MotorDirLeft_Write(0); // remember to reset the motors into forward mode
    MotorDirRight_Write(0); // it's cleaner to call the function this way
}
void m_forward(uint8 speed, uint32 delay)
{
    MotorDirLeft_Write(0);
    MotorDirRight_Write(0);
    motor_turn(speed*1.04, speed*0.97, delay);
}
#endif
#if 0
float checkBat(void)
{
    int16 adcresult = 0;
    float volts = 0.0;
    uint8 button = 1;
    BatteryLed_Write(0);    // SW1_Read() returns zero when button is pressed and vice versa
        ADC_Battery_StartConvert(); // start sampling
        if(ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT)) {   // wait for ADC converted value
            adcresult = ADC_Battery_GetResult16(); // get the ADC value (0 - 4095)
            volts = ((float)adcresult * (5.0f/4095.0f)) / (2.0f / 3.0f);
            printf("%d %f\r\n",adcresult, volts);
            }
        while ( volts < 4.0 && button == 1) {
            BatteryLed_Write(1);
            vTaskDelay(500);
            button = SW1_Read();
            BatteryLed_Write(0);
            vTaskDelay(500);
            button = SW1_Read();
        }
    return volts;
    }
#endif
#if 1
void search_turn(void)
{
    while(Ultra_GetDistance() < 11){
        motor_forward(0,0);
        
        for(int i=1; i < 10; i++){
            Beep(25, i*10);
        }
            MotorDirRight_Write(1);
            MotorDirLeft_Write(1);
            motor_turn(50, 200, 450);
            MotorDirRight_Write(0);
            MotorDirLeft_Write(0);
        }
}
#endif
#if 0
    //do course
void zmain(void *p)
{
    (void) p;
    motor_start();
    motor_forward(0,0);
    int i;
    while(SW1_Read() == 1) { // loop while button isn't pressed
        vTaskDelay(10);
    }
    
	Beep(200,100);
	m_forward(200, 1900);
	tank_turn(200,205,0);

	m_forward(200, 1600);
	tank_turn(200,210,0); 

	m_forward(200, 1600);
	tank_turn(100,500,0);
	
	motor_turn(203, 136, 1460);
	m_forward(200, 350);
	motor_forward(0,0);

	vTaskDelay(400);
    
    for (i = 0; i < 15; i++) {
        Beep(50, i*12);
        Beep(50, i*3);
        motor_forward(0,0);
        tank_turn(i*20,300,i % 2);
        Beep(50, i*12);
        Beep(50, i*6);
        motor_forward(0,0);
    }
}
#endif
#if 0
// button
void zmain(void *p)
{
    (void) p; // we don't use this parameter

    while(1) {
        printf("Press button within 5 seconds!\n");
        int i = 50;
        while(i > 0) {
            if(SW1_Read() == 0) {
                break;
            }
            vTaskDelay(100);
            --i;
        }
        if(i > 0) {
            printf("Good work\n");
            while(SW1_Read() == 0) vTaskDelay(10); // wait until button is released
        }
        else {
            printf("You didn't press the button\n");
        }
    }
}
#endif
#if 0
// button
void zmain(void *p)
{
    (void) p; // we don't use this parameter
    printf("\nBoot\n");

    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 
    
    //uint8 button;
    //button = SW1_Read(); // read SW1 on pSoC board
    // SW1_Read() returns zero when button is pressed
    // SW1_Read() returns one when button is not pressed
    
    bool led = false;
    
    for(;;)
    {
        // toggle led state when button is pressed
        if(SW1_Read() == 0) {
            led = !led;
            BatteryLed_Write(led);
            if(led) printf("Led is ON\n");
            else printf("Led is OFF\n");
            Beep(1000, 150);
            while(SW1_Read() == 0) vTaskDelay(10); // wait while button is being pressed
        }        
    }
 }   
#endif
#if 0
//ultrasonic sensor//
void zmain(void *p)
{
    (void) p; // we don't use this parameter
    Ultra_Start();                          // Ultra Sonic Start function
    
    while(1) {
        int d = Ultra_GetDistance();
        //If you want to print out the value  
        printf("distance = %d\r\n", d);
        vTaskDelay(200);
    }
}   
#endif
#if 0
//IR receiver//
void zmain(void *p)
{
    (void) p; // we don't use this parameter
    IR_Start();
    
    printf("\n\nIR test\n");
    
    IR_flush(); // clear IR receive buffer
    printf("Buffer cleared\n");
    
    bool led = false;
    // Toggle led when IR signal is received
    for(;;)
    {
        IR_wait();  // wait for IR command
        led = !led;
        BatteryLed_Write(led);
        if(led) printf("Led is ON\n");
        else printf("Led is OFF\n");
    }    
 }   
#endif
#if 0
//IR receiver//
void zmain(void *p)
{
    (void) p; // we don't use this parameter
    IR_Start();
    
    uint32_t IR_val; 
    
    printf("\n\nIR test\n");
    
    IR_flush(); // clear IR receive buffer
    printf("Buffer cleared\n");
    
    // print received IR pulses and their lengths
    for(;;)
    {
        if(IR_get(&IR_val, portMAX_DELAY)) {
            int l = IR_val & IR_SIGNAL_MASK; // get pulse length
            int b = 0;
            if((IR_val & IR_SIGNAL_HIGH) != 0) b = 1; // get pulse state (0/1)
            printf("%d %d\r\n",b, l);
        }
    }    
 }   
#endif
#if 0
// line follow and skip week4 1
void zmain(void *p)
{
    (void) p;
    motor_start();
    m_forward(0,0);
    reflectance_start();
    struct sensors_ dig;
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000);
    uint8 button = 1;
    uint8 line = 0;
    
     IR_Start();
    printf("\n\nIR test\n");
    IR_flush(); // clear IR receive buffer
    printf("Buffer cleared\n");

    while(SW1_Read() == 0 && button == 1) { // loop while button isn't pressed
        button = 0;
        break;
    }
    for(;;)
    {
        reflectance_digital(&dig);
        printf("line = %d\n", line);
        printf("r3,l3 = %d %d\n", dig.r3, dig.l3);
        if (dig.r3 && dig.l3) { // only go in, if robot is on a line
            line++;
            if(line == 1 || line == 5) { // line == 1 is for the beginning. line == 5 is for the 4h line.
                while(SW1_Read()) { // wait until button is pressed, stay still until that
                    m_forward(0,20);
                }
                vTaskDelay(750);
            }
            if (line == 2){  //line == 2 means the first line
                m_forward(0,0); // here we stop and next we wait for the ir signal
                IR_wait();  
            }
            while(dig.r3 && dig.l3) { // while on a line
                reflectance_digital(&dig);
                m_forward(60, 20); // move forward while on a line
            }
        }
        if ((dig.r1 || dig.l1) && !(dig.r3 && dig.l3)) { // move forward if not on a line
            m_forward(60, 20);
        }
    }
    
}
#endif
#if 0
// line follow and turns toka teht week4 2
    void zmain (void *p)
    {
        (void) p;
        motor_start();
        m_forward(0,0);
        reflectance_start();
        struct sensors_ dig;
        reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000);
        uint8 button = 1;
        uint8 line = 0;
    
        IR_Start();
        printf("\n\nIR test\n");
        IR_flush(); // clear IR receive buffer
        printf("Buffer cleared\n");

        while(SW1_Read() == 0 && button == 1) { // loop while button isn't pressed
            button = 0;
            break;
        }
        for(;;)
        {
        reflectance_digital(&dig);
        printf("line = %d\n", line);
        printf("r3,l3 = %d %d\n", dig.r3, dig.l3);
        if (dig.r3 && dig.l3) { // only go in, if robot is on a line
            line++;
            if(line == 1 || line == 6) { // line == 1 is for the beginning. line == 6 is for the last line.
                while(SW1_Read()) { // wait until button is pressed, stay still until that
                    m_forward(0,20);
                }
               vTaskDelay(750);
            }
            if (line == 2){  //line == 2 means the first line
                m_forward(0,0); // here we stop and next we wait for the ir signal
                IR_wait();  //wait for the ir signal
            }
            
              if(line == 3) { // line == 1 is for the beginning. line == 5 is for the 4h line.
                m_forward(50,400);    
                tank_turn(70, 1250, 1);
            }
              if(line == 4 || line == 5) { // line == 1 is for the beginning. line == 5 is for the 4h line.
                m_forward(50,350);    
                tank_turn(70, 1250, 0);
            }
            while(dig.r3 && dig.l3) { // while on a line
                reflectance_digital(&dig);
                m_forward(60, 20); // move forward while on a line
            }
        }
       reflectance_digital(&dig);
       while ((dig.r1 || dig.l1) && !(dig.r3 && dig.l3)) { // move forward if not on a line
             reflectance_digital(&dig);
        
       while ((dig.l1 && dig.r1) && !(dig.l3 && dig.r3)) {
            m_forward(30,10);
            reflectance_digital(&dig);
        }
       
       while ((dig.l2 && dig.l1) && !(dig.l3 && dig.r3)) {
           motor_turn(20, 80 , 10);
        reflectance_digital(&dig);
        }

       while ((dig.l3 && dig.l2) && (dig.r2==0 && dig.r3==0)) {
           motor_turn(10, 120, 10);
        reflectance_digital(&dig);
        }
       while (dig.l3 && dig.r3==0) {
             motor_turn(0, 150, 10);
            reflectance_digital(&dig);
        }
       while ((dig.r1 && dig.r2) && !(dig.l3 && dig.l2)) {
           motor_turn(80, 20 , 10);
        reflectance_digital(&dig);
        }
        
       while ((dig.r2 && dig.r3) && (dig.l2==0 && dig.l3==0)) {
           motor_turn(120, 10, 10);
        reflectance_digital(&dig);
        }
       while (dig.r3 && dig.l3==0) {
            motor_turn(150 ,0, 10);
            reflectance_digital(&dig);
    }
        }
    }
    }
    

#endif
#if 0
// line follow curve week4 3
void zmain(void *p)
{
    (void) p;
    struct sensors_ dig;
    motor_start();
    m_forward(0,0);
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000);
    
    IR_Start();
    printf("\n\nIR test\n");
    IR_flush(); // clear IR receive buffer
    printf("Buffer cleared\n");
   
    while(SW1_Read() == 1) { // loop while button isn't pressed
        vTaskDelay(10);
    }
    vTaskDelay(750);
    
    reflectance_digital(&dig);
      while (!(dig.r3 && dig.l3)) { // move forward if not on a line
            m_forward(60, 20);
            reflectance_digital(&dig);
        }
        m_forward(0,0);
        IR_wait();
        reflectance_digital(&dig);
        while (dig.l3 && dig.r3) {
            m_forward(30,10);
            reflectance_digital(&dig);
            }

    
    for(;;)
    {
        reflectance_digital(&dig);
        
       while ((dig.l1 && dig.r1) && !(dig.l3 && dig.r3)) {
            m_forward(245,1);
            reflectance_digital(&dig);
        }
       
       while ((dig.l2 && dig.l1) && !(dig.l3 && dig.r3)) {
           motor_turn(100, 255 , 1);
        reflectance_digital(&dig);
        }

       while ((dig.l3 && dig.l2) && (dig.r2==0 && dig.r3==0)) {
           motor_turn(50, 255, 1);
        reflectance_digital(&dig);
        }
       while (dig.l3 && dig.r3==0) {
             motor_turn(20, 255, 1);
            reflectance_digital(&dig);
        }
       while ((dig.r1 && dig.r2) && !(dig.l3 && dig.l2)) {
           motor_turn(255, 100 , 1);
        reflectance_digital(&dig);
        }
        
       while ((dig.r2 && dig.r3) && (dig.l2==0 && dig.l3==0)) {
           motor_turn(255, 50, 1);
        reflectance_digital(&dig);
        }
       while (dig.r3 && dig.l3==0) {
            motor_turn(255 ,20, 1);
            reflectance_digital(&dig);
    }
            
      reflectance_digital(&dig);
      if ((dig.r3 && dig.l2) || (dig.l3 && dig.r2)) { // stop if on line
            motor_stop();
      }
    }
    
}

#endif
#if 0
// follow line project
void zmain(void *p)
{
    (void) p;
    struct sensors_ dig;
    motor_start();
    m_forward(0,0);
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000);
    uint8 line = 0;
    TickType_t start, end;
    RTC_Start(); // start real time clock
    
    RTC_TIME_DATE now, runtime, finish;

    // set current time
    now.Min = 0;
    now.Sec = 0;
    RTC_WriteTime(&now); // write the time to real time clock
    
    IR_Start();
    IR_flush(); // clear IR receive buffer
   
    while(SW1_Read() == 1) { 
        m_forward(0,0);
        vTaskDelay(10);
        
    }
    vTaskDelay(750);
    
    reflectance_digital(&dig);
        while (!(dig.r3 && dig.l3)) { // move forward if not on a line
            m_forward(60, 20);
            reflectance_digital(&dig);
        }
        m_forward(0,0);
        IR_wait();
        /*while(SW1_Read() == 1) { // loop while button isn't pressed
         vTaskDelay(10);
        }*/
        reflectance_digital(&dig);
        while (dig.l3 && dig.r3) {
            m_forward(230,10);
            reflectance_digital(&dig);
        }
        
        RTC_DisableInt(); /* Disable Interrupt of RTC Component */
        now = *RTC_ReadTime(); /* copy the current time to a local variable */
        RTC_EnableInt(); /* Enable Interrupt of RTC Component */
        start = xTaskGetTickCount();
    for(;;){
        
        reflectance_digital(&dig);
        if(dig.r3 && dig.l3)
        {
            line++;
            while(dig.r3 && dig.l3)
            {
                m_forward(245,1);
                reflectance_digital(&dig);
            }
            if(line == 2)
            {
                RTC_DisableInt(); /* Disable Interrupt of RTC Component */
                finish = *RTC_ReadTime(); /* copy the current time to a local variable */
                RTC_EnableInt(); /* Enable Interrupt of RTC Component */
                //print_mqtt("Zumo002/finishtime", "%d:%d.%d", finish.Hour, finish.Min, finish.Sec);
                //print_mqtt("Zumo002/ticks", "%d", );
                //runtime.Hour = finish.Hour - now.Hour;
                //runtime.Min = finish.Min - now.Min;
                //runtime.Sec = finish.Sec - now.Sec;
                end = xTaskGetTickCount();
                int ticktime = end - start;
                print_mqtt("Zumo002/runtime", "%d ticks", ticktime);
                motor_stop();
            }
        }
        reflectance_digital(&dig);
        
         while ((dig.l1 && dig.r1) && (dig.l3==0 && dig.r3==0)) {      //eteenpäin
            m_forward(245,1);
            reflectance_digital(&dig);
        }
    
        while ((dig.l1) && (dig.l3==0 && dig.r3==0 && dig.r1==0)) {      //loiva vasen
            motor_turn(155, 255 , 1);
            reflectance_digital(&dig);
        }
       
        while ((dig.l2 && dig.l1) && (dig.l3==0 && dig.r3==0)) {      //vasen
            motor_turn(100, 255 , 1);
            reflectance_digital(&dig);
        }

        while ((dig.l3 && dig.l2) && (dig.r2==0 && dig.r3==0)) { //jyrkkä vasen
            motor_turn(50, 255, 1);
            reflectance_digital(&dig);
        }
        while (dig.l3 && dig.r3==0) {                            //tosi jyrkkä vasen
            motor_turn(0, 255, 1);
            reflectance_digital(&dig);
        }
    
        while ((dig.r1) && (dig.l3==0 && dig.r3==0 && dig.l1==0)) {      // loiva oikea
            motor_turn(255, 155 , 1);
            reflectance_digital(&dig);
        }
      
        while ((dig.r1 && dig.r2) && (dig.l3==0 && dig.l2==0)) {      //oikea
            motor_turn(255, 100 , 1);
            reflectance_digital(&dig);
        }       
       
        while ((dig.r2 && dig.r3) && (dig.l2==0 && dig.l3==0)) { //jyrkkä oikea
            motor_turn(255, 50, 1);
            reflectance_digital(&dig);
        }        
        while (dig.r3 && dig.l3==0) {                            //tosi jyrkkä oikea
            motor_turn(255 ,0, 1);
            reflectance_digital(&dig);
        }
    }
}
#endif
#if 0
// line follow and do maze of squares 
    void zmain (void *p)
    {
    (void) p;
    motor_start();
    m_forward(0,0);
    reflectance_start();
    struct sensors_ dig;
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000);
    uint8 button = 1;
    uint8 line = 0;
    
     IR_Start();
    printf("\n\nIR test\n");
    IR_flush(); // clear IR receive buffer
    printf("Buffer cleared\n");

    while(SW1_Read() == 0 && button == 1) { // loop while button isn't pressed
        button = 0;
        break;
    }
    for(;;)
    {
        reflectance_digital(&dig);
        printf("line = %d\n", line);
        printf("r3,l3 = %d %d\n", dig.r3, dig.l3);
        if (dig.r3 && dig.l3) { // only go in, if robot is on a line
            line++;
            if(line == 1 || line == 6) { // line == 1 is for the beginning. line == 6 is for the last line.
                while(SW1_Read()) { // wait until button is pressed, stay still until that
                    m_forward(0,20);
                }
               vTaskDelay(750);
            }
            if (line == 2){  //line == 2 means the first line
                m_forward(0,0); // here we stop and next we wait for the ir signal
                IR_wait();  //wait for the ir signal
            }
            
              if(line == 3) { // line == 1 is for the beginning. line == 5 is for the 4h line.
                m_forward(50,400);    
                tank_turn(70, 1250, 1);
            }
              if(line == 4 || line == 5) { // line == 1 is for the beginning. line == 5 is for the 4h line.
                m_forward(50,350);    
                tank_turn(70, 1250, 0);
            }
            while(dig.r3 && dig.l3) { // while on a line
                reflectance_digital(&dig);
                m_forward(60, 20); // move forward while on a line
            }
        }
       reflectance_digital(&dig);
       while ((dig.r1 || dig.l1) && !(dig.r3 && dig.l3)) { // move forward if not on a line
             reflectance_digital(&dig);
        
       while ((dig.l1 && dig.r1) && !(dig.l3 && dig.r3)) {
            m_forward(30,10);
            reflectance_digital(&dig);
        }
       
       while ((dig.l2 && dig.l1) && !(dig.l3 && dig.r3)) {
           motor_turn(20, 80 , 10);
        reflectance_digital(&dig);
        }

       while ((dig.l3 && dig.l2) && (dig.r2==0 && dig.r3==0)) {
           motor_turn(10, 120, 10);
        reflectance_digital(&dig);
        }
       while (dig.l3 && dig.r3==0) {
             motor_turn(0, 150, 10);
            reflectance_digital(&dig);
        }
       while ((dig.r1 && dig.r2) && !(dig.l3 && dig.l2)) {
           motor_turn(80, 20 , 10);
        reflectance_digital(&dig);
        }
        
       while ((dig.r2 && dig.r3) && (dig.l2==0 && dig.l3==0)) {
           motor_turn(120, 10, 10);
        reflectance_digital(&dig);
        }
       while (dig.r3 && dig.l3==0) {
            motor_turn(150 ,0, 10);
            reflectance_digital(&dig);
    }
        }
    }
    }
    
#endif
#if 0
//reflectance// zumo arena paini
void zmain(void *p)
{
    (void) p; // we don't use this parameter
    struct sensors_ dig;
    TickType_t start, end;
    motor_start();
    Ultra_Start();
    m_forward(0,0);
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000); // set center sensor threshold to 11000 and others to 9000
    IR_Start();
    IR_flush();
    vTaskDelay(1000);
    reflectance_digital(&dig);
    while(dig.r3 == 0 && dig.l3 == 0)
    {
        m_forward(60,10);
        reflectance_digital(&dig);
    }
    m_forward(0, 0);
    IR_wait(); 
    m_forward(245, 300);
    start = xTaskGetTickCount();
    for(;;)
    {   
        reflectance_digital(&dig);
 
        if(dig.l3 || dig.l2 || dig.l1) 
        {
            motor_backward(255, 155);
            tank_turn(255,200,0);
        }
        if(dig.r3 || dig.r2 || dig.r1) 
        {
            motor_backward(255, 155);
            tank_turn(255,200,1);
        }
        while (!(dig.l3 || dig.r3 || dig.l2 || dig.r2 || dig.l1 || dig.r1))  
        {
            reflectance_read(&dig); 
            m_forward(245, 1); 
        }
        if(SW1_Read() == 0) {
            end = xTaskGetTickCount();
            int ticks = end - start;
            print_mqtt("Zumo002/wrestletime", "%d ticks", ticks);
            m_forward(0,0);
            motor_stop();
            break;
        }
    }
}
#endif
#if 0
// Name and age
void zmain(void *p)
{
    (void) p; // we don't use this parameter
    char name[32];
    int age;
    
    
    printf("\n\n");
    
    printf("Enter your name: ");
    //fflush(stdout);
    scanf("%s", name);
    printf("Enter your age: ");
    //fflush(stdout);
    scanf("%d", &age);
    
    printf("You are [%s], age = %d\n", name, age);

    while(true)
    {
        BatteryLed_Write(!SW1_Read());
        vTaskDelay(100);
    }
 }   
#endif
#if 0
// S.O.S LED blink
void zmain(void *p)
{
    (void) p; // we don't use this parameter
    
    while(1) {
        if (!SW1_Read()) {
            uint8 i = 0;
            for (i = 0; i < 3; i++) {
                BatteryLed_Write(1);
                vTaskDelay(500);
                BatteryLed_Write(0);
                vTaskDelay(500);
            }
            for (i = 0; i < 3; i++) {
                BatteryLed_Write(1);
                vTaskDelay(1500);
                BatteryLed_Write(0);
                vTaskDelay(500);
            }
            for (i = 0; i < 3; i++) {
                BatteryLed_Write(1);
                vTaskDelay(500);
                BatteryLed_Write(0);
                vTaskDelay(500);
            }
        }
    }
}
#endif 
#if 0
// how old are you + speedtest
void zmain(void *p)
{
    while(true) {
    (void) p;
    
    int age;
    TickType_t start;
    TickType_t end;
    
    start = xTaskGetTickCount();
    printf("\n\n");
    printf("How old are you: ");
    scanf("%d", &age);
    end = xTaskGetTickCount();
    
    uint32 timestamp = 0;
    timestamp = end - start;
    if (timestamp < 3000) {
        if (age <= 21) {
            printf("Super fast dude!\n"); }
        if ( age > 21 && age < 50) {
            printf("Be quick or be dead\n"); }
        else if (age > 50) { printf("Still going strong\n"); }
    }
    if (timestamp > 3000 && timestamp < 5000) {
        if (age <= 21) {
            printf("So mediocre.\n"); }
        if (age > 21 && age < 50) {
            printf("You're so average.\n"); }
        else if (age > 50) { printf("You are doing ok for your age.\n"); }
    }
    if (timestamp > 5000) {
        if (age <= 21) {
            printf("My granny is faster than you!\n"); }
        if (age > 21 && age < 50) {
            printf("Have you been smoking something illegal?\n"); }
        else if (age > 50) {
            printf("Do they still allow you to drive?\n"); }
    }
    }
}
#endif
#if 0
//accelerometer with random turns
void zmain(void *p)
{
    (void) p;
    printf("Accelerometer test...\n");
     if(!LSM303D_Start()){

    printf("LSM303D failed to initialize!!! Program is Ending!!!\n");
     while(1) vTaskDelay(10);

 }
    else
        printf("Device Ok...\n");
    
    int16 HIGH = 12000;
    int16 LOW = -5000;
    struct accData_ data;
    TickType_t start, end;
    
    motor_start();
    m_forward(0,0);
    end = 0;
    
    while(1){
        m_forward(0, 200);
        LSM303D_Read_Acc(&data);
        printf("X = %d\n", data.accX);
        start = xTaskGetTickCount();
	while(data.accX < HIGH && data.accX > LOW)
	{
            LSM303D_Read_Acc(&data);
            printf("X = %d\n", data.accX);
            for (int i = 0;i <= 50;i++) {
                LSM303D_Read_Acc(&data);
                if (!(data.accX < HIGH && data.accX > LOW)) 
                    break;
                m_forward(150, 20);
            }
            if (data.accX < HIGH && data.accX > LOW)
	    {
                if ((end - start) % 5 == 0) {
                    motor_turn(255, 90, 250);
                }
		else if ((end - start - 2) % 3 == 0)
                    motor_turn(90, 255, 250);
            }
            end = xTaskGetTickCount();
    }
    end = xTaskGetTickCount();
    for(int i=1; i < 10; i++){
            Beep(25, i*10);
                             }
    MotorDirLeft_Write(1); MotorDirRight_Write(1);
    
    if ((end - start) % 5 == 0) {
        motor_turn(245, 245, 800);
        tank_turn(250, 200, 1);
    }else //if ((end - start - 2) % 3 == 0)
       motor_turn(245, 245, 800);
        tank_turn(250, 200, 0);
    //}
    }
}
    if(!LSM303D_Start()){
        printf("LSM303D failed to initialize!!! Program is Ending!!!\n");
        vTaskSuspend(NULL);
    }
    else {
        printf("Device Ok...\n");
    }
    
    for(;;)
    {
        LSM303D_Read_Acc(&data);
        printf("%8d %8d %8d\n",data.accX, data.accY, data.accZ);
        vTaskDelay(50);
    }
 }   
#endif    

#if 0
//accel mqtt test
void zmain(void *p) {
    
    (void) p; // we don't use this parameter
    struct accData_ data;
    struct sensors_ ref;
    struct sensors_ dig;
    
    printf("MQTT and sensor test...\n");

    if(!LSM303D_Start()){
        printf("LSM303D failed to initialize!!! Program is Ending!!!\n");
        vTaskSuspend(NULL);
    }
    else {
        printf("Accelerometer Ok...\n");
    }
    
    int ctr = 0;
    reflectance_start();
    for(;;)
    {
        LSM303D_Read_Acc(&data);
        
            printf("Acc: %8d %8d %8d\n",data.accX, data.accY, data.accZ);
            print_mqtt("Zumo002/acc", "%d,%d,%d", data.accX, data.accY, data.accZ);
            reflectance_read(&ref);
            printf("Ref: %8d %8d %8d %8d %8d %8d\n", ref.l3, ref.l2, ref.l1, ref.r1, ref.r2, ref.r3);       
            print_mqtt("Zumo002/ref", "%d,%d,%d,%d,%d,%d", ref.l3, ref.l2, ref.l1, ref.r1, ref.r2, ref.r3);
            reflectance_digital(&dig);
            printf("Dig: %8d %8d %8d %8d %8d %8d\n", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);
            print_mqtt("Zumo002/dig", "%d,%d,%d,%d,%d,%d", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);
            ctr = 0;
            vTaskDelay(1500);
        
        
    }
 }   

#endif

#if 0
// MQTT print time to raspi //ORGINAL
void zmain(void *p) {
    
    (void) p; // we don't use this parameter
    
    RTC_Start(); // start real time clock
    
    RTC_TIME_DATE now;

    // set current time
    now.Hour = 12;
    now.Min = 34;
    now.Sec = 56;
    now.DayOfMonth = 25;
    now.Month = 9;
    now.Year = 2018;
    RTC_WriteTime(&now); // write the time to real time clock

    for(;;)
    {
        if(SW1_Read() == 0) {
            // read the current time
            RTC_DisableInt(); /* Disable Interrupt of RTC Component */
            now = *RTC_ReadTime(); /* copy the current time to a local variable */
            RTC_EnableInt(); /* Enable Interrupt of RTC Component */

            // print the current time
            print_mqtt("Zumo002/timeofday", "%2d:%02d.%02d", now.Hour, now.Min, now.Sec);
            
            // wait until button is released
            while(SW1_Read() == 0) vTaskDelay(50);
        }
        vTaskDelay(50);
    }
 }   
#endif

#if 0
// MQTT print time to raspi week5 1
void zmain(void *p) {
    
    (void) p; // we don't use this parameter
    
    RTC_Start(); // start real time clock
    
    RTC_TIME_DATE now;

    // set current time
    int hour = 12;
    int min = 34;
   /* now.Sec = 56;
    now.DayOfMonth = 25;
    now.Month = 9;
    now.Year = 2018*/
    //RTC_WriteTime(&now); // write the time to real time clock

    printf("Anna tunnit: \n");
    scanf("%d", &hour);
    printf("Anna minuutit: \n");
    scanf("%d", &min);
    now.Hour = hour;
    now.Min = min;
    for(;;)
    {
        if(SW1_Read() == 0) {
            // read the current time
            RTC_DisableInt(); /* Disable Interrupt of RTC Component */
            now = *RTC_ReadTime(); /* copy the current time to a local variable */
            RTC_EnableInt(); /* Enable Interrupt of RTC Component */

            // print the current time
            print_mqtt("Zumo002/timeofday", "%2d:%02d", now.Hour, now.Min);
            
            
            // wait until button is released
            while(SW1_Read() == 0) vTaskDelay(50);
        }
        vTaskDelay(50);
    }
 }   
#endif
#if 0 // week5 2
void m_forward(uint8, uint32);
void tank_turn(uint8, uint32, uint8);
void zmain(void *p)
{
    (void) p;
    ADC_Battery_Start();
    Ultra_Start();
    motor_start();
    motor_forward(0,0);
    uint8 d = 0;
    int r;
    
    while(1) {
        r = rand() % 2;
        d = Ultra_GetDistance();
        //printf("distance = %d\t battery = %f\r\n", d, bat);
        vTaskDelay(100);
        d = Ultra_GetDistance();
        if(d > 90)
            m_forward(150, 0);
        else if(20 < d && d < 91)
            m_forward(90, 0);
        else {
            m_forward(200,0);
            motor_backward(255,300);
            tank_turn(250, 155, r);
            if(r == 0)
                print_mqtt("Zumo002/turn", "Right");
            else
                print_mqtt("Zumo002/turn", "Left");
        }
   }
}
#endif
#if 0
// week5 3
void zmain (void *p)
{
    (void) p;
    motor_start();
    RTC_Start();
    m_forward(0,0);
    reflectance_start();
    struct sensors_ dig;
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000);
    bool inlap = 0;
    TickType_t start, end;
    RTC_TIME_DATE now, later, finish;
    now.Min, later.Min = 0;
    now.Sec, later.Sec = 0;
    RTC_WriteTime(&now);
    int finishMin = 0;
    int finishSec = 0;
     IR_Start();
    printf("\n\nIR test\n");
    IR_flush(); // clear IR receive buffer
    printf("Buffer cleared\n");

    /*while(SW1_Read() == 1) { // loop while button isn't pressed
        vTaskDelay(10);
    }*/
    for(;;)
    {
        reflectance_digital(&dig);

        if (dig.r3 && dig.l3) 
        { // only go in, if robot is on a line
            m_forward(0,0);
            if(inlap) 
            {
                RTC_DisableInt();
                now = *RTC_ReadTime();
                RTC_EnableInt();
                //print_mqtt("Zumo002/now", "%2d:%2d", now.Min, now.Sec);
                //print_mqtt("Zumo002/later", "%2d:%2d", later.Min, later.Sec);
                finishMin = now.Min;
                finishSec = now.Sec;
                end = xTaskGetTickCount();
                end = end - start;
            }
                print_mqtt("Zumo002/lap", "%2d:%2d, %d ticks", finishMin, finishSec, end);
                inlap = false;
            //IR_wait();
            while(SW1_Read() == 1) 
            { // loop while button isn't pressed
                 vTaskDelay(10);
            }
            later.Min = 0; later.Sec = 0;
            RTC_WriteTime(&later);
            inlap = true;
            start = xTaskGetTickCount();
            while(dig.r3 && dig.l3) 
            { // while on a line
                reflectance_digital(&dig);
                m_forward(60, 20); // move forward while on a line
            }
        }
       reflectance_digital(&dig);
  
        while ((dig.l1 && dig.r1) && !(dig.l3 && dig.r3)) {      //eteenpäin
            m_forward(245,1);
            reflectance_digital(&dig);
        }
    
        while ((dig.l1) && !(dig.l3 && dig.r3 && dig.r1)) {      //loiva vasen
            motor_turn(155, 255 , 1);
            reflectance_digital(&dig);
        }
       
        while ((dig.l2 && dig.l1) && !(dig.l3 && dig.r3)) {      //vasen
            motor_turn(100, 255 , 1);
            reflectance_digital(&dig);
        }

        while ((dig.l3 && dig.l2) && (dig.r2==0 && dig.r3==0)) { //jyrkkä vasen
            motor_turn(50, 255, 1);
            reflectance_digital(&dig);
        }
    
        while ((dig.r1) && !(dig.l3 && dig.r3 && dig.l1)) {      // loiva oikea
            motor_turn(255, 155 , 1);
            reflectance_digital(&dig);
        }
      
        while ((dig.r1 && dig.r2) && !(dig.l3 && dig.l2)) {      //oikea
            motor_turn(255, 100 , 1);
            reflectance_digital(&dig);
        }       
       
        while ((dig.r2 && dig.r3) && (dig.l2==0 && dig.l3==0)) { //jyrkkä oikea
            motor_turn(255, 50, 1);
            reflectance_digital(&dig);
        }
        
    }

}
#endif
#if 1
// maze project
void zmain (void *p)
{
    (void) p;
    motor_start();
    Ultra_Start();
    uint8 d = 0;
    RTC_Start();
    m_forward(0,0);
    reflectance_start();
    struct sensors_ dig;
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000);
    TickType_t start, end;
    RTC_TIME_DATE now, later, finish;
    now.Min, later.Min = 0;
    now.Sec, later.Sec = 0;
    RTC_WriteTime(&now);
    int finishMin = 0;
    int finishSec = 0;
    IR_Start();
    printf("\n\nIR test\n");
    IR_flush(); // clear IR receive buffer
    printf("Buffer cleared\n");
    int r, line = 0;
    
    while(SW1_Read() == 1) { 
        m_forward(0,0);
        vTaskDelay(10);
        
    }
    vTaskDelay(750);
    
    reflectance_digital(&dig);
        while (!(dig.r3 && dig.l3)) { // move forward if not on a line
            m_forward(60, 20);
            reflectance_digital(&dig);
        }
        m_forward(0,0);
        IR_wait();
        reflectance_digital(&dig);
        while (dig.l3 && dig.r3) {
            m_forward(100,10);
            reflectance_digital(&dig);
        }
    IR_wait();
    for(;;)
    {
        r = rand()%2;
        reflectance_digital(&dig);
        d = Ultra_GetDistance();
    
        if(dig.r3 && dig.l3)
        {
            while(dig.r3 && dig.l3) 
            {
                reflectance_digital(&dig);
                m_forward(245, 1); // move forward while on a line
            }    
            if(line == 2)
            {
                tank_turn(150, 300, 0);
                line = 3;
                d = Ultra_GetDistance();
                if( d < 20)
                    tank_turn(150, 300, 1);               
            }
            else if(line == 3)
            {
                tank_turn(150, 300, 1);
                line = 2;
                d = Ultra_GetDistance();
                if( d < 20)
                    tank_turn(150, 300, 0);    
            }
            else
                line++;
        }
        d = Ultra_GetDistance();
        if (d < 20)
        {
            if(dig.r3 && dig.l3)        //viivalla
            {
                m_forward(50,200);    
                tank_turn(150, 300, r);
            
                d = Ultra_GetDistance();
                if(d < 20)
                {
                m_forward(50,200);    
                tank_turn(150, 300, r);
                }
            }
            else
            {
                while(dig.r3 == 0 && dig.l3 == 0) 
                { 
                    m_forward(60,10);
                    reflectance_digital(&dig);
                }
                if(dig.r3) {
                    m_forward(50,200);    
                    tank_turn(150, 300, 0);
                    d = Ultra_GetDistance();
                    if(d < 20)                    
                        tank_turn(150, 300, 0); 
                }
                if(dig.l3) {
                    m_forward(50,200);    
                    tank_turn(150, 300, 1);
                    d = Ultra_GetDistance();
                    if(d < 20)                    
                        tank_turn(150, 300, 1); 
                }
            }                 
        }
        reflectance_digital(&dig);
        if (dig.r3==0 && dig.r2==0 && dig.r1==0 && dig.l1==0 && dig.l2==0 && dig.l3==0) // kaikki sensorit pois
        {
            m_forward(50,200);
            while(dig.r3 == 0)
            {
                tank_turn(100, 1, 0);
                reflectance_digital(&dig);
            }
        }  
       
        while ((dig.l1 && dig.r1) && (dig.l3==0 && dig.r3==0)) {      //eteenpäin
            m_forward(245,1);
            reflectance_digital(&dig);
        }
    
        while ((dig.l1) && (dig.l3==0 && dig.r3==0 && dig.r1==0)) {      //loiva vasen
            motor_turn(155, 255 , 1);
            reflectance_digital(&dig);
        }
       
        while ((dig.l2 && dig.l1) && (dig.l3==0 && dig.r3==0)) {      //vasen
            motor_turn(100, 255 , 1);
            reflectance_digital(&dig);
        }

        while ((dig.l3 && dig.l2) && (dig.r2==0 && dig.r3==0)) { //jyrkkä vasen
            motor_turn(50, 255, 1);
            reflectance_digital(&dig);
        }
        while (dig.l3 && dig.r3==0) {                            //tosi jyrkkä vasen
            motor_turn(0, 255, 1);
            reflectance_digital(&dig);
            line = 2;
        }
    
        while ((dig.r1) && (dig.l3==0 && dig.r3==0 && dig.l1==0)) {      // loiva oikea
            motor_turn(255, 155 , 1);
            reflectance_digital(&dig);
        }
      
        while ((dig.r1 && dig.r2) && (dig.l3==0 && dig.l2==0)) {      //oikea
            motor_turn(255, 100 , 1);
            reflectance_digital(&dig);
        }       
       
        while ((dig.r2 && dig.r3) && (dig.l2==0 && dig.l3==0)) { //jyrkkä oikea
            motor_turn(255, 50, 1);
            reflectance_digital(&dig);
        }        
        while (dig.r3 && dig.l3==0) {                            //tosi jyrkkä oikea
            motor_turn(255 ,0, 1);
            reflectance_digital(&dig);
            line = 3;
        }
    }
}
#endif


/* [] END OF FILE */
