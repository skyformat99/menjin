#ifndef __VS10XX_H__
#define __VS10XX_H__
#include "stm32f10x.h"	    
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//Mini STM32������
//VS10XX  ��������		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2011/6/18
//�汾��V1.3
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.2
//1,����VS1053��FLAC����֧��.
//2,�޸�GetHeadInfo����.    
//V1.3�޸�
//1,�淶���������ӿ�
//2,����1.2�汾��VS1053ģ�����޷�����WMA,WAV�Ĵ���.
//3,����оƬ�Զ�ʶ��
//////////////////////////////////////////////////////////////////////////////////	 
//VS10XX��XX(ID�б�)
#define	 VS1001 	0
#define	 VS1011 	1
#define	 VS1002 	2
#define	 VS1003 	3
#define	 VS1053 	4
#define	 VS1033 	5
#define	 VS1103 	7
extern u8 VS10XX_ID;	  		  //VS10XX����ʵ�ͺ�
//����					  
#define VS_WRITE_COMMAND 	0x02  //д����
#define VS_READ_COMMAND 	0x03  //������
//VS10XX�Ĵ�������
#define SPI_MODE        	0x00  //ģʽ����   
#define SPI_STATUS      	0x01  //VS10XX��״̬�Ĵ��� 
#define SPI_BASS        	0x02  //�ߵ������� 
#define SPI_CLOCKF      	0x03  //ʱ��Ƶ�ʱ��˼Ĵ��� 
#define SPI_DECODE_TIME 	0x04  //����ʱ�䳤�� 
#define SPI_AUDATA      	0x05  //������Ƶ���� 
#define SPI_WRAM        	0x06  //RAM д/�� 
#define SPI_WRAMADDR    	0x07  //RAM д/������ʼ��ַ 
#define SPI_HDAT0       	0x08  //�����ݱ�ͷ0 
#define SPI_HDAT1       	0x09  //�����ݱ�ͷ1	 
   
#define SPI_AIADDR      	0x0a  //Ӧ�ó������ʼ��ַ 
#define SPI_VOL         	0x0b  //�������� 
#define SPI_AICTRL0     	0x0c  //Ӧ�ó�����ƼĴ���0 
#define SPI_AICTRL1     	0x0d  //Ӧ�ó�����ƼĴ���1 
#define SPI_AICTRL2     	0x0e  //Ӧ�ó�����ƼĴ���2 
#define SPI_AICTRL3     	0x0f  //Ӧ�ó�����ƼĴ���3
//SPI_MODE�ĸ�λ����,�±�����VS1053���ֲ��ע
#define SM_DIFF         	0x01  //��� 
#define SM_LAYER12         	0x02  //����MPEG 1,2���� FOR vs1053 
#define SM_RESET        	0x04  //������λ 
#define SM_CANCEL       	0x08  //ȡ����ǰ���� 
#define SM_EARSPEAKER_LO  	0x10  //EarSpeaker���趨 
#define SM_TESTS        	0x20  //����SDI���� 
#define SM_STREAM       	0x40  //��ģʽ 
#define SM_EARSPEAKER_HI   	0x80  //EarSpeaker���趨  
#define SM_DACT         	0x100 //DCLK����Ч����  
#define SM_SDIORD       	0x200 //SDIλ˳��  
#define SM_SDISHARE     	0x400 //����SPIƬѡ  
#define SM_SDINEW       	0x800 //VS1002 ����SPIģʽ  
#define SM_ADPCM        	0x1000//ADPCM¼������   
#define SM_LINE1         	0x4000//��/��·1 ѡ�� 		 
#define SM_CLK_RANGE     	0x8000//����ʱ�ӷ�Χ 	
	 
//VS10xx���ⲿ�ӿڶ���(δ����MISO,MOSI,SCK)

#define VS_RST_SET       GPIOA->BSRR=GPIO_Pin_12  		//RST
#define VS_RST_CLR       GPIOA->BRR=GPIO_Pin_12

#define VS_DQ           GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)     //DREQ 

#define VS_XCS_SET      GPIOB->BSRR=GPIO_Pin_7    		     //XCS
#define VS_XCS_CLR      GPIOB->BRR=GPIO_Pin_7

#define VS_XDCS_SET     GPIOB->BSRR=GPIO_Pin_6  		     //XDCS 
#define VS_XDCS_CLR     GPIOB->BRR=GPIO_Pin_6 

extern u8 VS10XXram[5];
u16  VS_RD_Reg(u8 address);			//���Ĵ���
void VS_WR_Cmd(u8 address,u16 data);//дָ��
void VS_WR_Data(u8 data);			//д����  
u16  VS_RD_Wram(u16 addr);			//��WRAM	
  
void VS_Init(void);					//��ʼ��VS10XX	  			    
u8   VS_HD_Reset(void);				//Ӳ��λ
void VS_Soft_Reset(void);     		//����λ   
u16  VS_Ram_Test(void);           	//RAM����
void VS_Sine_Test(void);          	//���Ҳ���	  
u16  VS_Get_DecodeTime(void);   	//�õ�����ʱ��
u16  VS_Get_HeadInfo(void);     	//�õ�������
void VS_Rst_DecodeTime(void);		//�������ʱ��
												    
void set10XX(void);        			//����VS10XX 

#endif














			  















