#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_uart.h"
#include "driverlib/gpio.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/timer.h"
#include "driverlib/pwm.h"
#include "driverlib/adc.h"
#include "driverlib/rom.h"
#include "Time.h"

#define MAX 	SysCtlClockGet() / 1000000UL
#define MAX_DISTANCE 	40

uint32_t srf[3];
int flag[3];
uint32_t _tmp[3],_tick[3],cur;
static	uint32_t ui32Var1;
static	uint32_t ui32Var2;
static	uint32_t ui32Var3;
static	uint32_t ui32Time1=0;
static	uint32_t ui32Time2=0;
static	uint32_t ui32Time3=0;

static	volatile uint32_t ui32Period;
static	int i=0;
static int j=0;
static int k=0;

void GPIOE_Handler(void)
{
	unsigned status;
	status = GPIOIntStatus(GPIO_PORTE_BASE,false);
//	TimerDisable(TIMER2_BASE,TIMER_A);
//	ui32Time3= ui32Period-TimerValueGet(TIMER2_BASE,TIMER_A);
//	k=0;
	cur =  SysTickValueGet();
	if(status & GPIO_PIN_1)
	{
		if(flag[0])
		{
			srf[0] = MAX*(micros() - _tick[0]) + _tmp[0] - cur;
			flag[0] = 0;	
		}
		else
		{
			_tmp[0] = cur;
			_tick[0] = micros();
			flag[0] = 1;
		}
		GPIOIntClear(GPIO_PORTE_BASE,GPIO_PIN_1);
	}
}
void GPIOA_Handler(void)
{
	unsigned status;
	status = GPIOIntStatus(GPIO_PORTA_BASE,false);
//	TimerDisable(TIMER2_BASE,TIMER_A);
//	ui32Time3= ui32Period-TimerValueGet(TIMER2_BASE,TIMER_A);
//	k=0;
	cur =  SysTickValueGet();
	if(status & GPIO_PIN_4)
	{
		if(flag[1])
		{
			srf[1] = MAX*(micros() - _tick[1]) + _tmp[1] - cur;
			flag[1] = 0;	
		}
		else
		{
			_tmp[1] = cur;
			_tick[1] = micros();
			flag[1] = 1;
		}
		GPIOIntClear(GPIO_PORTA_BASE,GPIO_PIN_4);
	}
	else if(status & GPIO_PIN_6)
	{
		if(flag[2])
		{
			srf[2] = MAX*(micros() - _tick[2]) + _tmp[2] - cur;
			flag[2] = 0;	
		}
		else
		{
			_tmp[2] = cur;
			_tick[2] = micros();
			flag[2] = 1;
		}
		GPIOIntClear(GPIO_PORTA_BASE,GPIO_PIN_6);
	}
}
//void TIMER0A_Handler(void)
//{
//TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
//i=0;
//}

//void DetectedISRE1(void)
//{
//	TimerDisable(TIMER0_BASE,TIMER_A);
//	ui32Time1= ui32Period-TimerValueGet(TIMER0_BASE,TIMER_A);
//	i=0;
//	GPIOIntClear(GPIO_PORTA_BASE,GPIO_PIN_6);
//}

void InitUltraSound1(void)
{

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_6);
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5);

	GPIOIntTypeSet(GPIO_PORTA_BASE,GPIO_PIN_6, GPIO_BOTH_EDGES );
	GPIOIntRegister(GPIO_PORTA_BASE,&GPIOA_Handler);
	GPIOIntEnable(GPIO_PORTA_BASE,GPIO_PIN_6);
	
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
//	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
//	ui32Period = (SysCtlClockGet() /60) ;


//	IntEnable(INT_TIMER0A);
//	IntEnable(INT_GPIOA);
//	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
}

int DistanceReturn1(void)
{
	uint8_t ui8Echo;
	ui32Time1 =0;
	//trigger
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_5,0xff);
	SysCtlDelay(SysCtlClockGet()/250000);
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_5,0x00);

//	ui8Echo=GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_6);
//	while (ui8Echo==0)
//	{
//		ui8Echo=GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_6);
//	}
//	i=1;
//	TimerEnable(TIMER0_BASE,TIMER_A);
//	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);
//	GPIOIntTypeSet(GPIO_PORTA_BASE,GPIO_PIN_6, GPIO_FALLING_EDGE);
//	GPIOIntRegister(GPIO_PORTA_BASE,&DetectedISRE1);
//	GPIOIntEnable(GPIO_PORTA_BASE,GPIO_PIN_6);
//	while (i)
//	{
//	}
//	ui32Var1=ui32Time1*0.000425/2;
//	SysCtlDelay(SysCtlClockGet()/80);
	ui32Var2 = (uint32_t)srf[2]*0.000207f;
	return ui32Var2;
}

