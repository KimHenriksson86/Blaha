#include <stdio.h>
#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "Delay.h"


Hej hopp pille snopp

Hej o hå

  int SensorDiff;
  int Light_Sensor_1;
  int Light_Sensor_2;
  int switchSensor = 1;
  int LightValueRDY = 0;
//========================== Interrupt Handler ================================
  
  void ADC_Handler(){
    
    AT91C_BASE_ADCC->ADC_IDR = (1<<2) | (1<<24); // Disable interrupt
    switchSensor++;
  }
//=========================== Init_ADC ========================================
void Init_Light_Sensor(void){
    
  *AT91C_PMC_PCER1 = (1<<5); // Starta ADC "port"
  AT91C_BASE_ADCC->ADC_MR = (1<<9); // Mode register (to 14 MHz)

  AT91C_BASE_ADCC->ADC_CHER = (1<<2);  // Select channel 2 (to pin 59 / A5 / PA4)
  AT91C_BASE_ADCC->ADC_CHER = (1<<1);  // Select channel 1 (to pin 60 / A6 / PA3)
  
  NVIC_ClearPendingIRQ(ADC_IRQn); // Initialize interrupt
  NVIC_SetPriority(ADC_IRQn, 6);
  NVIC_EnableIRQ(ADC_IRQn);
  

}


//================== measure_Light_Sensor ========================================
// Reads and prints the values from Light sensor 1

  void measure_Ligtht_Sensor(void){

    
    if(switchSensor == 1 || switchSensor > 3  ){
      switchSensor = 1; 
      AT91C_BASE_ADCC->ADC_CHER = (1<<2);  // Select channel 2 (to pin 59 / A5 / PA4)
      AT91C_BASE_ADCC->ADC_CR = (1<<1); // Start convertion
      AT91C_BASE_ADCC->ADC_IER = (1<<2); // Enable Interrupt
    }
    
    if(switchSensor == 2){
      
      Light_Sensor_1 = (int)AT91C_BASE_ADCC->ADC_CDR2;// Get value
      AT91C_BASE_ADCC->ADC_CHER = (1<<1);  // Select channel 1 (to pin 60 / A6 / PA3)
      AT91C_BASE_ADCC->ADC_CR = (1<<1); // Start convertion
      AT91C_BASE_ADCC->ADC_IER = (1<<2);// Enable Interrupt
    }
     
    else if(switchSensor == 3){
      Light_Sensor_2 =(int)AT91C_BASE_ADCC->ADC_CDR1; // Get value
      SensorDiff = Light_Sensor_1 - Light_Sensor_2;
      switchSensor = 1;
      LightValueRDY = 1;
      
      //printf("Värde1 = %d\n",Light_Sensor_1);
      //printf("Värde2 = %d\n",Light_Sensor_2);
      //printf("VärdeDiff = %d\n",SensorDiff);
    }
 }
 
 