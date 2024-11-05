/* ###################################################################
**     Filename    : main.c
**     Project     : FinalProject
**     Processor   : MK64FN1M0VLL12
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2024-05-21, 14:41, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Pins1.h"
#include "CsIO1.h"
#include "IO1.h"
#include "FX1.h"
#include "GI2C1.h"
#include "WAIT1.h"
#include "MCUC1.h"
#include "CI2C1.h"
#include "SM1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include <stdbool.h>
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */
  uint32_t inval = 0;
  uint32_t B1 = 0;
  uint32_t B2 = 0;
  uint32_t B3 = 0;
  bool modeFlag = 0;
  int actLen = 10;
  int actList [10] = {5, 1, 2, 5, 3, 4 , 2, 1, 1, 3};
  int countdown = 0;
  int actIndex = 0;
  int score = 0;

  unsigned char write[512];
  int len;
  LDD_TDeviceData *SM1_DeviceData;
  SM1_DeviceData = SM1_Init(NULL);

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; /*Enable Port A Clock Gate Control*/
//  SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; /*Enable Port B Clock Gate Control*/
  SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; /*Enable Port C Clock Gate Control*/

  PORTA_GPCLR = 0x00030100; /* Configure pins 0  - 3 port A to GPIO */
//  PORTB_GPCLR = 0x000C0100; /* Configure pins 2 and 3 port B to GPIO */
  PORTC_GPCLR = 0x00040100; /* Configure pins 2 port C to GPIO */

  GPIOA_PDDR = 0x00000000; /* Configure for input*/
