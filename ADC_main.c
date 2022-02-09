// Copyright 2021 Elissa Bou Karam
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//       http://www.apache.org/licenses/LICENSE-2.0
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

/////////////////////////////////////////////////////////////////////////////////////////////////
//  This project aims to read the analog voltage sinusoidal signal as input
//  and output the voltage frequency value to digital ports to show the result
//  as a hex value by turning on the corresponding LEDs.
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <ADC6_lib.h>

void main(void){
   
   const int limit = 100;                           //size of the 2-D array
   unsigned int16 data[limit][2];                   //2-D array to store ADc and time samples
   unsigned int16 time1=0, time2=0;                 //variables to store the time of maxima
   unsigned int1 b=0;                               //boolean (initialized to 0) used to store consecutive maxima in different variables
   float freq, period;                              //variable to store the value of the frequency and the final period
   unsigned int8 whole, dec, i=0, j=0;              //whole=integer part of frequency ; dec=decimal part of frequency; i=iterator
   unsigned int16 period_int, time_diff;            //time_diff to store time difference between 2 consecutive peaks       //period_int for debugging
   unsigned int16 time_diff_sum=0, periods_count=0; //time_diff_sum to store sum of all periods           //time_diffs_count to store number of periods

   setup_timer_0(T0_INTERNAL);                      //setting up timer0 to internal clock (Fosc/4 = 64MHz/4 = 16MHz) (fastest possible speed)
   
   setup_adc_ports(sAN1);                           //setting ADc to read from analog port A1
   setup_adc(ADC_CLOCK_DIV_32 | ADC_TAD_MUL_0);     //setting ADc clock to internal clock/64 = 1us (fastest acceptable speed) and the TAD to be minimum
   set_adc_channel(1);                              //using ADc channel 1
   delay_us(10);                                    //give time for ADC to initialize before starting to sample
   
   
   while(TRUE){                                     //infinite loop
      set_timer0(0);                                //initializing timer0 to start from 0 at the beginning of every while loop to avoid overflow
      for(i=0; i<limit; i++){                       //1st value is data[0][0] ; last value is data[99][1]
         data[i][0]=(read_adc());                   //ADC samples of input signals are stored in the 1st column of the array
         data[i][1]=(get_timer0());                 //time of ADC samples (value in cycles) stored in 2nd column of the array
      }
      
      ///////////////////////////////////////array method////////////////////////////////////////
      // Iterate through the 2-D array to find ADc maxima then save time of ADc sample maximum //
      // in different variables by using boolean b to switch between saving in time1 &  saving //
      // in time2. The time difference between the consecutive maxima is calculated and        //
      // incremented to a sum. The peaks are counted and the period calculated is the average  //
      // of all periods found (sum of time differences / number of peaks)                      //
      ///////////////////////////////////////////////////////////////////////////////////////////
      //resetting the following variables allows us to restart the calculation whithout being affected by results of previous while loop
      b=0;
      j=0;
      time1=0;
      time2=0;
      time_diff_sum=0;
      periods_count=0;
      for(i=1;i<limit-1;i++){                       //bounds are set to stay in 2-D array size
         if( (data[i][0]>data[i-1][0] && data[i][0]>data[i+1][0]) || (data[i][0]==data[i-1][0] && data[i][0]>data[i+1]) ){
                                                    //1st test is (data[1][0]>data[0][0]) ; last test is (data[98][0]>data[99][0])
                                                    //the OR options are to avoid missing peaks where the ADc reads 2 consecutive equal values close to the peak
            if(b==0){                               //test to store 1st consecutive value in time1
               time1=(data[i][1]);                  //time of 1st consecutive ADc maximum (value in cycles)
               b=1;                                 //changing b to 1 so that we enter the else for 2nd consecutive ADc max
               if(time2!=0){                        //finding time difference between maximum (time1) and previous maximum (time2)(from a previous value of i)
                  time_diff = time1 - time2;        //storing time difference in variable time_diff for comparison later on
                  time_diff_sum += time_diff;       //adding the current period to the sum of all periods
                  periods_count++;                  //counting the periods
               }
            }
            else{                                   //test to store 2nd consecutive value in time2
               time2=(data[i][1]);                  //time of 2nd consecutive ADc maximum (value in cycles)
               b=0;                                 //changing b to 0 so that we enter the if for upcoming consecutive ADc max
               time_diff = time2 - time1;           //storing time difference in variable time_diff for comparison later on
               time_diff_sum += time_diff;          //adding the current period to the sum of all periods
               periods_count++;                     //counting the periods
            }
         }
      }

      period_int = time_diff_sum / periods_count;        //for debugging
      period = (float)(time_diff_sum / periods_count);   //period is the average of all periods
      ///////////////////////////////////////end of array method///////////////////////////////////////
      
      ////////////////////////////////////calculating the frequency////////////////////////////////////      
      //                       period in seconds = [period in cycles]/16000000*2                     //
      //                       freq in Hz = 1/period                                                 //
      //                       freq in kHz = freq in Hz / 1000                                       //
      freq = 16000000/(2000*period);            
      whole = (unsigned int8)(freq);               //whole = integer part of frequency, example: freq=5.1 ; whole=5
      dec = (unsigned int8)(( freq - (float)whole )*100);  //dec = decimal part of frequency, example: freq=5.1 ; dec=10
      
      ////////////////////////////////showing values on PORTs B and C//////////////////////////////////
      ////whole and dec are unsigned int8 since output is on 8-bit Port (using 8 LEDs)
      output_B(whole);                             //show whole value on Port B
      output_C(dec);                               //show decimal value on Port C
   }
}
