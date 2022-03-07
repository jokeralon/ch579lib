#include "app_adc.h"


UINT16 abcBuff[40];
volatile UINT8  adclen;
signed short  RoughCalib_Value=0;		// ADC粗调偏差值
	
void app_adc_init()
{
	UINT8 i;
	ADC_InterTSSampInit();
	RoughCalib_Value = ADC_DataCalib_Rough();  // 用于计算ADC内部偏差，记录到变量 RoughCalib_Value中，注意这个变量需要定义为有符号变量
	for(i=0; i<20; i++)
	{
			abcBuff[i] = ADC_ExcutSingleConver() + RoughCalib_Value;      // 连续采样20次
	}
	/* 单通道采样：选择adc通道11做采样，对应 PA7引脚， 带数据校准功能 */   
//	PRINT( "\n2.Single channel sampling...\n");
	GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_Floating);
	ADC_ExtSingleChSampInit( SampleFreq_3_2, ADC_PGA_0 );
	
	/* 
    注意：数据校准包括 粗调和细调：
    ADC_DataCalib_Rough() 是粗调函数，调用前需保证PA5引脚配置浮空输入模式，外部没有电压信号，在ADC初始化之后调用，一般调用一次，除非更改了ADC模式（增益改变）
    ADC_DataCalib_Fine() 是细调函数，对于粗调过的数据进行拟合算法运算，得到更精确数据
*/    
    GPIOA_ModeCfg(GPIO_Pin_5, GPIO_ModeIN_Floating);
    RoughCalib_Value = ADC_DataCalib_Rough();  // 用于计算ADC内部偏差，记录到全局变量 RoughCalib_Value中  
//    PRINT("=%d \n", RoughCalib_Value);    
	
		
}

void app_adc_readData(UINT16 *read)
{
		UINT8 i;
		ADC_ChannelCfg( 12 );
		for(i=0; i<2; i++)
    {
        read[i] = ADC_ExcutSingleConver() + RoughCalib_Value;      // 连续采样20次
        ADC_DataCalib_Fine( &read[i], ADC_PGA_0 );
    }
}


