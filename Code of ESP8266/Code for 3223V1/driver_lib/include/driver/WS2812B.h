

#ifndef __WS2812B_H__
#define __WS2812B_H__

#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"

#define Dout_High (GPIO_OUTPUT_SET(GPIO_ID_PIN(12),1))
#define Dout_Low (GPIO_OUTPUT_SET(GPIO_ID_PIN(12),0))

#define LedNumMAX 15	//宏定义 LED的最大数量


#endif


