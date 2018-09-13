#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"

//#include "Handler.h"

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "inc/hw_uart.h"
#include "inc/hw_gpio.h"
#include "inc/hw_pwm.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"
#include "driverlib/flash.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/udma.h"
#include "driverlib/pwm.h"
#include "driverlib/ssi.h"
#include "driverlib/systick.h"
#include "inc/tm4c123gh6pm.h"
#include <string.h>

#include "uart1.h"
#include "sensor.h"
#include "PWM_Config.h"
#include "Time.h"

#define MAX_DISTANCE 	40
#define MIN_DISTANCE 	2
#define HEIGHT_VEC 		20
#define MAX_DIS 			20
#define BLOCK					20

#define LED_PERIPH SYSCTL_PERIPH_GPIOF
#define LED_BASE GPIO_PORTF_BASE
#define RED_LED GPIO_PIN_1

#define GPIO_PIN_4_PERIPH SYSCTL_PERIPH_GPIOF

typedef enum {
	STOP,
	FIND,
	SCAN
}Mode_t;
typedef enum {
	PAUSE,
	RUN
}test_t;
typedef enum {
	N,
	W,
	S,
	E,
}Dir_t;
int sensor_R= MAX_DISTANCE,sensor_L= MAX_DISTANCE,sensor_S = MAX_DISTANCE;
extern uint32_t Period;
void PortB_Init();					//Cau hinh GPIO Port F
void GPIOB_Handler(void);
void State_Process();				//Kiem tra trang thai
void check_Direction(uint32_t * count);
test_t g = PAUSE;
float dt;
uint8_t ReadL=0,ReadR=0;
uint32_t countL,countR;
uint32_t x_axis,y_axis;
uint8_t mat[MAX_DIS*MAX_DIS];
int i=0;
int t = 0;
int fStore = 0,fLoad;
Mode_t st = STOP;
Dir_t dr = N;
int flagR,flagL;
volatile uint8_t value=0;
uint8_t state=0;

void GPIOF_Handler()
{
	uint32_t status=0;
	status = GPIOIntStatus(GPIO_PORTF_BASE,false);
	GPIOIntClear(GPIO_PORTF_BASE,status);

	if(status & GPIO_INT_PIN_4)
	{
		//Then there was a GPIO_PIN_4 pin interrupt
		//		if(fStore)
		//		{
		//			st = SCAN;
		//		}
		//		else
		//		{
		//			st = FIND;
		//		}
		st = FIND;
		GPIOPinWrite(LED_BASE,RED_LED, ~RED_LED);
	}
}
void PortF_Init(void)
{
	SysCtlPeripheralEnable(LED_PERIPH);
	SysCtlDelay(3);

	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
	GPIOPadConfigSet(GPIO_PORTF_BASE ,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_FALLING_EDGE);
	GPIOIntRegister(GPIO_PORTF_BASE,&GPIOF_Handler);
	GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);
	IntMasterEnable();
	GPIOPinTypeGPIOOutput(LED_BASE, RED_LED);
}
void PortB_Init()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_2|GPIO_PIN_1, GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_2|GPIO_PIN_1, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPD);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2|GPIO_PIN_1, 0x00);

	GPIOIntDisable(GPIO_PORTB_BASE, GPIO_PIN_2|GPIO_PIN_1);
	GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_2|GPIO_PIN_1);
	GPIOIntTypeSet(GPIO_PORTB_BASE,GPIO_PIN_2|GPIO_PIN_1, GPIO_RISING_EDGE);
	GPIOIntRegister(GPIO_PORTB_BASE, GPIOB_Handler);
	GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_2|GPIO_PIN_1);
	IntEnable(INT_GPIOB);
}
void turnBackLeft(void)
{
	Turn_Left();
	delay(1500);//1500
}
void turnBackRight(void)
{
	Turn_Right();
	delay(1700);//1700
}

