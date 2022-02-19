/******************************************************************************
* Copyright (C) 2017, Huada Semiconductor Co.,Ltd All rights reserved.
*
* This software is owned and published by:
* Huada Semiconductor Co.,Ltd ("HDSC").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with HDSC
* components. This software is licensed by HDSC to be adapted only
* for use in systems utilizing HDSC components. HDSC shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. HDSC is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* Disclaimer:
* HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS),
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
* WARRANTY OF NONINFRINGEMENT.
* HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
* SAVINGS OR PROFITS,
* EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
* FROM, THE SOFTWARE.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/
/******************************************************************************/
/** \file main.c
 **
 ** A detailed description is available at
 ** @link Sample Group Some description @endlink
 **
 **   - 2017-05-28 LiuHL    First Version
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "adc.h"
#include "gpio.h"
#include "bgr.h"
#include "uart.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')                            
 ******************************************************************************/


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
volatile uint32_t u32AdcRestult0;
volatile uint32_t u32AdcRestult0_1;
volatile uint32_t u32AdcRestult2;
volatile uint32_t u32AdcRestult5;
volatile uint32_t u32AdcRestult16;
/******************************************************************************
 * Local type definitions ('typedef')                                         
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
void App_AdcPortInit(void);
void App_AdcInit(void);
void App_AdcJqrCfg(void);

static void LedInit(void);

 void App_UartCfg(void);
// void App_DmaCfg(void);
 void App_UartPortInit(void);
/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** This sample
 **
 ******************************************************************************/
int32_t main(void)
{   
	LedInit();
	    //uart端口配置
    App_UartPortInit();
    
    //UART模块配置
    App_UartCfg();
	
    ///< ADC 采样端口初始化
    App_AdcPortInit();    
    
    ///< ADC模块初始化
    App_AdcInit();
    
    ///< ADC插队扫描功能配置
    App_AdcJqrCfg();
	  
    
    while(1)
    {      
			
//			u32AdcRestult16   = Adc_GetJqrResult(AdcJQRCH2MUX);
			printf("\r\n ADCData\r\n");
//			printf("\r\n ADCData = %d\r\n",u32AdcRestult16);
			delay1ms(2000);
			  Adc_JQR_Start();
//			  Adc_ClrIrqStatus(AdcMskIrqJqr);
//			  u32AdcRestult16   = Adc_GetJqrResult(AdcJQRCH3MUX);
//        printf("\r\n Data = %d \r\n",u32AdcRestult16);
//			Adc_JQR_Stop();
    }
}

static void LedInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    ///< 打开GPIO外设时钟门控
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); 
    
    ///< 端口方向配置->输出(其它参数与以上（输入）配置参数一致)
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口上下拉配置->下拉
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LED关闭
//    Gpio_ClrIO(GpioPortB, GpioPin2);
     Gpio_SetIO(GpioPortB, GpioPin2);
    ///< GPIO IO LED端口初始化
    Gpio_Init(GpioPortB, GpioPin2, &stcGpioCfg);
    Gpio_SetIO(GpioPortB, GpioPin2);
}
//UART模块配置
void App_UartCfg(void)
{
    stc_uart_cfg_t  stcCfg;
    stc_uart_baud_t stcBaud;

    DDL_ZERO_STRUCT(stcCfg);                               //初始化变量
    DDL_ZERO_STRUCT(stcBaud);                              //初始化变量
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralUart1,TRUE);//使能UART1外设时钟门控开关

    stcCfg.enRunMode = UartMskMode3;                       //模式3
    stcCfg.enStopBit = UartMsk1bit;                        //1位停止位
    stcCfg.enMmdorCk = UartMskDataOrAddr;                        //偶校验
    stcCfg.stcBaud.u32Baud = 9600;                         //波特率9600
    stcCfg.stcBaud.enClkDiv = UartMsk8Or16Div;             //通道采样分频配置
    stcCfg.stcBaud.u32Pclk = Sysctrl_GetPClkFreq();        //获得外设时钟（PCLK）频率值
    Uart_Init(M0P_UART1, &stcCfg);                         //串口初始化

    Uart_ClrStatus(M0P_UART1,UartRC);                      //清接收请求
    Uart_ClrStatus(M0P_UART1,UartTC);                      //清发送请求
        
    Uart_EnableFunc(M0P_UART1,UartDmaTxFunc);              //使能DMA发送, DMA相关通道使能后，如果Tx Buff为空，会立马启动传输
    Uart_EnableFunc(M0P_UART1,UartDmaRxFunc);              //使能DMA接收
}
//uart端口配置
void App_UartPortInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    DDL_ZERO_STRUCT(stcGpioCfg);                               //初始化变量
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);     //使能GPIO外设时钟门控开关
    
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(GpioPortA,GpioPin2,&stcGpioCfg);
    Gpio_SetAfMode(GpioPortA,GpioPin2,GpioAf1);                //PA02 配置为UART1 TX
    stcGpioCfg.enDir = GpioDirIn;
    Gpio_Init(GpioPortA,GpioPin3,&stcGpioCfg);
    Gpio_SetAfMode(GpioPortA,GpioPin3,GpioAf1);                //PA03 配置为UART1 RX
}

 ///< ADC中断服务函数
