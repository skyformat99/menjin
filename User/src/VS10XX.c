#include "VS10XX.h"	
#include "delay.h" 	 
#include "spi.h" 	   	 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
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
 
//VS10XX���ò���
//0,�������ƣ�1�����Ƶ�ޣ�2���������ƣ�3�����Ƶ��
//0,henh.1,hfreq.2,lenh.3,lfreq 5,������
u8 VS10XX_ID=0;
u8 VS10XXram[5]={0,0,0,0,254};
									 
	   	   
//��λVS10XX
//ע��,�������������,ϵͳSPIʱ�ӱ��޸ĳ�9M����
void VS_Soft_Reset(void)
{	 
	u8 retry; 	   		   
	SPIx_SetSpeed(SPI_SPEED_256);   	//256��Ƶ ������ 281.25Khz	
	while(VS_DQ==0);					//�ȴ�����
	SPIx_ReadWriteByte(0X00);       	//��������	 
	retry=0; 					
	VS10XX_ID=VS_RD_Reg(SPI_STATUS);//��ȡ״̬�Ĵ���
	VS10XX_ID>>=4;					//�õ�VS10XX��оƬ�ź� 
	
	if(VS10XX_ID==VS1053)VS_WR_Cmd(SPI_MODE,0x0816);//�����λ,��ģʽ
	else VS_WR_Cmd(SPI_MODE,0x0804);				//�����λ,��ģʽ

	while(VS_DQ==0&&retry<200)			//�ȴ�DREQΪ��
	{
		retry++;
		delay_us(50);
	}; 	
	retry=0;
	while(VS_RD_Reg(SPI_CLOCKF)!=0X9800)//�ȴ����óɹ�
	{
		VS_WR_Cmd(SPI_CLOCKF,0X9800);  	//����VS10XX��ʱ��,3.5��Ƶ ,2xADD  
		if(retry++>100)break; 
	}		   				
 	VS_Rst_DecodeTime();				//��λ����ʱ��	    
    //��VS10XX����4���ֽ���Ч���ݣ���������SPI����
 	SPIx_SetSpeed(SPI_SPEED_8);			//8��Ƶ�ٶ�	  
    VS_XDCS_CLR;							//ѡ�����ݴ��� �ǵ�,����һ��Ҫ����0X00
	SPIx_ReadWriteByte(0X0);
	SPIx_ReadWriteByte(0X0);
	SPIx_ReadWriteByte(0X0);
	SPIx_ReadWriteByte(0X0);
	VS_XDCS_SET;							//ȡ�����ݴ���   								    
} 
//Ӳ��λMP3
//����1:��λʧ��
//����0:��λ�ɹ�
u8 VS_HD_Reset(void)
{
	u8 retry=0;
	VS_XDCS_SET;	//ȡ�����ݴ���
	VS_XCS_SET;	//ȡ��ָ���
	VS_RST_CLR;     //VS_RST=0;			    
	delay_ms(100);    
	VS_RST_SET;     //VS_RST=1;    
	while(VS_DQ==0&&retry<200)//�ȴ�DREQΪ��
	{
		retry++;
		delay_us(50);
	}; 	   
	delay_ms(20);	
	if(retry>=200)
          return 1;
	else return 0;	    		 
}

