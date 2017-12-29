#include <PlayMode.h>
#include "FAT.h"

void UART_Send_Missage(u8 *Data)
{
  while(*Data!='\0')
  {
      USART_SendData(USART1, *Data);
      while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
      Data++;
  }
  /*
  USART_SendData(USART1, 0x0D);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  USART_SendData(USART1, 0x0A);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  */
}

void UART2_Send_Missage(u8 *Data)
{
  while(*Data!='\0')
  {
      USART_SendData(USART2, *Data);
      while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
      Data++;
  }
}


/****************读写默认配置flash***********************************************/
int write_flash(u32 StartAddr,u16 *buf,u16 len)
{
        volatile FLASH_Status FLASHStatus;
	u32 FlashAddr;
	len=(len+1)/2;
	FLASH_Unlock();
        FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
	FlashAddr=StartAddr;
	FLASH_ErasePage(StartAddr);
        while(len--)
        {
          FLASHStatus = FLASH_ProgramHalfWord(FlashAddr,*buf++);
          if (FLASHStatus != FLASH_COMPLETE)
          {
            //printf("FLSH :Error %08X\n\r",FLASHStatus);
            return -1;
          }
          FlashAddr += 2;
        }
        
	FLASH_Lock();
	return 0;
}
int read_flash(u32 StartAddr,u32 *buf,u16 len) //一次读一个字 4字节
{
	len=(len+3)/4;
	while(len--){
		 *buf=*(__IO uint32_t *)StartAddr;
                 StartAddr=StartAddr+4;
                 buf++;
	}
        return 0;
}