void Status(void)
{
	switch(st)
	{
	case SCAN:
		Go_Straight();
//		if((countL >= (x_axis + 3))|(sensor_S <= (BLOCK-2)))
		if((countL >= (x_axis + 3)))
		{
			if((sensor_L >= 17)&(flagL))//kiem tra trai, flagL = true //15
			{
				Stop();
				delay(300);
				turnBackLeft();
				Stop();
				flagL = 0;					//flagL huong queo
				//					if (!fStore)
//				t++;								//t so lan queo
				//					else
				//						t --;
//				st = SCAN;
				sensor_S = MAX_DISTANCE;
				sensor_L = MAX_DISTANCE;
				sensor_R = MAX_DISTANCE;
			}
			else if((sensor_R >= 17)&(!flagL)) //kiem tra phai, flagL = false //15
			{
				Stop();
				delay(300);
				turnBackRight();
				Stop();
				flagL = 1;
				//if (!fStore)
//				t++;								//t so lan queo
				//					else
				//						t --;
//				st = SCAN;
				sensor_S = MAX_DISTANCE;
				sensor_L = MAX_DISTANCE;
				sensor_R = MAX_DISTANCE;
			}
			else if(((sensor_R < 14)&(!flagL))|((sensor_L < 15)&(flagL)))	// End //13
			{
				//Store t, x_axis to FLASH
				//					FlashErase(0xFC000); // erases 16K block
				//					FlashProgram(&t, 0xFC000, sizeof(t)); // saved as
				//					FlashProgram(&x_axis, 0xFC000 + sizeof(t), sizeof(x_axis));
				//					FlashProgram(&fStore, 0xFC000+sizeof(t)+ sizeof(x_axis), sizeof(fStore));
				//					fStore = 1;
				st = STOP;
			}
			countL = 0;
			countR = 0;
			sensor_S = MAX_DISTANCE;
		}
		break;
	case FIND:
		GPIOPinWrite(LED_BASE,RED_LED, ~RED_LED);
		Go_Straight();
		if(sensor_S <= BLOCK)
		{
			x_axis = countL; // canh dai nhat bang so xung banh trai
			if (sensor_R <= BLOCK)
			{
				Stop();
				delay(300);
				flagL = 0;
//				t =1;
				turnBackLeft();
			}
			else if (sensor_L <= BLOCK)
			{
				Stop();
				delay(300);
				flagL = 1;
//				t =1;
				turnBackRight();
			}
			sensor_S = MAX_DISTANCE ; // set tam gia tri de tranh gia tri mac dinh la 0
			st = SCAN;
			countL = 0;
			countR = 0;
		}
		break;
	case STOP:
		//Load
		//			if(!fLoad)
		//			{
		//				//load
		////				memcpy(&t, (const void *)0xFC000, sizeof(int));
		////				memcpy(&x_axis, (const void *)0xFC000, sizeof(uint8_t));
		//				t = 3;
		//				x_axis = 25;
		//				fLoad = 1;
		//				fStore = 1;
		//			}
		Stop();
		GPIOPinWrite(LED_BASE,RED_LED, RED_LED);
		break;
	}
}
//void check_Direction(uint32_t * count)
//{
//	switch(dr)
//	{
//		case S:
//			*count--;
//			break;
//		case N:
//			*count++;
//			break;
//		case W:
//			*count++;
//			break;
//		case E:
//			*count--;
//			break;
//	}
//}
void GPIOB_Handler(void)
{
	uint32_t PortB_Status = 0x00;
	PortB_Status = GPIOIntStatus(GPIO_PORTB_BASE,false);
	if (PortB_Status == GPIO_PIN_1)
	{
		//		check_Direction(&countL);
		countL++;
	}
	if (PortB_Status == GPIO_PIN_2)
	{
		//		check_Direction(&countR);
		countR++;
	}
	GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_1|GPIO_PIN_2);
}

/* 			INT1 INT2 INT3 INT4
 * 			0    1	              STRAIGHT
 * 			1	 0				  BACK
 */
int main()
{
	SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	PortF_Init();
	PortB_Init();
	SysCtlDelay(20);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x0A); // test with INT1 = 1, INT2 = 0, Left

	InitUltraSound1();
	SysCtlDelay(20);
	InitUltraSound2();
	SysCtlDelay(20);
	InitUltraSound3();
	SysCtlDelay(20);

	Config_PWM();
	SysCtlDelay(20);
	initTime();
	SysCtlDelay(20);
	InitConsole();
	IntMasterEnable();

	while(1){

		uint32_t now = micros();
		static uint32_t timer = 0; // Used to keep track of the time
		dt = (float)(now - timer)/ 1e6f ; // Convert to seconds

		sensor_R = DistanceReturn1();
		sensor_L = DistanceReturn2();
		sensor_S = DistanceReturn3();

		if(sensor_R > MAX_DISTANCE){
			sensor_R= MAX_DISTANCE;
		}
		if(sensor_S > MAX_DISTANCE){
			sensor_S=MAX_DISTANCE;
		}
		if(sensor_L > MAX_DISTANCE){
			sensor_L=MAX_DISTANCE;
		}
		if(sensor_R < MIN_DISTANCE){
			sensor_R= MAX_DISTANCE;
		}
		if(sensor_S < MIN_DISTANCE){
			sensor_S=MAX_DISTANCE;
		}
		if(sensor_L < MIN_DISTANCE){
			sensor_L=MAX_DISTANCE;
		}
		UARTprintf("Dis= %d \t %d \t %d \n",sensor_R,sensor_S,sensor_L);
		UARTprintf("Encoder= %d \t %d \t ",countL,countR);
		//		Go_Straight();
		//		g = RUN;
		//		if(sensor_S <= BLOCK){
		//			Stop();
		//			g = PAUSE;
		//		}
		Status();
		//Status
		//		if (sensor_S < 15){
		////			Turn_Left(); // done
		//			Turn_Right(); // done
		//			delay(1000);
		//			Stop();
		//		}
		////		else Go_Straight(); // done

	}
}

