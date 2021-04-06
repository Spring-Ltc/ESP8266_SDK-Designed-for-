

#include "driver/WS2812B.h"






uint8_t R_buffer[LedNumMAX]= {0};
uint8_t G_buffer[LedNumMAX]= {0};
uint8_t B_buffer[LedNumMAX]= {0};


void WS2812B_IOinit(void);


void WS2812B_Send_24bit(uint8_t R,uint8_t G,uint8_t B);
void WS2812B_Send_8bit(uint8_t dat);

void Send_WS_1(void);
void Send_WS_0(void);

void SetOneLedColor(uint8_t lable,uint32_t color);
void SetAllLedOneColor_LowSpeed(uint32_t color,uint8_t wait);
void SetAllLedOneColor_HighSpeed(uint8_t R,uint8_t G,uint8_t B);
uint32_t ColorCombine(uint8_t R,uint8_t G,uint8_t B);


void ColorArrayUpdate(uint16_t star);
void OutputArrayUpdate(void);


void Mynop(void);
void Mynop(void)
{
	uint16_t time=2500;
	while(time--);
}


void WS2812B_IOinit(void)
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U,FUNC_GPIO12);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(12),0);
}



void WS2812B_Send_24bit(uint8_t R,uint8_t G,uint8_t B)
{
	WS2812B_Send_8bit(R);
	WS2812B_Send_8bit(G);
	WS2812B_Send_8bit(B);
}

void WS2812B_Send_8bit(uint8_t dat)
{
	uint8_t mask=0x80;
	uint8_t temp=dat;
	Dout_Low;//要先把信号线拉低
	ets_intr_lock();	//关闭全局中断，避免时序被打断

	while(mask)
	{
		if(mask & temp)
			Send_WS_1();//发送高电平
		else
			Send_WS_0();//发送低电平
		mask >>=1;
	}

	ets_intr_unlock();	//本次通信结束，开启中断
}


void Send_WS_1(void)
{
	uint16_t time;
	time = 3;while(time--)Dout_High;
	time = 3;while(time--)Dout_Low;
}


void Send_WS_0(void)
{
	uint16_t time;
	time = 1;while(time--)Dout_High;
	time = 3;while(time--)Dout_Low;
}


//设置指定某个LED的颜色：lable为标号，color为颜色
void SetOneLedColor(uint8_t lable,uint32_t color)
{
	uint16_t i;

	//根据颜色拆分RGB值
	R_buffer[lable] = (uint8_t) (color>>16);
	G_buffer[lable] = (uint8_t) (color>>8);
	B_buffer[lable] = (uint8_t) color;

	for(i=0;i<LedNumMAX;i++)
		WS2812B_Send_24bit(R_buffer[i],G_buffer[i],B_buffer[i]);
}

//把所有的LED设置为同一个颜色：color颜色；wait两个灯刷新的时间间隔
void SetAllLedOneColor_LowSpeed(uint32_t color,uint8_t wait)
{
	uint16_t i;
	for(i=0;i<LedNumMAX;i++)
	{
		SetOneLedColor(i,color);
		os_delay_us(wait*1000);
	}
}

void SetAllLedOneColor_HighSpeed(uint8_t R,uint8_t G,uint8_t B)
{
	uint16_t i;
	for(i=0;i<LedNumMAX;i++)//更新存储数组
	{
		R_buffer[i] = R;
		G_buffer[i] = G;
		B_buffer[i] = B;
	}
	for(i=0;i<LedNumMAX;i++)//写入数据
		WS2812B_Send_24bit(R_buffer[i],G_buffer[i],B_buffer[i]);
}


//根据RGB值，合并计算一个颜色color数值
uint32_t ColorCombine(uint8_t R,uint8_t G,uint8_t B)
{
	return ((uint32_t)R<<16) | ((uint32_t)G<<8) | B ;
}



//更新颜色数组
void ColorArrayUpdate(uint16_t star)
{
	uint16_t  i;
	for(i=0;i<LedNumMAX;i++)
	{
		switch (i % 7)
		{
			case 0:
				R_buffer[star] = 0x0a;G_buffer[star] = 0x00;B_buffer[star] = 0x00;break;
			case 1:
				R_buffer[star] = 0x00;G_buffer[star] = 0x0a;B_buffer[star] = 0x00;break;
			case 2:
				R_buffer[star] = 0x00;G_buffer[star] = 0x00;B_buffer[star] = 0x0a;break;
			case 3:
				R_buffer[star] = 0x0a;G_buffer[star] = 0x0a;B_buffer[star] = 0x00;break;
			case 4:
				R_buffer[star] = 0x0a;G_buffer[star] = 0x00;B_buffer[star] = 0x0a;break;
			case 5:
				R_buffer[star] = 0x00;G_buffer[star] = 0x0a;B_buffer[star] = 0x0a;break;
			case 6:
				R_buffer[star] = 0x0a;G_buffer[star] = 0x0a;B_buffer[star] = 0x0a;break;
		}
		star++;
		if(star > LedNumMAX) star =0;
	}

}



void OutputArrayUpdate(void)
{
	uint16_t i;
	for(i=0;i<LedNumMAX;i++)//写入数据
		WS2812B_Send_24bit(R_buffer[i],G_buffer[i],B_buffer[i]);
}

