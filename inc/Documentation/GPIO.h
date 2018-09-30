/*
 * Servo.h
 *
 *  Created on: 17.9. 2018
 *      Author: Zahorack
 */

/* User Read only file */

#ifndef DOCUMENTATION_GPIO_H_
#define DOCUMENTATION_GPIO_H_


/* GPIO pinout list  */


/*
		 _____________________________________________________________________
		|alternate | function  | application   | pin   |   notes			 |
		|==========|===========|===============|=======|=====================|
		|TIM8	   | PWM	   | engine1       | PC9   |wheel            	 | DC brushed drivers
		|__________|___________|_______________|_______|_____________________|
		|TIM8	   | PWM	   | engine2       | PC8   |wheel				 |
		|__________|___________|_______________|_______|_____________________|
		|TIM8	   | PWM	   | engine3       | PC7   |wheel 				 |
		|__________|___________|_______________|_______|_____________________|
		|TIM8	   | PWM	   | engine4       | PC6   |wheel				 |
		|__________|___________|_______________|_______|_____________________|
		|TIM4	   | PWM	   | engine5       | PD12  |wheel				 |
		|__________|___________|_______________|_______|_____________________|
		|TIM4	   | PWM	   | engine6       | PD13  |wheel				 |
		|__________|___________|_______________|_______|_____________________|
		|TIM4	   | PWM	   | engine7       | PD14  |differential		 |
		|__________|___________|_______________|_______|_____________________|
		|		   | OUT	   | direction1    | PE7   |					 |
		|__________|___________|_______________|_______|_____________________|
		|	   	   | OUT	   | direction2    | PE8   |					 |
		|__________|___________|_______________|_______|_____________________|
		|		   | OUT	   | direction3    | PE9   | 					 |
		|__________|___________|_______________|_______|_____________________|
		|		   | OUT	   | direction4    | PE10  |					 |
		|__________|___________|_______________|_______|_____________________|
		|	 	   | OUT	   | direction5    | PE11  |					 |
		|__________|___________|_______________|_______|_____________________|
		|		   | OUT	   | direction6    | PE12  | 					 |
		|__________|___________|_______________|_______|_____________________|
		|		   | OUT	   | direction7    | PE14  | 					 |
		|__________|___________|_______________|_______|_____________________|
		|TIM2	   | PWM	   | servo1        | PA0   |					 | Servo motors
		|__________|___________|_______________|_______|_____________________|
		|TIM2	   | PWM	   | servo2        | PA1   | 					 |
		|__________|___________|_______________|_______|_____________________|
		|		   | OUT	   | step1         | PG2   |					 | Stepper motors
		|__________|___________|_______________|_______|_____________________|
		|		   | OUT	   | step2         | PG3   |					 |
		|__________|___________|_______________|_______|_____________________|
		|	 	   | OUT	   | direction1    | PB10  |					 |
		|__________|___________|_______________|_______|_____________________|
		|		   | OUT	   | direction2    | PB11  | 					 |
		|__________|___________|_______________|_______|_____________________|
		|		   | OUT	   | enable1       | PD0   | 					 |
		|__________|___________|_______________|_______|_____________________|
		|		   | OUT	   | enable2       | PD1   | 					 |
		|__________|___________|_______________|_______|_____________________|
		|USART1	   | OUT	   | TX    	       | PA10  | 					 |
		|__________|___________|_______________|_______|_____________________|
		|USART1	   | IN 	   | RX		       | PA9   | 					 |
		|__________|___________|_______________|_______|_____________________|
		|USART2	   | OUT	   | TX    	       | PD5   | 					 |
		|__________|___________|_______________|_______|_____________________|
		|USART2	   | IN 	   | RX		       | PD6   | 					 |
		|__________|___________|_______________|_______|_____________________|
		|	       | OUT	   | LED1          | PB0   |Green				 |
		|__________|___________|_______________|_______|_____________________|
		|		   | OUT	   | LED2		   | PB7   |Blue				 |
		|__________|___________|_______________|_______|_____________________|
		|		   | OUT	   | LED3 		   | PB14  |Red 				 |
		|__________|___________|_______________|_______|_____________________|


*/
#endif
