#ifndef __MP3PLAYER_H
#define __MP3PLAYER_H
//Mini STM32开发板
//音乐播放器 V1.0
//正点原子@ALIENTEK
//2010/12/13	

#include "stm32f10x.h"
#include "mmc_sd.h"
#include "spi.h"
#include "delay.h"
#include"FAT.h"
#include "VS10XX.h"	
#include<PlayMode.h>


#define VOL_STEP     12		 
#define LED_MP3_ON1       GPIOB->BRR=GPIO_Pin_1
#define LED_MP3_ON2       GPIOB->BRR=GPIO_Pin_11
#define LED_MP3_OFF1      GPIOB->BSRR=GPIO_Pin_1
#define LED_MP3_OFF2      GPIOB->BSRR=GPIO_Pin_11

extern u8 mode;
u8 Play_Music(u8 data);	//播放音乐
u8 Play_Song(u8 *Music_Name,u8 data);//播放一首歌曲
void MP3_Msg_Show(u32 lenth,u16 index,u16 total);
#endif