//  GPIOB_PDDR = 0x00000000; /* Configure for input*/
  GPIOC_PDDR = 0x00000000; /* Configure for input*/

  SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK; // 0x8000000u; Enable ADC0 Clock
  ADC0_CFG1 = 0x0C; // 16bits ADC; Bus Clock
  ADC0_SC1A = 0x1F; // Disable the module, ADCH = 11111

  unsigned short ADC_read16b(void)
  {
  	ADC0_SC1A = 0x00; //Write to SC1A to start conversion from ADC_0
  	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); // Conversion in progress
  	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); // Until conversion complete
  	return ADC0_RA;
  }

  void DisplayAction(int i) {
	  switch (i) {
	  case 1:
		  printf("PRESS IT!\n");
		  len = sprintf(write, "PRESS IT\n");
		  SM1_SendBlock(SM1_DeviceData, &write, len);
		  for (int i = 0; i < 100000; ++i);
		  break;
	  case 2:
		  printf("ROTATE!\n");
		  len = sprintf(write, "ROTATE\n");
		  SM1_SendBlock(SM1_DeviceData, &write, len);
		  for (int i = 0; i < 100000; ++i);
		  break;
	  case 3:
		  printf("POINT UP!\n");
		  len = sprintf(write, "POINT UP\n");
		  SM1_SendBlock(SM1_DeviceData, &write, len);
		  for (int i = 0; i < 100000; ++i);
		  break;
	  case 4:
		  printf("FLICK UP\n");
		  len = sprintf(write, "FLICK UP\n");
		  SM1_SendBlock(SM1_DeviceData, &write, len);
		  for (int i = 0; i < 100000; ++i);
		  break;
	  case 5:
		  printf("TWIST IT\n");
		  len = sprintf(write, "TWIST IT\n");
		  SM1_SendBlock(SM1_DeviceData, &write, len);
		  for (int i = 0; i < 100000; ++i);
		  break;
	  default:
		  printf("Display function broken :(\n");
		  break;
	  }
  }

  uint32_t GetActionValue(int i) {
	  uint32_t input = 0;
	  int16_t accX, accY, accZ;
	  unsigned short data = 0;


	  switch (i) {
	  case 1:
		  input = GPIOC_PDIR & 0x04; // HIT Button
		  return input & 0x4;
		  break;

	  case 2:
		  accX = FX1_GetX();
		  accY = FX1_GetY();
		  accZ = FX1_GetZ();
		  printf("Accelerometer value \tX: %4d\t Y: %4d\t Z: %4d\n", accX, accY, accZ);
		  if (accX > 500) { // Rotate Right;
			  return 1;
		  }
		  else {
			  return 0;
		  }
		  break;

	  case 3:
		  accX = FX1_GetX();
		  accY = FX1_GetY();
		  accZ = FX1_GetZ();
		  printf("Accelerometer value \tX: %4d\t Y: %4d\t Z: %4d\n", accX, accY, accZ);
		  if (accY > 500) { // if point up, check accY  > 0;
			  return 1;
		  }
		  else {
			  return 0;
		  }
		  break;

	  case 4:
		  input = GPIOA_PDIR & 0x2;
//		  printf("%u\n", B3);
	  	  if (input != 0) { // if point up, check accY  > 0;
	  	  	return 0;
	  	  }
	  	  else {
	  	  	return 1;
	  	  }
	  	  break;

	  case 5:
		  data = ADC_read16b();
		  printf("%d\n", data);
		  if (data > 10000) {
			  return 1;
		  }
		  else {
			  return 0;
		  }
		  break;

	  default:
		  printf("INPUT VALUE BROKEN\n");
		  break;
	  }
  }

  enum States {INIT, SETMODE, ACTION, MISS, HIT, OUTPUT} state = INIT;

  void Tick() {
	  // Actions
	  switch (state) {
	  	  case INIT:
	  		  // Do any initialization here
	  		  actIndex = 0;
	  		  score = 0;
	  		  countdown = 0;
	  		  break;

	  	  case SETMODE:
	  		  // Get input from buttons
	  		  inval = GPIOC_PDIR & 0x04; // HIT Button
	  		  B1 = inval & 0x4;
	  		  break;

	  	  case ACTION:
	  		  if (actIndex >= actLen) break;
//	  		  printf("You are on action number: %d\n", actIndex);
//	  		  len = sprintf(write, "ACTION\n");
//	  		  SM1_SendBlock(SM1_DeviceData, &write, len);
	  		  B3 = GetActionValue(actList[actIndex]);
	  		  ++countdown;
	  		  break;

	  	  case HIT:
	  		  ++actIndex;
	  		  if (actIndex < actLen) DisplayAction(actList[actIndex]);
	  		  for (int i = 0; i < 10000; ++i);
	  		  ++score;
	  		  countdown = 0;
//	  		  printf("HIT STATE\n");
	  		  break;

	  	  case MISS:
	  		  countdown = 0;
	  		  ++actIndex;
	  		  if (actIndex < actLen) DisplayAction(actList[actIndex]);
	  		  break;

	  	  case OUTPUT:
	  		  printf("|||||||||||||||||\n");
	  		  printf("YOUR SCORE IS: %d\n", score);
	  		  printf("|||||||||||||||||\n\n");
	  		  len = sprintf(write, "Your Score: %d\n", score);
	  		  SM1_SendBlock(SM1_DeviceData, &write, len);
	  		  for (int i = 0; i < 3000000; ++i);
	  		  countdown = 0;
	  		  actIndex = 0;
	  		  score = 0;
	  		  break;

	  	  default:
	  		  printf("BROKENNNNN\n");
	  		  break;
	  }

	  //Transitions
	  switch(state) {
	  	  case INIT:
	  		  printf("PRESS BUTTON TO START GAME\n");
	  		  len = sprintf(write, "PRESS START\n");
	  		  SM1_SendBlock(SM1_DeviceData, &write, len);
	  		  for (int i = 0; i < 10000; ++i);
			  state = SETMODE;
			  break;

	  	  case SETMODE:
	  		  if (B1 != 0) {
	  			  printf("STARTING GAME\n\n");
	  			  printf("||||||||||||||||||\n\n");
	  			  len = sprintf(write, "STARTING GAME\n");
	  		      SM1_SendBlock(SM1_DeviceData, &write, len);
	  			  for (int i = 0; i < 1000000; ++i);
	  			  DisplayAction(actList[actIndex]);
	  			  modeFlag = FALSE;
	  			  inval = 0;
	  			  B1 = 0;
	  			  state = ACTION;
	  		  }
	  		  else {
//	  			  printf("Waiting...\n");
	  			  state = SETMODE;
	  		  }
	  		  break;

	  	  case ACTION:
	  		  if (countdown == 15) {
	  			  state = MISS;
	  			  printf("MISS\n");
	  			  len = sprintf(write, "MISS\n");
	  			  SM1_SendBlock(SM1_DeviceData, &write, len);
	  			  for (int i = 0; i < 10000; ++i);
	  			  break;
	  		  }
	  		  if (actIndex >= actLen) {
	  			state = OUTPUT;
	  			printf("GAME DONE\n\n");
	  			len = sprintf(write, "GAME DONE\n");
	  			SM1_SendBlock(SM1_DeviceData, &write, len);
	  		    for (int i = 0; i < 10000; ++i);
	  		    break;
	  		  }
	  		  if (B3 != 0) {
	  			  printf("HITTTTTTTTTTTTT\n\n\n");
	  			  len = sprintf(write, "HITTTT\n");
	  			  SM1_SendBlock(SM1_DeviceData, &write, len);
	  			  for (int i = 0; i < 10000; ++i);
	  			  state = HIT;
	  		  }
	  		  else {
	  			  state = ACTION;
	  		  }
	  		  break;

	  	  case HIT:
	  		  state = ACTION;
	  		  break;

	  	  case MISS:
	  		  state = ACTION;
	  		  break;

	  	  case OUTPUT:
	  		  state = INIT;
	  		  break;

	  	  default:
	  		  break;
	  }


  }

  while(1) {
	  Tick();
	  for(int delay = 0; delay < 1000000; delay++); //delay
  }
  /* For example: for(;;) { } */

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
