#include "mp3player.h"
//Mini STM32��������չʵ��10
//MP3������ ʵ��
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//2011.6.18

u16 f_kbps=0;//�����ļ�λ��
u16  newmnob=0;
u16  mnob=0;

extern u8 Rev_ok;
extern u8 USRT_Buffer[7];
extern u8 exit_set;
//��ʾ����ʱ��,������ ������Ϣ 
void MP3_Msg_Show(u32 lenth,u16 index,u16 total)
{	
	static u16 playtime=0;//����ʱ����	     
 	u16 time=0;// ʱ�����
	u16 temp=0;

	if(f_kbps==0xffff)//δ���¹�
	{
		playtime=0;
		f_kbps=VS_Get_HeadInfo();	   //��ñ�����
	}	 	 
	time=VS_Get_DecodeTime(); //�õ�����ʱ��
	if(playtime==0)playtime=time;
	else if((time!=playtime)&&(time!=0))//1sʱ�䵽,������ʾ����
	{
		playtime=time;//����ʱ�� 	 				    
		temp=VS_Get_HeadInfo(); //��ñ�����	   				 
		if(temp!=f_kbps)
		{
			f_kbps=temp;//����KBPS	  				     
		}			 
		//��ʾ����ʱ��			 
		//LCD_Show2Num(60,110,time/60,2,16,0);   //����
		//LCD_ShowChar(60+16,110,':',16,0);
		//LCD_Show2Num(60+24,110,time%60,2,16,0);//����
		//LCD_ShowChar(60+40,110,'/',16,0);	    	 
		//��ʾ��ʱ��
		//if(f_kbps)time=(lenth/f_kbps)/125;//�õ�������   (�ļ�����(�ֽ�)/(1000/8)/������=����������    	  
		//else time=0;//�Ƿ�λ��	  
 		//LCD_Show2Num(60+48,110,time/60,2,16,0);//����
		//LCD_ShowChar(60+64,110,':',16,0);
		//LCD_Show2Num(60+72,110,time%60,2,16,0);//����		    
		//��ʾ��ǰ��Ŀ������,������Ŀ��
  		//LCD_Show2Num(60+0,130,index,3,16,0);  //����
		//LCD_ShowChar(60+24,130,'/',16,0);
  		//LCD_Show2Num(60+32,130,total,3,16,0); //����Ŀ		  
		//��ʾλ��			   
 		//LCD_ShowNum(60+60,130,f_kbps,3,16);//��ʾλ��
		//LCD_ShowString(60+84,130,"Kbps");	  	  
	}   		 
}			  		 