//���Ҳ��� 
void VS_Sine_Test(void)
{											    
	VS_HD_Reset();	 
	VS_WR_Cmd(0x0b,0X2020);	  //��������	 
 	VS_WR_Cmd(SPI_MODE,0x0820);//����VS10XX�Ĳ���ģʽ     
	while(VS_DQ== 0);     //�ȴ�DREQΪ��
	//printf("mode sin:%x\n",VS_RD_Reg(SPI_MODE));
 	//��VS10XX�������Ҳ������0x53 0xef 0x6e n 0x00 0x00 0x00 0x00
 	//����n = 0x24, �趨VS10XX�����������Ҳ���Ƶ��ֵ��������㷽����VS10XX��datasheet
  	SPIx_SetSpeed(SPI_SPEED_64);//���� 
	VS_XDCS_CLR;//ѡ�����ݴ���
	SPIx_ReadWriteByte(0x53);
	SPIx_ReadWriteByte(0xef);
	SPIx_ReadWriteByte(0x6e);
	SPIx_ReadWriteByte(0x24);
	SPIx_ReadWriteByte(0x00);
	SPIx_ReadWriteByte(0x00);
	SPIx_ReadWriteByte(0x00);
	SPIx_ReadWriteByte(0x00);
	delay_ms(200);
	VS_XDCS_SET; 
    //�˳����Ҳ���
    VS_XDCS_CLR;//ѡ�����ݴ���
	SPIx_ReadWriteByte(0x45);
	SPIx_ReadWriteByte(0x78);
	SPIx_ReadWriteByte(0x69);
	SPIx_ReadWriteByte(0x74);
	SPIx_ReadWriteByte(0x00);
	SPIx_ReadWriteByte(0x00);
	SPIx_ReadWriteByte(0x00);
	SPIx_ReadWriteByte(0x00);
	delay_ms(100);
	VS_XDCS_SET;		 

    //�ٴν������Ҳ��Բ�����nֵΪ0x44���������Ҳ���Ƶ������Ϊ�����ֵ
    VS_XDCS_CLR;//ѡ�����ݴ���      
	SPIx_ReadWriteByte(0x53);
	SPIx_ReadWriteByte(0xef);
	SPIx_ReadWriteByte(0x6e);
	SPIx_ReadWriteByte(0x44);
	SPIx_ReadWriteByte(0x00);
	SPIx_ReadWriteByte(0x00);
	SPIx_ReadWriteByte(0x00);
	SPIx_ReadWriteByte(0x00);
	delay_ms(200);
	VS_XDCS_SET;
    //�˳����Ҳ���
    VS_XDCS_CLR;//ѡ�����ݴ���
	SPIx_ReadWriteByte(0x45);
	SPIx_ReadWriteByte(0x78);
	SPIx_ReadWriteByte(0x69);
	SPIx_ReadWriteByte(0x74);
	SPIx_ReadWriteByte(0x00);
	SPIx_ReadWriteByte(0x00);
	SPIx_ReadWriteByte(0x00);
	SPIx_ReadWriteByte(0x00);
	delay_ms(100);
	VS_XDCS_SET;	 
} 	  
//VS10XX ram ���� 	
//����ֵ:
//��1053:0X83FF,����ȫ������OK
//��1003:0x807F,����ȫ������OK																			 
u16 VS_Ram_Test(void)
{		
	u16 RAM_Read;				 
	VS_HD_Reset();     		   
 	VS_WR_Cmd(SPI_MODE,0x0820);		// ����VS10XX��RAM����ģʽ
	while (VS_DQ==0); 				// �ȴ�DREQΪ��	   
	SPIx_SetSpeed(SPI_SPEED_256);   //256��Ƶ�ٶ�	     
	VS_XDCS_CLR;												   	 
	SPIx_ReadWriteByte(0x4d);
	SPIx_ReadWriteByte(0xea);
	SPIx_ReadWriteByte(0x6d);
	SPIx_ReadWriteByte(0x54);
	SPIx_ReadWriteByte(0x00);
	SPIx_ReadWriteByte(0x00);
	SPIx_ReadWriteByte(0x00);
	SPIx_ReadWriteByte(0x00);
	VS_XDCS_SET;	  
	delay_ms(200);  
	RAM_Read=VS_RD_Reg(SPI_HDAT0);
	// ����õ���ֵΪ0x807F���������á�			 
	//printf("SPI_HDAT0:%x\n",RAM_Read);//������ 
	return RAM_Read;
}		   
//��VS10XXд����
//address:�����ַ
//data:��������
void VS_WR_Cmd(u8 address,u16 data)
{  
	u16 retry=0;
    while(VS_DQ==0&&(retry++)<0xfffe)retry++;	//�ȴ�����    
	SPIx_SetSpeed(SPI_SPEED_64);    			//64��Ƶ�ٶ�	     
	VS_XDCS_SET; 
	VS_XCS_CLR;  	   
	SPIx_ReadWriteByte(VS_WRITE_COMMAND);//����VS10XX��д����
	SPIx_ReadWriteByte(address); //��ַ
	SPIx_ReadWriteByte(data>>8); //���͸߰�λ
	SPIx_ReadWriteByte(data);	 //�ڰ�λ
	VS_XCS_SET;               
	SPIx_SetSpeed(SPI_SPEED_8);  //8��Ƶ�ٶ�	 
} 
//��VS10XXд����
void VS_WR_Data(u8 data)
{
	VS_XDCS_CLR;    
	SPIx_ReadWriteByte(data);
	VS_XDCS_SET;      
}         
//��VS10XX�ļĴ���     
//ע�ⲻҪ�ñ��ٶ�ȡ,�����
u16 VS_RD_Reg(u8 address)
{ 
	u16 temp=0; 
	u8 retry=0;
    while(VS_DQ==0&&(retry++)<0XFE);	//�ȴ�����	  
	if(retry>=0XFE)return 0;	 						    
	SPIx_SetSpeed(SPI_SPEED_64);		//64��Ƶ�ٶ�	
	VS_XDCS_SET;       
	VS_XCS_CLR;       
	SPIx_ReadWriteByte(VS_READ_COMMAND);//����VS10XX�Ķ�����
	SPIx_ReadWriteByte(address);        //��ַ
	temp=SPIx_ReadWriteByte(0xff);		//��ȡ���ֽ�
	temp=temp<<8;
	temp+=SPIx_ReadWriteByte(0xff); 	//��ȡ���ֽ�
	VS_XCS_SET;      
	SPIx_SetSpeed(SPI_SPEED_8);			//8��Ƶ�ٶ�	
    return temp; 
}  
//��ȡVS10XX��RAM
u16 VS_RD_Wram(u16 addr) 
{ 			   	  
	VS_WR_Cmd(SPI_WRAMADDR, addr); 
	return VS_RD_Reg(SPI_WRAM);  
} 
  
