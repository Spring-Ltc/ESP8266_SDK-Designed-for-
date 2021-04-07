
#include "driver/key.h"
#include "driver/WS2812B.h"




void Key_Init(void);

void Key_Interrupt(void);








void Key_Init(void)
{
	//step1:设置管脚为GPIO功能
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U,FUNC_GPIO5);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_SD_DATA3_U,FUNC_GPIO10);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U,FUNC_GPIO13);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U,FUNC_GPIO14);

	//step2:禁止输出，即设置为输入功能
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(5));
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(10));
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(13));
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(14));

	//step3:禁止管脚内部上拉，因为外接了上拉电阻的******************这一步可以没有
	PIN_PULLUP_DIS(PERIPHS_IO_MUX_GPIO5_U);
	PIN_PULLUP_DIS(PERIPHS_IO_MUX_SD_DATA3_U);
	PIN_PULLUP_DIS(PERIPHS_IO_MUX_MTCK_U);
	PIN_PULLUP_DIS(PERIPHS_IO_MUX_MTMS_U);

	//step4:关闭GPIO中断
	ETS_GPIO_INTR_DISABLE();

	//step5：配置GPIO中断触发类型
	gpio_pin_intr_state_set(GPIO_ID_PIN(5),GPIO_PIN_INTR_NEGEDGE);//下降沿触发
	gpio_pin_intr_state_set(GPIO_ID_PIN(10),GPIO_PIN_INTR_NEGEDGE);
	gpio_pin_intr_state_set(GPIO_ID_PIN(13),GPIO_PIN_INTR_NEGEDGE);
	gpio_pin_intr_state_set(GPIO_ID_PIN(14),GPIO_PIN_INTR_NEGEDGE);

	//step6：注册中断处理函数
	ETS_GPIO_INTR_ATTACH(Key_Interrupt,NULL);

	//step7:开启使能GPIO中断
	ETS_GPIO_INTR_ENABLE();
}


void Key_Interrupt(void)
{
	uint32_t gpio_status;
	gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);//读取中断寄存器的状态
	os_delay_us(2000);//延时消抖
	if( gpio_status & (1<<5))	//判断是不是GPIO5触发的中断
	{
			os_printf("\nGPIO5触发中断！\n");
	}
	else if( gpio_status & (1<<10))	//判断是不是GPIO10触发的中断
	{
		os_printf("\nGPIO10触发中断！\n");
	}
	else if( gpio_status & (1<<13))	//判断是不是GPIO13触发的中断
	{
	    	ColorDepth +=10;
	    	if(ColorDepth >= 240) ColorDepth =240;
	    	os_printf("\n\n颜色深度为：%d\n\n",ColorDepth);
	}
	else if( gpio_status & (1<<14))	//判断是不是GPIO14触发的中断
	{
	    	ColorDepth -=10;
	    	if(ColorDepth <= 11) ColorDepth =11;
	    	os_printf("\n\n颜色深度为：%d\n\n",ColorDepth);
	}

	//while(!((GPIO_INPUT_GET(GPIO_ID_PIN(5))) && (GPIO_INPUT_GET(GPIO_ID_PIN(10))) && (GPIO_INPUT_GET(GPIO_ID_PIN(13))) && (GPIO_INPUT_GET(GPIO_ID_PIN(14))) ));//松手检测
	while(!GPIO_INPUT_GET(GPIO_ID_PIN(5)));
	while(!GPIO_INPUT_GET(GPIO_ID_PIN(10)));
	while(!GPIO_INPUT_GET(GPIO_ID_PIN(13)));
	while(!GPIO_INPUT_GET(GPIO_ID_PIN(14)));
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS,gpio_status);//清除状态标志

}
