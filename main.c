#include "FreeRTOS.h"
#include "task.h"

#include"LED.h"
#include"DIO.h"



#define SwitchRelease 1
#define SwitchOn      2
#define over2sec      3
#define over4sec      4
#define Shorter2sec   5




void Switch_init(void);
void Switch_on(void);
void Switch_over2sec(void);
void Switch_over4sec(void);
void Switch_release(void);
void Switch_Shorter2sec(void);
void Select_Func(uint8_t Func_num);






int main(void)
{

	                /*Example to create task
	                 *
	 * BaseType_t xTaskCreate(    TaskFunction_t pvTaskCode,
		                            const char * const pcName,
		                            configSTACK_DEPTH_TYPE usStackDepth,
		                            void *pvParameters,
		                            UBaseType_t uxPriority,
		                            TaskHandle_t *pxCreatedTask
		                          );*/


	xTaskCreate(Switch_init,"init",100,NULL,1,NULL);

	/*choose which tasks you want to create through the predefine tasks' name*/
	xTaskCreate(Select_Func,"Son",100,over4sec,1,NULL);


	//xTaskCreate(Switch_release,"Son",100,NULL,1,NULL);
	//xTaskCreate(Switch_on,"Son",100,NULL,1,NULL);
	//xTaskCreate(Switch_over2sec,"Son",100,NULL,1,NULL);
	//xTaskCreate(Switch_over4sec,"Son",100,NULL,1,NULL);
	//xTaskCreate(Switch_Shorter2sec,"Son",100,NULL,1,NULL);







	/* Start Scheduler */
	vTaskStartScheduler();

	while(1)
	{

	}




}



/*****************************************************************************************************/
/*****************************************************************************************************/

                       /*init the Dio_Drive FOr one time then go
                               * suspended state*/

void Switch_init(void)
{

	DIO_init();
	vTaskSuspend(NULL);
	while(1);

}

/*****************************************************************************************************/
/*****************************************************************************************************/
                               /*select which tasks to create*/
								   	/*   SwitchRelease 1
										 SwitchOn      2
										 over2sec      3
										 over4sec      4
										 Shorter2sec   5*/
void Select_Func(uint8_t Func_num)
{
	switch(Func_num)
	{
	case 1:
		Switch_release();
		break;
	case 2:
		Switch_on();
		break;
	case 3:
		Switch_over2sec();
		break;
	case 4:
		Switch_over4sec();
		break;
	case 5:
		Switch_Shorter2sec();
		break;


	}
}

/*****************************************************************************************************/
/*****************************************************************************************************/

                 /* wait until you release your hand of the button*/

void Switch_release(void)
{
	static uint8_t flag=0,Pressed_button=0;
	while(1)
	{
		DIO_read(PORT_B,PIN2,& Pressed_button);
		if(Pressed_button)
		{
			flag=1;
			LED_oFF();
		}
		else
		{
			if(flag==1)
				LED_on();
		}
		vTaskDelay(20);
	}
}

/*****************************************************************************************************/
/*****************************************************************************************************/

                        /*  LED ON IF YOU PRESSED THE BUTTOn*/
void Switch_on(void)
{
	uint8_t static Pressed_button;
	while(1)
	{
		DIO_read(PORT_B,PIN2,& Pressed_button);
		if(Pressed_button)
		{
			LED_on();
		}
		else
			LED_oFF();

		vTaskDelay(20);
	}
}

/*****************************************************************************************************/
/*****************************************************************************************************/

                      /*LED WILL BE ON ONLY IF YOU PRESSED MORE THAN 2 SEC*/
void Switch_over2sec(void)
{
	static uint8_t flag=0,Pressed_button=0;
	while(1)
	{
		DIO_read(PORT_B,PIN2,& Pressed_button);
		if(flag==0)
		{
			if(Pressed_button)
			{
				vTaskDelay(2100);
				if(  Pressed_button )
					flag=1;
				else
					flag=0;
			}
		}
		else
		{
			LED_toggle();

		}
		vTaskDelay(400);
	}
}


/*****************************************************************************************************/
/*****************************************************************************************************/

                             /*LED WILL BE ON ONLY IF YOU PRESSED MORE THAN 4 SEC*/
void Switch_over4sec(void)
{
	static uint8_t flag=0,Pressed_button=0;
	while(1)
	{
		if(flag==0)
		{
			DIO_read(PORT_B,PIN2,& Pressed_button);
			if(Pressed_button)
			{
				vTaskDelay(4100);
				if( Pressed_button)
					flag=1;
				else
					flag=0;
			}
		}
		else
		{
			LED_toggle();
		}
		vTaskDelay(100);
	}
}

/*****************************************************************************************************/
/*****************************************************************************************************/

                          /*LED WILL BE OFF ONLY IF YOU PRESSED LESS THAN 2 SEC*/
void Switch_Shorter2sec(void)
{
	static uint8_t Pressed_button=0,count=0;
	while(1)
	{
		DIO_read(PORT_B,PIN2,& Pressed_button);
		if(count==0)
			LED_on();
		if(Pressed_button)
		{
			count++;
		}
		else
		{
			if(count<40 && count>0)
				LED_oFF();
		}
		vTaskDelay(50);
	}
}