/************/
//void TIMER1A_Handler(void)
//{
//TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
//j=0;
//}

//void DetectedISRE2(void)
//{
//	TimerDisable(TIMER1_BASE,TIMER_A);
//	ui32Time2= ui32Period-TimerValueGet(TIMER1_BASE,TIMER_A);
//	j=0;
//	GPIOIntClear(GPIO_PORTA_BASE,GPIO_PIN_4);
//}

void InitUltraSound2(void)
{

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_4);
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);

	GPIOIntTypeSet(GPIO_PORTA_BASE,GPIO_PIN_4, GPIO_BOTH_EDGES );
	GPIOIntRegister(GPIO_PORTA_BASE,&GPIOA_Handler);
	GPIOIntEnable(GPIO_PORTA_BASE,GPIO_PIN_4);
	
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
//	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
//	ui32Period = (SysCtlClockGet() /60) ;

//	IntEnable(INT_TIMER1A);
//	IntEnable(INT_GPIOA);
//	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
}

int DistanceReturn2(void)
{
	uint8_t ui8Echo;
	ui32Time2 =0;
	//trigger:PA3
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,0xff);
	SysCtlDelay(SysCtlClockGet()/250000);
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,0x00);
	//echo PA4
//	ui8Echo=GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4);
//	while (ui8Echo==0)
//	{
//		ui8Echo=GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4);
//	}
//	j=1;
//	TimerEnable(TIMER1_BASE,TIMER_A);
//	TimerLoadSet(TIMER1_BASE, TIMER_A, ui32Period -1);
//	GPIOIntTypeSet(GPIO_PORTA_BASE,GPIO_PIN_4, GPIO_FALLING_EDGE);
//	GPIOIntRegister(GPIO_PORTA_BASE,&DetectedISRE2);
//	GPIOIntEnable(GPIO_PORTA_BASE,GPIO_PIN_4);
//	while (j)
//	{
//	}
//	ui32Var2=ui32Time2*0.000425/2;
//	SysCtlDelay(SysCtlClockGet()/80);
	ui32Var1 = (uint32_t)srf[1]*0.000207f;
	return ui32Var1;
}

/***************************************************************/


//void TIMER2A_Handler(void)
//{
//TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
//k=0;
//}


void InitUltraSound3(void)
{

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1);
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2);
	
	GPIOIntTypeSet(GPIO_PORTE_BASE,GPIO_PIN_1, GPIO_BOTH_EDGES );
	GPIOIntRegister(GPIO_PORTE_BASE,&GPIOE_Handler);
	GPIOIntEnable(GPIO_PORTE_BASE,GPIO_PIN_1);
	
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
//	TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);
//	ui32Period = (SysCtlClockGet() /60) ;


//	IntEnable(INT_TIMER2A);
//	IntEnable(INT_GPIOE);
//	TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
}

int DistanceReturn3(void)
{
	uint8_t ui8Echo;
	ui32Time2 =0;
	//trigger:PE3
	GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_2,0xff);
	SysCtlDelay(SysCtlClockGet()/250000);
	GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_2,0x00);
	//echo PA4
//	ui8Echo=GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1);
//	while (ui8Echo==0)
//	{
//		ui8Echo=GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1);
//	}
//	k=1;
//	TimerEnable(TIMER2_BASE,TIMER_A);
//	TimerLoadSet(TIMER2_BASE, TIMER_A, ui32Period -1);
//	GPIOIntTypeSet(GPIO_PORTE_BASE,GPIO_PIN_1, GPIO_FALLING_EDGE);
//	GPIOIntRegister(GPIO_PORTE_BASE,&DetectedISRE3);
//	GPIOIntEnable(GPIO_PORTE_BASE,GPIO_PIN_1);
//	while (k)
//	{
//	}
//	ui32Var3=ui32Time3*0.000425/2;
//	SysCtlDelay(SysCtlClockGet()/80);
	ui32Var3 = (uint32_t)srf[0]*0.000207f;
	return ui32Var3;
}
