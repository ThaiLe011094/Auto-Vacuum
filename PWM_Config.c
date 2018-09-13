#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_pwm.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"

#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/pwm.h"

#define PULSE_RIGHT		72//75
#define PULSE_LEFT		74//70
//////// GLOBAL VARIABLES /////////
 uint32_t Period;
	   uint32_t PWMPulse;
///////////////////////////////////


//void Config_PWM(void)
//{
//
//	//Configure PWM Clock to match system
//	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
//
//	// Enable the peripherals used by this program.
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
//
//	Period= SysCtlClockGet()/20000; //4000
//
//	//Configure PC4,PC5 Pins as PWM
//	GPIOPinConfigure(GPIO_PC4_M0PWM6);
//	GPIOPinConfigure(GPIO_PC5_M0PWM7);
//	GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);
//
//	//Configure PWM Options
//	//PWM_GEN_3 Covers M0PWM6 and M0PWM7
//	PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN|PWM_GEN_MODE_NO_SYNC);
//
//	//Set the Period
//	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, Period);
//
//	// Enable the PWM generator
//	PWMGenEnable(PWM0_BASE, PWM_GEN_3);
//
//
//}

void Config_PWM(void)
{
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1); // Set divider to 32

		SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0); // Enable PWM peripheral
		SysCtlDelay(2); // Insert a few cycles after enabling the peripheral to allow the clock to be fully activated
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC); // Enable GPIOB peripheral


		SysCtlDelay(2); // Insert a few cycles after enabling the peripheral to allow the clock to be fully activated

		// Use alternate function

		GPIOPinConfigure(GPIO_PC4_M0PWM6);
		GPIOPinConfigure(GPIO_PC5_M0PWM7);



		GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 ); // Use pin with PWM peripheral

		// Configure the PWM generator for count down mode with immediate updates to the parameters
		PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);


		// The period is set to 25ms
		Period= 10000;//SysCtlClockGet()/20000; //4000

		PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, Period); // Set the period


		// Start the timers in generator 0 and 1
		PWMGenEnable(PWM0_BASE, PWM_GEN_3);


		//
		//    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, 800 );
		// Enable the outputs
//		PWMOutputState(PWM0_BASE,PWM_OUT_6_BIT | PWM_OUT_7_BIT, true);
}

void Go_Straight(void)
{
	// RIGHT ENGINE
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_6, Period*PULSE_RIGHT/100);//DutyCycle_Red/10-1); //54.5//
	// LEFT ENGINE
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_7, Period*PULSE_LEFT/100);//DutyCycle_Red/10-1); //58//
	PWMOutputState(PWM0_BASE,PWM_OUT_6_BIT | PWM_OUT_7_BIT, true);
//	PWMOutputState(PWM0_BASE,PWM_OUT_7_BIT, true);
//	PWMOutputState(PWM0_BASE,PWM_OUT_6_BIT, true);
}

void Turn_Right(void)
{
	// RIGHT ENGINE
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_6, Period*73.8/100);//DutyCycle_Red/10-1); //54.5
	PWMOutputState(PWM0_BASE,PWM_OUT_6_BIT , false);
	// LEFT ENGINE
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_7, Period*75.57/100);//DutyCycle_Red/10-1); //58
	PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
}

void Turn_Left(void)
{
//	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x02);
	// RIGHT ENGINE
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_6, Period*73.4/100);//DutyCycle_Red/10-1); //54.5
	PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);

	// LEFT ENGINE
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_7, Period*75/100);//DutyCycle_Red/10-1); //58
	PWMOutputState(PWM0_BASE,PWM_OUT_7_BIT, false);
}

void Stop(void)
{
//	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
	PWMOutputState(PWM0_BASE,PWM_OUT_6_BIT , false);
	PWMOutputState(PWM0_BASE,PWM_OUT_7_BIT, false);
}
