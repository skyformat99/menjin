/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : stm32f10x_it.c
* Author             : MCD Application Team
* Version            : V1.0
* Date               : 10/08/2007
* Description        : Main Interrupt Service Routines.
*                      This file can be used to describe all the exceptions
*                      subroutines that may occur within user application.
*                      When an interrupt happens, the software will branch
*                      automatically to the corresponding routine.
*                      The following routines are all empty, user can write code
*                      for exceptions handlers and peripherals IRQ interrupts.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
//#include "delay.h"
#include "hardware_conf.h"
#include <string.h>
#include <PlayMode.h>
#include "InitIQS333.h"
#include "oled.h"

extern u16 newmnob;
extern u8  Rev_ok;
extern u16 Temperature,Weight,testvalue;
extern u8  Heat;      // heat sign
extern u8  Warm,EnCool;      // keep warm sign                
extern u8  Warmtemp,WaterTemp;  // keep warm temp valuable
extern u8  CheckTime; 
extern u8  HeatCRCTime;
extern u8  Telecontrol;  //wifi Telecontrol
extern u8  Keycontrol;   // Keyboard operation
extern u8  HeatCRCStart;
extern u16 LightHeatOffTime;
extern u8  WarmOK;
extern u32 WarmTime;
extern vu16 ADC_ConvertedValue[64];
extern u8  adcok;
extern u8  CommRcvBuf[512];
extern uc16 Temp_Table[126];
extern u8  bFlagOK;
extern u8  WaterWeight,CWaterWeight;
extern u8  Factor_Set_Sign;
extern u8  EnSendHubState;
extern u16  TimeSendHubState;
extern u8  Connect_Service;
extern u8  RecvWifiCmd[64];
extern u8  *RecvWifiCmdEnd;
extern u8  cmdmode;
extern u16 DelayNum10ms;
extern u8  WifiConnectok;
extern u16 OffLineTime;
extern u16 Read_RequstTime;
extern u8  Read_Requst;
extern HeatRecord RecordTable[10];
extern u8  RecordNum;
extern u16  StateTime;
extern u8  Finish;
extern u16 FinishTime;
extern u8  StartTime;
extern u8  LastTime[15];
extern u8  RcvDate[30];
extern u8  ModeReadOk;
extern u8  WifiMode;
extern u8  TeaPotHub,nowhubstate;
extern u8  Keyend;
extern u8  App_OnLine;
extern u8  PhoneID[37];
extern u8  Heat;
extern u8  HeatCRC;
extern u8  ledstate; 
extern u8  RelayOn;
extern u32 RelayOnTime;
extern u8  TryConnectWan;
extern u16 TryConnectWanTime;
extern u8  OrderOperState;
extern u8  Weight_Verf_Sign,ActWeightVerf;
extern u8  MachineID[21];
extern uc16 Weight_Table[50];
extern u16 WEIGHT_VERIFY;
extern u8  wtmp,wtmppre;
extern u8  FirstPowerOn;
extern u8  RenewListEn;
extern u8  Mystate;
extern u8  Start_Pure;
extern u16 Pure_Time;

extern u8  TYear;
extern u8  TMon;
extern u8  TDay;
extern u8  THour;
extern u8  TMin;
extern u8  TSec;
extern u8  TWeek;
extern u32 SecondTime;
extern u8  UpdateState;
extern u8  KeyEnable;
extern u8  KeyEvent;
extern u8  Key_end_Telecontrol;
extern u16 RcvokTime;
extern u16 TestHobTime;
extern u8  UmbrellaID[19];
extern u8  PollingOk;
extern u8  PollingTime;
extern u16 dispopendoortime;


extern GetUmb     GetUmbTable[10];
extern u8  GetResultNum;
extern ReturnUmb  ReturnUmbTable[10];
extern u8  ReturnNum;
extern u8 BeeTime;

extern void Rst_Value(void);
extern void Leddisp(u8 tempvalue,u8 state);
extern void RTC_Configuration(void);
extern void DataReturn(u8 num);