//��������
u8 Play_Music(u8 data)
{
  
        u16 songname=0;
        u16 k;
        u16 index=0;
	//u8 key;
 	FileInfoStruct FileInfo;	
        u8 Music_name[4];
	u16 mus_total=0;//�������ļ��ĸ���
	if(FAT32_Enable)Cur_Dir_Cluster=FirstDirClust;//��Ŀ¼�غ�
 	else Cur_Dir_Cluster=0;	
        
        /*�Ǵ���ģʽ�²���ASY01Ŀ¼�µ� MP3 �ļ�*/
        if(data==1)
        {
          Cur_Dir_Cluster=FirstDirClust;
	  FileInfo=F_Search(Cur_Dir_Cluster,"ASY01",T_FILE);
          if(FileInfo.F_StartCluster==0)
          {
            return 1;
          }
          else
          {
            Cur_Dir_Cluster=FileInfo.F_StartCluster;            //�õ��ļ��п�ʼ�غ�
           
            /****  ����ģʽ1  ****/
            if(mode==1)
            {
              if(P03==0)                              
              {
                delay_ms(10);    //��������
                if(P03==0)
                {
                  while(P03==0); //�ȴ������ɿ�
                  mnob=1;
                  while(1)
                 {  
                    Music_name[0]=0x30+mnob/100;
                    Music_name[1]=0x30+mnob%100/10;
                    Music_name[2]=0x30+mnob%10;
                    Music_name[3]='0';                   
                    k=Play_Song(Music_name,data);
                    
                    if(k==1)
                    {
                      if(mnob>1)
                        mnob--;
                      else
                        mnob=1;
                    }
                    else if(k==2|k==0)
                    {
                      mnob++;
                    }
                    else if(k==3)
                    {
                      mnob=1;
                    }
                    else if(k==4)   //ֹͣ���˳�����
                    {
                      return 0;
                    }
                 }
                }
              }
            }
            
            /****  ����ģʽ2  ****/
            else if(mode==2)
            {
              if(exit_set==0)
              {
                exit_set=1;
              EXTI_InitTypeDef EXTI_InitStructure;	//�ⲿ�ж���
	
            /* Connect Button EXTI Line to Button GPIO Pin */
              GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);  	//ѡ��PB0���ڵ�GPIO�ܽ������ⲿ�ж���·EXIT0		
            /* Configure Button EXTI line */
              EXTI_InitStructure.EXTI_Line = EXTI_Line0;	//�ⲿ��·EXIT0
              EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�����ⲿ�ж�ģʽ:EXTI��·Ϊ�ж�����  EXTI_Mode_Event ;//���� EXTI��·Ϊ�¼����� 
              EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //�ⲿ�жϴ�����ѡ��:����������·�½���Ϊ�ж�����
              EXTI_InitStructure.EXTI_LineCmd = ENABLE;		//ʹ���ⲿ�ж���״̬
              EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
                            
              GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1); 
              EXTI_InitStructure.EXTI_Line = EXTI_Line1;	//�ⲿ��·EXIT1
              EXTI_Init(&EXTI_InitStructure);	
              
              GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2); 
              EXTI_InitStructure.EXTI_Line = EXTI_Line2;	//�ⲿ��·EXIT2
              EXTI_Init(&EXTI_InitStructure);	
              
              GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3); 
              EXTI_InitStructure.EXTI_Line = EXTI_Line3;	//�ⲿ��·EXIT3
              EXTI_Init(&EXTI_InitStructure);	
              
              GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4); 
              EXTI_InitStructure.EXTI_Line = EXTI_Line4;	//�ⲿ��·EXIT4
              EXTI_Init(&EXTI_InitStructure);	
              
              GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5); 
              EXTI_InitStructure.EXTI_Line = EXTI_Line5;	//�ⲿ��·EXIT5
              EXTI_Init(&EXTI_InitStructure);	
              
              GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6); 
              EXTI_InitStructure.EXTI_Line = EXTI_Line6;	//�ⲿ��·EXIT6
              EXTI_Init(&EXTI_InitStructure);	
              
              GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7); 
              EXTI_InitStructure.EXTI_Line = EXTI_Line7;	//�ⲿ��·EXIT7
              EXTI_Init(&EXTI_InitStructure);	
              
              GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8); 
              EXTI_InitStructure.EXTI_Line = EXTI_Line8;	//�ⲿ��·EXIT8
              EXTI_Init(&EXTI_InitStructure);	
              
              GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11); 
              EXTI_InitStructure.EXTI_Line = EXTI_Line11;	//�ⲿ��·EXIT11
              EXTI_Init(&EXTI_InitStructure);	
              }
              
              while(1)
              {
                mnob=newmnob;
                Music_name[0]=0x30+mnob/100;
                Music_name[1]=0x30+mnob%100/10;
                Music_name[2]=0x30+mnob%10;
                Music_name[3]='\0';  
                if(mnob>0)
                {
                  k=Play_Song(Music_name,data);
                  if(k==3)
                  {
                    newmnob=0;
                    return 0;
                  }
                }
                else
                  return 0;  
              }         
            }
          
            
            /****  ����ģʽ3  ****/
            else if(mode==3)
            {
              while(P01==0)
              {
                mnob=P02|(P03<<1)|(P04<<2)|(P05<<3)|(P06<<4)|(P07<<5)|(P08<<6)|(P09<<7)|(P10<<8);
                songname=~mnob&0x1ff;
                Music_name[0]=0x30+songname/100;
                Music_name[1]=0x30+songname%100/10;
                Music_name[2]=0x30+songname%10;
                Music_name[3]='\0';
                if(songname)
                {
                  Play_Song(Music_name,data);
                }
              }
              return 0;
            }
            
           /****  ����ģʽ4  ****/
            else if(mode==4)  
            {
              Get_File_Info(Cur_Dir_Cluster,&FileInfo,T_MP3,&index); 
              mus_total=index;
              k=1;
              while(1)
              {
                Get_File_Info(Cur_Dir_Cluster,&FileInfo,T_MP3,&k); 
                if(Play_Song(FileInfo.F_Name,data)==4)
                  return 0;
                if(k<mus_total)
                  k++;
                else
                  k=1;
                
              }
              
              
            }
	  		
          }
        } 
        else if(data==2)   //����ģʽ
        {
          Cur_Dir_Cluster=FirstDirClust;
          u8 dir[6]="ASY01";
          dir[3]=USRT_Buffer[2];
          dir[4]=USRT_Buffer[3];
          
	  FileInfo=F_Search(Cur_Dir_Cluster,dir,T_FILE);
          if(FileInfo.F_StartCluster==0)
          {
            UART_Send_Missage("Not this dir!");
            return 1;
          }
          else
          {
            Cur_Dir_Cluster=FileInfo.F_StartCluster;
            Music_name[0]=USRT_Buffer[4];
            Music_name[1]=USRT_Buffer[5];
            Music_name[2]=USRT_Buffer[6];
            Music_name[3]='\0';
            for(u8 m=0;m<7;m++)
            {
              USRT_Buffer[m]='\0';
            }
            k=Play_Song(Music_name,data);
            if(k==3)
            UART_Send_Missage("Not this MP3 file!");
            else
            UART_Send_Missage("play end");
          }
        }
        return 0;
}
////////////////////////////////////////////////////////
//��������
//index:���ŵĸ������
//����ֵ��0,�ɹ���1����һ����2����һ����0xff �õ��ļ���Ϣʧ�ܣ�0xfe��Ӳ��λʧ�ܣ�	 
u8 MUSIC_BUFFER[512];
u8 Play_Song(u8 *Music_Name,u8 data)
{	 
	u32 bfactor;
	u32 bcluster;
	u16 count;
	u8  n,key;
        u8  led=0;
	u16 i;	 
	u8 pause=0;//����ͣ  
 
  	FileInfoStruct FileInfo; 
        
        
        FileInfo=F_Search(Cur_Dir_Cluster,Music_Name,T_MP3);
        if(FileInfo.F_StartCluster==0)
          {
            return 3;  //�Ҳ��������� ģʽһ����һ�ף����ز��ŵ�һ�� ģʽ��������
          }         
	//i=Get_File_Info(Cur_Dir_Cluster,&FileInfo,T_MP3|T_WMA|T_WAV|T_MID|T_FLAC|T_OGG,&index);    
	//if(i==0)return 0xff;			//�õ��ļ���Ϣʧ�ܡ�
	if(VS_HD_Reset())
        {
          UART_Send_Missage("VS_HD_Reset Fail!");
          //return 0xfe;	
        }//Ӳ��λ						 				  		 	  
	VS_Soft_Reset();  		        //��λVS10XX   
        
	set10XX();        				//������������Ϣ  								 
	//LCD_Fill(0,110,239,319,WHITE);			//������Ļ���
	//Show_Str(60,150,FileInfo.F_Name,16,0); 		//��ʾ��������
        
	bfactor=fatClustToSect(FileInfo.F_StartCluster);  	//�õ���ʼ�ض�Ӧ������
	bcluster=FileInfo.F_StartCluster;			//�õ��ļ���ʼ�غ�	 
	count=0;	
        UART_Send_Missage(FileInfo.F_Name);
        
        
	while(1)   //�������ֵ���ѭ��
	{
	    if(SD_ReadSingleBlock(bfactor,MUSIC_BUFFER))break;//��ȡһ������������   
		SPIx_SetSpeed(SPI_SPEED_8);//����,��VS1003B,���ֵ���ܳ���36.864/6Mhz����������Ϊ4.5M 

		count++;//���������� 
		i=0;
		do      //������ѭ��
	       {  	    
			if(VS_DQ!=0&&pause==0)  // ����ͣ �����ݸ�VS1003 
			{			   	 
	    		  VS_XDCS_CLR;    
	                  for(n=0;n<32;n++)
				{
				  SPIx_ReadWriteByte(MUSIC_BUFFER[i++]);	 			
				}
			  VS_XDCS_SET;    
 			}
                        
                      if(data==1)
                      {
                        if(mode==1)
                        {
                          key=P01|(P02<<1)|(P03<<2)|(P04<<3)|(P05<<4)|(P06<<5);
                          while((P01|(P02<<1)|(P03<<2)|(P04<<3)|(P05<<4)|(P06<<5))!=0x3F); //�ȴ����а���songkai 
                          switch(key)
                          {
                            case 0x3E:     //���š���ͣ
                              {
                                pause=!pause;   
                                break;
                              }
                            case 0x3D:     //��һ��
                                return 1;
                            case 0x3B:     //��һ��
                                return 2;
                            case 0x37:     //vol-
                              {
                               if(VS10XXram[4]>(0xff-7*VOL_STEP))
                                 VS10XXram[4]=VS10XXram[4]-VOL_STEP;
                               else
                                 VS10XXram[4]=0;  
                               set10XX();
                               break;
                              }
                            case 0x2F:    //vol+
                              {
                                if(VS10XXram[4]==0)
                                  VS10XXram[4]=0xff-7*VOL_STEP;
                                else if(VS10XXram[4]<(0xff-VOL_STEP))
                                 VS10XXram[4]=VS10XXram[4]+VOL_STEP;
                                else
                                 VS10XXram[4]=0xff;  
                                 set10XX();
                                 break;
                              }
                            case 0x1F:   //stop
                              {
                                return 4;
                              } 
                          default:
                              break;
                           }
                        }
                        
                        else if(mode==2)
                        {
                          if(mnob!=newmnob)//���°�������
                          {
                            return 0;
                          }
                          else
                          {
                            switch(mnob)
                            {
                            case 1:
                              {
                                if(P01==1) 
                                  return 3;
                                else 
                                  break;
                              }
                            case 2:
                              {
                                if(P02==1) 
                                  return 3;
                                else
                                  break;
                              }
                            case 3:
                              {
                                if(P03==1) 
                                  return 3;
                                else
                                  break;
                              }
                            case 4:
                              {
                               if(P04==1) 
                                 return 3;
                               else
                                 break;
                              }
                            case 5:
                              {
                                if(P05==1) 
                                  return 3;
                                else
                                  break;
                              }
                            case 6:
                              {
                                if(P06==1) 
                                  return 3;
                                else
                                  break;
                              }
                            case 7:
                              {
                                if(P07==1) 
                                  return 3;
                                else
                                  break;
                              }
                            case 8:
                              {
                                if(P08==1) 
                                  return 3;
                                else
                                  break;
                              }
                            case 9:
                              {
                                if(P09==1) 
                                  return 3;
                                else
                                  break;
                              }
                            case 10:
                              {
                                if(P10==1) 
                                  return 3;
                                else
                                  break;
                              }
                            default:
                              return 3;
                            }
                          }
                        }
                        
                        else if(mode==3)
                        {
                          if(mnob!=(P02|(P03<<1)|(P04<<2)|(P05<<3)|(P06<<4)|(P07<<5)|(P08<<6)|(P09<<7)|(P10<<8)))
                            return 0;
                          if(P01==1)
                          {
                            return 0;
                          }
                        }
                      }
                        if(Rev_ok==1)
                        {
                          key=USRT_Buffer[1];
                          switch(key)
                          {
                          case PLAY:
                          case PLAY_BMSC:
                              return 4;
                              
                          case STOP:
                          case STOP_BMSC:
                            {
                              Rev_ok=0;
                              return 5;
                            }
                            
                          case PAUSE:
                          case PAUSE_BMSC:
                            {
                              pause=1;
                              Rev_ok=0;
                              break;
                            }
                            
                          case REPLAY:
                          case REPLAY_BMSC:
                            {
                              pause=0;
                              Rev_ok=0;
                              break;
                            }
                            
                          case VOL:
                            {
                               Rev_ok=0;
                                if(USRT_Buffer[2]==0)
                                  VS10XXram[4]=0;
                                else
                                  VS10XXram[4]= 0xff-VOL_STEP*(8-USRT_Buffer[2]);
                               set10XX(); 
                               break;
                            }
                          default:
                            break;
                          }
                        }
			    
		}while(i<511);	//ѭ������512���ֽ� 
		//MP3_Msg_Show(FileInfo.F_Size,index,total);	    
	    bfactor++; 		//������				  
	    if(count>=SectorsPerClust)//һ���ؽ���,����
	    {									  
	        count=0;  					 	   
	        bcluster=FAT_NextCluster(bcluster);	
                if(led==0)
                {
                  LED_MP3_ON1;
                  LED_MP3_ON2;
                }
                else
                {
                  LED_MP3_OFF1; 
                  LED_MP3_OFF2; 
                }
                led=!led;
			//printf("NEXT:%d\n",bcluster);
			//LED1=!LED1;	   
			//�ļ����� 
		if((FAT32_Enable==0&&bcluster==0xffff)||bcluster==0x0ffffff8||bcluster==0x0fffffff)break;//error	
	        bfactor=fatClustToSect(bcluster);//�õ��ض�Ӧ������
	    } 
	}	 
	VS_HD_Reset();  //Ӳ��λ						 				  		 	  
	VS_Soft_Reset();//��λ 
	LED_MP3_OFF1;		//�ر�MP3_LED��
        LED_MP3_OFF2;		//�ر�MP3_LED��
        
	return 0;		//���ذ����ļ�ֵ! 
}













