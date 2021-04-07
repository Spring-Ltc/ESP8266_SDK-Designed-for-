
#include "driver/key.h"
#include "driver/WS2812B.h"




void Key_Init(void);

void Key_Interrupt(void);








void Key_Init(void)
{
	//step1:���ùܽ�ΪGPIO����
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U,FUNC_GPIO5);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_SD_DATA3_U,FUNC_GPIO10);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U,FUNC_GPIO13);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U,FUNC_GPIO14);

	//step2:��ֹ�����������Ϊ���빦��
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(5));
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(10));
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(13));
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(14));

	//step3:��ֹ�ܽ��ڲ���������Ϊ��������������******************��һ������û��
	PIN_PULLUP_DIS(PERIPHS_IO_MUX_GPIO5_U);
	PIN_PULLUP_DIS(PERIPHS_IO_MUX_SD_DATA3_U);
	PIN_PULLUP_DIS(PERIPHS_IO_MUX_MTCK_U);
	PIN_PULLUP_DIS(PERIPHS_IO_MUX_MTMS_U);

	//step4:�ر�GPIO�ж�
	ETS_GPIO_INTR_DISABLE();

	//step5������GPIO�жϴ�������
	gpio_pin_intr_state_set(GPIO_ID_PIN(5),GPIO_PIN_INTR_NEGEDGE);//�½��ش���
	gpio_pin_intr_state_set(GPIO_ID_PIN(10),GPIO_PIN_INTR_NEGEDGE);
	gpio_pin_intr_state_set(GPIO_ID_PIN(13),GPIO_PIN_INTR_NEGEDGE);
	gpio_pin_intr_state_set(GPIO_ID_PIN(14),GPIO_PIN_INTR_NEGEDGE);

	//step6��ע���жϴ�����
	ETS_GPIO_INTR_ATTACH(Key_Interrupt,NULL);

	//step7:����ʹ��GPIO�ж�
	ETS_GPIO_INTR_ENABLE();
}


void Key_Interrupt(void)
{
	uint32_t gpio_status;
	gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);//��ȡ�жϼĴ�����״̬
	os_delay_us(2000);//��ʱ����
	if( gpio_status & (1<<5))	//�ж��ǲ���GPIO5�������ж�
	{
			os_printf("\nGPIO5�����жϣ�\n");
	}
	else if( gpio_status & (1<<10))	//�ж��ǲ���GPIO10�������ж�
	{
		os_printf("\nGPIO10�����жϣ�\n");
	}
	else if( gpio_status & (1<<13))	//�ж��ǲ���GPIO13�������ж�
	{
	    	ColorDepth +=10;
	    	if(ColorDepth >= 240) ColorDepth =240;
	    	os_printf("\n\n��ɫ���Ϊ��%d\n\n",ColorDepth);
	}
	else if( gpio_status & (1<<14))	//�ж��ǲ���GPIO14�������ж�
	{
	    	ColorDepth -=10;
	    	if(ColorDepth <= 11) ColorDepth =11;
	    	os_printf("\n\n��ɫ���Ϊ��%d\n\n",ColorDepth);
	}

	//while(!((GPIO_INPUT_GET(GPIO_ID_PIN(5))) && (GPIO_INPUT_GET(GPIO_ID_PIN(10))) && (GPIO_INPUT_GET(GPIO_ID_PIN(13))) && (GPIO_INPUT_GET(GPIO_ID_PIN(14))) ));//���ּ��
	while(!GPIO_INPUT_GET(GPIO_ID_PIN(5)));
	while(!GPIO_INPUT_GET(GPIO_ID_PIN(10)));
	while(!GPIO_INPUT_GET(GPIO_ID_PIN(13)));
	while(!GPIO_INPUT_GET(GPIO_ID_PIN(14)));
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS,gpio_status);//���״̬��־

}