u16   ProtectTime=0;
u16  RcvCount=0;
u8   RcvCount1=0;
u16  Datalength=0;
u8   RcvFlag1=0;
u8   RcvFlag=0;
u8   rcv_err=0;
u8   headflag1=0;
u8   dataflag1=0;
u8   dateflag1=0;
u8   lengthsign=0;
u8   RcvSign=0;
u16  slighttime=0;
u8   beepwarm=0;
u16  StartWeightVrefTime=0;
u8   StartWeightVref=0;
u8   lightstate=0;
u16   KeyWaitTime=0;
u8   waterlevelwarm=0;
u8   sixlighttime=0;
u8   WarmWaitTime=0;
u8   TempRenew=0;

u8  CommRcvBuf1[64];
u8  *EndCommRcvBuf1=CommRcvBuf1;
u8  Data_start=0;
u8  USRT_Buffer[7];
u8  i=0;
u8  k=0;
u32 AdcConValue1,AdcConValue2;
extern u16 adcconvertime;
u8  CheckTimeNumber=0;
extern u8  TimeSync;

u16 Key_Warm_Time=0;
u16 Key_60_Time=0;
u8  Key_Read_Buf;
u8  Keychange=0;
u8  Key_Buff[7]={0,0,0,0,0,0,0,};
u8  Key_Buff_Now[7]={0,0,0,0,0,0,0,};
u8  KeyPress[7]={0,0,0,0,0,0,0,};
extern u8  LED_Buff[8];

enum{
	COM_START_BIT,
	COM_D0_BIT,
	COM_D1_BIT,
	COM_D2_BIT,
	COM_D3_BIT,
	COM_D4_BIT,
	COM_D5_BIT,
	COM_D6_BIT,
	COM_D7_BIT,
	COM_STOP_BIT,
};
u8 recvStat = COM_STOP_BIT;
u8 recvData = 0;
u8 RcvCountIO,RcvFlagIO;
extern u8 CommRcvBufIO[16];
extern u8 bFlagOKm;


void KEY_WARM(void);
void KEY_60(void);
void KEY_70(void);
void KEY_80(void);
void KEY_90(void);
void KEY_100(void);
extern u8 I2C1_Write_NBytes(u8 driver_Addr, u8 start_Addr, u8 number_Bytes, u8 *write_Buffer);
extern u8 I2C1_Read_NBytes(u8 driver_Addr, u8 start_Addr, u8 number_Bytes, u8 *read_Buffer);
extern void delay(u16 num);
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

//�� �� ����AscToHex()
//������������ASCIIת��Ϊ16����
u8 AscToHex(u8 aChar){
    if((aChar>=0x30)&&(aChar<=0x39))
        aChar -= 0x30;
    else if((aChar>=0x41)&&(aChar<=0x46))//��д��ĸ
        aChar -= 0x37;
    else if((aChar>=0x61)&&(aChar<=0x66))//Сд��ĸ
        aChar -= 0x57;
    else aChar = 0xff;
    return aChar; 
}


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : NMIException
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMI_Handler(void)
{
}

/*******************************************************************************
* Function Name  : HardFaultException
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : MemManageException
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : BusFaultException
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : UsageFaultException
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : DebugMonitor
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMonitor_Handler(void)
{
}

/*******************************************************************************
* Function Name  : SVCHandler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVC_Handler(void)
{
}

/*******************************************************************************
* Function Name  : PendSVC
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PendSVC_Handler(void)
{
  
}


/*
void TIM6_DAC_IRQHandler(void)
{  
	if(TIM_GetFlagStatus(TIM6, TIM_FLAG_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);	
		 recvStat++;
		if(recvStat == COM_STOP_BIT)
		{
			TIM_Cmd(TIM6, DISABLE);	
                        switch(RcvCountIO)
                        {
                          case 0: 	if(recvData == 0x95)			
                                          {
                                            CommRcvBufIO[0] = recvData;
                                            RcvCountIO++;
                                          }	
                                          else 	
                                            RcvCountIO = 0;	
                                          break; 
                                          
                          case 1: 	if(recvData == 0x97)			
                                          {
                                            CommRcvBufIO[1] = recvData;
                                            RcvCountIO++;
                                          }	
                                          else 	
                                            RcvCountIO = 0;	
                                          break; 
                          
                          default:						
                                          RcvFlagIO = 1;
                                          break;
                        }
                      
                        if(RcvFlagIO)			// receive valid data
                        {
                          RcvFlagIO = 0;
                          CommRcvBufIO[RcvCountIO++] = recvData;
                                                  
                          if(CommRcvBufIO[RcvCountIO-2] == 0x0d && CommRcvBufIO[RcvCountIO-1]== 0x0a)
                          {
                              RcvCountIO = 0;    
                              bFlagOKm=1;

                          }
                          
                          if(RcvCountIO>=15)
                          {
                               RcvCountIO = 0;

                          }			
                       }    
			return;
		}
		if(OI_RXD)
		{
			recvData |= (1 << (recvStat - 1));
		}
		else
		{
			recvData &= ~(1 << (recvStat - 1));
		}	
  }		
}
*/