//FOR WAV HEAD0 :0X7761 HEAD1:0X7665    
//FOR MIDI HEAD0 :other info HEAD1:0X4D54
//FOR WMA HEAD0 :data speed HEAD1:0X574D
//FOR MP3 HEAD0 :data speed HEAD1:ID
//������Ԥ��ֵ
const u16 bitrate[2][16]=
{ 
{0,8,16,24,32,40,48,56,64,80,96,112,128,144,160,0}, 
{0,32,40,48,56,64,80,96,112,128,160,192,224,256,320,0}
};
//����Kbps�Ĵ�С
//�õ�mp3&wma�Ĳ�����
u16 VS_Get_HeadInfo(void)
{
	unsigned int HEAD0;
	unsigned int HEAD1;            
    HEAD0=VS_RD_Reg(SPI_HDAT0); 
    HEAD1=VS_RD_Reg(SPI_HDAT1);
    switch(HEAD1)
    {        
        case 0x7665:return 0;	//WAV��ʽ
        case 0X4D54:			//MIDI��ʽ 
		case 0X4154:			//AAC_ADTS
		case 0X4144:			//AAC_ADIF
		case 0X4D34:			//AAC_MP4/M4A
		case 0X4F67:			//OGG
        case 0X574D:			//WMA��ʽ
        {
			////printf("HEAD0:%d\n",HEAD0);
            HEAD1=HEAD0*2/25;					//�൱��*8/100
            if((HEAD1%10)>5)return HEAD1/10+1;	//��С�����һλ��������
            else return HEAD1/10;
        }
        default://MP3��ʽ
        {
            HEAD1>>=3;
            HEAD1=HEAD1&0x03; 
            if(HEAD1==3)HEAD1=1;
            else HEAD1=0;
            return bitrate[HEAD1][HEAD0>>12];
        }
    } 
}  
//�������ʱ��                          
void VS_Rst_DecodeTime(void)
{
	VS_WR_Cmd(SPI_DECODE_TIME,0x0000);
	VS_WR_Cmd(SPI_DECODE_TIME,0x0000);//��������
}
//�õ�mp3�Ĳ���ʱ��n sec
u16 VS_Get_DecodeTime(void)
{ 
    return VS_RD_Reg(SPI_DECODE_TIME);   
} 	 

 	     		   
//�趨VS10XX���ŵ������͸ߵ��� 
void set10XX(void)
{
    u8 t;
    u16 bass=0; //�ݴ������Ĵ���ֵ
    u16 volt=0; //�ݴ�����ֵ
    u8 vset=0;  //�ݴ�����ֵ 	 
    vset=255-VS10XXram[4];//ȡ��һ��,�õ����ֵ,��ʾ���ı�ʾ 
    volt=vset;
    volt<<=8;
    volt+=vset;//�õ��������ú��С
     //0,henh.1,hfreq.2,lenh.3,lfreq        
    for(t=0;t<4;t++)
    {
        bass<<=4;
        bass+=VS10XXram[t]; 
    }     
	VS_WR_Cmd(SPI_BASS,bass);//BASS   
    VS_WR_Cmd(SPI_VOL,volt); //������ 
} 
//��ʼ��VS10XX��IO��	 
void VS_Init(void)
{
	 
	SPIx_Init();								//��ʼ��SPI 
} 	 