void Adc_IRQHandler(void)
{    
    if(TRUE == Adc_GetIrqStatus(AdcMskIrqJqr))
    {
        Adc_ClrIrqStatus(AdcMskIrqJqr);
        
//        u32AdcRestult0   = Adc_GetJqrResult(AdcJQRCH0MUX);
//        u32AdcRestult2   = Adc_GetJqrResult(AdcJQRCH1MUX);
//        u32AdcRestult5   = Adc_GetJqrResult(AdcJQRCH2MUX);

			
			  u32AdcRestult16   = Adc_GetJqrResult(AdcJQRCH1MUX);
//			  u32AdcRestult0_1 = Adc_GetJqrResult(AdcJQRCH3MUX);
        printf("\r\n Data111 = %d \r\n",u32AdcRestult16);
        Adc_JQR_Stop();
    }
}

///< ADC采样端口初始化
void App_AdcPortInit(void)
{    
    ///< 开启ADC/BGR GPIO外设时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    
//    Gpio_SetAnalogMode(GpioPortA, GpioPin0);        //PA00 (AIN0)
    Gpio_SetAnalogMode(GpioPortB, GpioPin0);        //PA02 (AIN2)  PB02 (AIN16)
//    Gpio_SetAnalogMode(GpioPortA, GpioPin5);        //PA05 (AIN5)
}

///< ADC模块初始化
void App_AdcInit(void)
{
    stc_adc_cfg_t              stcAdcCfg;

    DDL_ZERO_STRUCT(stcAdcCfg);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, TRUE); 
    Bgr_BgrEnable();        ///< 开启BGR
    ///< ADC 初始化配置
    stcAdcCfg.enAdcMode         = AdcScanMode;              ///<采样模式-扫描
    stcAdcCfg.enAdcClkDiv       = AdcMskClkDiv1;            ///<采样分频-1
    stcAdcCfg.enAdcSampCycleSel = AdcMskSampCycle8Clk;      ///<采样周期数-8
    stcAdcCfg.enAdcRefVolSel    = AdcMskRefVolSelAVDD;      ///<参考电压选择-VCC
    stcAdcCfg.enAdcOpBuf        = AdcMskBufDisable;         ///<OP BUF配置-关
    stcAdcCfg.enInRef           = AdcMskInRefDisable;       ///<内部参考电压使能-关
    stcAdcCfg.enAdcAlign        = AdcAlignRight;               ///<转换结果对齐方式-右
    Adc_Init(&stcAdcCfg);
}
///< ADC插队扫描功能配置
void App_AdcJqrCfg(void)
{
    stc_adc_jqr_cfg_t          stcAdcJqrCfg;
    
    DDL_ZERO_STRUCT(stcAdcJqrCfg);
    
    ///< 插队扫描模式功能及通道配置
    ///< 注意：扫描模式下，当配置转换次数为n时，转换通道的配置范围必须为[JQRCH(0)MUX,JQRCH(n-1)MUX]
    stcAdcJqrCfg.bJqrDmaTrig = FALSE;
    stcAdcJqrCfg.u8JqrCnt    = 4;
    Adc_JqrModeCfg(&stcAdcJqrCfg);

//    Adc_CfgJqrChannel(AdcJQRCH0MUX, AdcExInputCH0);
//    Adc_CfgJqrChannel(AdcJQRCH1MUX, AdcExInputCH2);
//    Adc_CfgJqrChannel(AdcJQRCH2MUX, AdcExInputCH5);
//    Adc_CfgJqrChannel(AdcJQRCH3MUX, AdcExInputCH0);
    
     Adc_CfgJqrChannel(AdcJQRCH2MUX, AdcExInputCH8);
    ///< ADC 中断使能
    Adc_EnableIrq();
    EnableNvic(ADC_IRQn, IrqLevel3, TRUE);
    
    ///< 启动插队扫描采样
    Adc_JQR_Start();
}
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
