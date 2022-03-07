#include "app_adc.h"


UINT16 abcBuff[40];
volatile UINT8  adclen;
signed short  RoughCalib_Value=0;		// ADC�ֵ�ƫ��ֵ
	
void app_adc_init()
{
	UINT8 i;
	ADC_InterTSSampInit();
	RoughCalib_Value = ADC_DataCalib_Rough();  // ���ڼ���ADC�ڲ�ƫ���¼������ RoughCalib_Value�У�ע�����������Ҫ����Ϊ�з��ű���
	for(i=0; i<20; i++)
	{
			abcBuff[i] = ADC_ExcutSingleConver() + RoughCalib_Value;      // ��������20��
	}
	/* ��ͨ��������ѡ��adcͨ��11����������Ӧ PA7���ţ� ������У׼���� */   
//	PRINT( "\n2.Single channel sampling...\n");
	GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_Floating);
	ADC_ExtSingleChSampInit( SampleFreq_3_2, ADC_PGA_0 );
	
	/* 
    ע�⣺����У׼���� �ֵ���ϸ����
    ADC_DataCalib_Rough() �Ǵֵ�����������ǰ�豣֤PA5�������ø�������ģʽ���ⲿû�е�ѹ�źţ���ADC��ʼ��֮����ã�һ�����һ�Σ����Ǹ�����ADCģʽ������ı䣩
    ADC_DataCalib_Fine() ��ϸ�����������ڴֵ��������ݽ�������㷨���㣬�õ�����ȷ����
*/    
    GPIOA_ModeCfg(GPIO_Pin_5, GPIO_ModeIN_Floating);
    RoughCalib_Value = ADC_DataCalib_Rough();  // ���ڼ���ADC�ڲ�ƫ���¼��ȫ�ֱ��� RoughCalib_Value��  
//    PRINT("=%d \n", RoughCalib_Value);    
	
		
}

void app_adc_readData(UINT16 *read)
{
		UINT8 i;
		ADC_ChannelCfg( 12 );
		for(i=0; i<2; i++)
    {
        read[i] = ADC_ExcutSingleConver() + RoughCalib_Value;      // ��������20��
        ADC_DataCalib_Fine( &read[i], ADC_PGA_0 );
    }
}