/*******************************************************************************
* Function Name  : SysTickHandler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None   10ms
*******************************************************************************/
extern unsigned int usart1_lent_old,usart1_lent,usart1_read_en;
extern u8 Usart1_buf[900];
extern unsigned int usart2_lent_old,usart2_lent,usart2_read_en;
extern u8 Usart2_buf[256];
char ms10_num=0;
extern int heart_num;
extern int net_stu;
extern int scroll;
extern int count;
extern int hardwareTime;

extern unsigned char best_use[];

void TIM2_IRQHandler(void)  
{  
//  u8 i;
  
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
  
    nowhubstate=TeaPotHub;
    
    IWDG_ReloadCounter();      /* Reset Watch Dog*/
    //ADC_StartOfConversion(ADC1); 
    
    
    if(dispopendoortime>0)
    {
      dispopendoortime--;
      if(dispopendoortime==0)
        OLED_Clear();
    }
    ms10_num++;
    if(ms10_num>10){
      ms10_num=0;
      if(usart1_lent_old==usart1_lent&&usart1_lent!=0){
        usart1_read_en=1;
        
      }else{
        usart1_lent_old=usart1_lent;
      }
      if(usart2_lent_old==usart2_lent&&usart2_lent!=0){
        usart2_read_en=1;
        
      }else{
        usart2_lent_old=usart2_lent;
      }      
    }
    
    if(TestHobTime>0)
      TestHobTime--;
    if(DelayNum10ms>0)
      DelayNum10ms--; 
    
    if(BeeTime>0)
    {
      BeeTime--;
      if(BeeTime==0)
      {
        DOOR_PWR(0);
        DOOR_PWR1(0);
      }      
    }
    if(net_stu == 1)
    {
      if(heart_num>0)heart_num--;
    }else{
      heart_num = 0;
    }
    count++;
    hardwareTime++;
    
  }
  else  if (TIM_GetITStatus(TIM2,  TIM_IT_CC1) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1); 
  }  
}
void USART1_IRQHandler(void)
{  
  u8 GetData;

  if(USART_GetITStatus(USART1, USART_IT_RXNE)!=RESET)
  {
    USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    GetData=(u8)USART_ReceiveData(USART1)&0xff;
    if(usart1_read_en==0&&usart1_lent<900){
      Usart1_buf[usart1_lent]=GetData;
        usart1_lent++;
    }

  }

  
  if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
  {
    USART_ClearFlag(USART1,USART_FLAG_ORE); //�� SR ��ʵ���������־
    USART_ReceiveData(USART1); //�� DR
  }   
}
void USART2_IRQHandler(void)

{ 
  u8 GetData1;
  if(USART_GetITStatus(USART2, USART_IT_RXNE)!=RESET)
  {
    USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    GetData1=(u8)USART_ReceiveData(USART2)&0xff;
    if(usart2_read_en==0&&usart2_lent<256){
      Usart2_buf[usart2_lent]=GetData1;
        usart2_lent++;
    }    
  }
  
  if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
  {
    USART_ClearFlag(USART2,USART_FLAG_ORE); //�� SR ��ʵ���������־
    USART_ReceiveData(USART2); //�� DR
  }   
}

/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_IRQHandler(void)
{
//  u8 a;

  if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 
    if(dispopendoortime==0)
    {
      scroll++;//ƫ��������
      if(scroll > 256 )scroll=0;//����������ƫ�����͸�λ
      main_OLED_DrawBMP(scroll, 128);  //显示
    }
  }
  else  if (TIM_GetITStatus(TIM3,  TIM_IT_CC1) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1); 
  }
}



/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1_IRQHandler global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/








/*******************************************************************************
* Function Name  : EXTI4_IRQHandler
* Description    : Warm Enable Key
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/





/*******************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : This function handles RTC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/