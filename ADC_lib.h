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

//////////// ADC_lib.h has all the setup needed to run the ADC_main.c program ////////////

#include <18F25K22.h>            //includes library for PIC18F25K22
#device ADC=10                   //sets number of bits read_adc() should read
#use delay(internal = 64000000)  //sets internal clock

void setup(void)
{   
   SET_TRIS_B(0x00);             //sets Port B as output
   SET_TRIS_C(0x00);             //sets Port C as output
}
