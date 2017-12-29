/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V1.0
* Date               : 10/08/2007
* Description        : Main program body
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include <PlayMode.h>
#include "hardware_conf.h"
#include <stdio.h>
#include <delay.h>
#include <string.h>
#include "InitIQS333.h"
#include "oled.h"
#include "bmp.h"
#include "mfrc522.h"
#include "string.h"
#include "time.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

//#define   ONLINE_SERVICE



GetUmb GetUmbTable[10];
u8 GetResultNum = 0;
ReturnUmb ReturnUmbTable[10];
u8 ReturnNum = 0;
u8 PollingOk = 0;
u8 PollingTime = POLLINGTIME;

u8 Rev_ok = 0;
u8 exit_set = 0;
extern u8 USRT_Buffer[7];
u16 Temperature, Weight, testvalue;
u8 Reconnectcount = 0;
int time_over = 10000;
//new defined valuable as below
u8 Heat;         // heat sign
u8 Warm;         // keep warm sign
u8 Warmtemp;     // keep warm temp valuable
u8 Warmheat;     // heating inorder to keep warm sign
u8 CheckTime;    // 0.5S check a time
u8 ledstate;     // use led slight
u8 WarmEnable;   // can start warm operation?usually enable after heat complete?
u8 HeatCRC;      // temp compare?after turn off heat device
u8 WaterTemp;    // real-time water temperature
u8 WaterWeight;
u8 CWaterWeight;
u8 HeatCRCStart; //start temp compare?after turn off heat device 15S
u8 HeatCRCTime;  //Timer, =0 start temp compare
u8 FirstCRC;     //
u8 Telecontrol;  //wifi Telecontrol
u8 Keycontrol;   // Keyboard operation
u16 LightHeatOffTime;
u8 WarmOK;
u32 WarmTime;
u8 Mystate, MystatePre;
u8 bFlagOK;
u8 TeaPotHub;
u8 Pre_TeaPotHub;
u8 Factor_Set_Sign;
u8 EnCool;
u8 EnSendHubState;
u16 TimeSendHubState;
u8 Connect_Service;
u8 FactorSign;
u8 cmdmode;
u16 DelayNum10ms = 0;
u8 WifiConnectok = 0;
u16 OffLineTime;
u16 Read_RequstTime;
u8 Read_Requst;
u8 Finish = 0;
u16 FinishTime = 0;
u8 StartTime = 0;
u8 ModeReadOk = 0;
u8 WifiMode = 0;
u8 Keyend = 0;
u8 App_OnLine = 1;
u8 RelayOn = 0;
u32 RelayOnTime;
u8 TryConnectWan = 0;
u16 TryConnectWanTime = 0;
u8 TimeSync = 0;
u8 FirstPowerOn;
u8 Weight_Verf_Sign;
u16 WEIGHT_VERIFY;
u8 KeyEvent = 0;
u8 nowhubstate;
u8 ActWeightVerf = 0;
u8 WarmHeatTime = 0;
u16 dispopendoortime=0;

u16 Pure_Time;
u8 Start_Pure = 0;
u8 SendTime = 0;
/***for test**/
u16 failtime = 0;
u16 restartcount = 0;
u16 TestHobTime = 0;
u8 displaying = 0;
extern u8 beepwarm;
extern u8 StartWeightVref;
extern u16 ProtectTime;
extern u8 TempRenew;


u8 RenewListEn = 0;
u8 Purify = 0;
u8 Boil = 0;
u8 CommRcvBuf[300];
u8 CommRcvBufIO[16];
u8 bFlagOKm = 0;
u8 UartSend_Buffer1[300];
u8 UartSend_BufferIO[16];
u8 RecvWifiCmd[64];
u8 *RecvWifiCmdEnd;
u8 RcvDate[30];
u8 disbuf[3];
u8 ReadIDSign = 0;

u8 adcok;


u8 Ordernum = 0;

HeatRecord RecordTable[10];
u8 RecordNum = 0;

u8 TeleWarmTime = 0;

//区号二维码
char community_code[62];
//五一
//0xff,0xff,0xff,0xff,0xff,0xff,0xdd,0x61,0xd4,0x4c,0x25,0x87
//区号对应密钥（凤凰城）
unsigned char DefaultKey[4][6] = {{0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
                                                                         {0x70, 0x5e, 0xc2, 0x4d, 0x6a, 0x83},
                                                                            {0x70, 0x5e, 0xc2, 0x4d, 0x6a, 0x83},
                                                                            {0x70, 0x5e, 0xc2, 0x4d, 0x6a, 0x83}};

char MachineID_Default[19] = "000000000000000000";//机器码
unsigned int usart1_lent_old = 0, usart1_lent = 0, usart1_read_en = 0;
u8 Usart1_buf[900];
unsigned int usart2_lent_old = 0, usart2_lent = 0, usart2_read_en = 0;
u8 Usart2_buf[256];
u8 str_buf[200];
u8 Usart2_buf_old[256];
u8 UmbrellaID[19] = {0};
u8 Orderid[17];
u8 Operation;
u16 StateTime = 0;
u8 OrderOperState = 0;
u16 RcvokTime = 60;

u8 TYear;
u8 TMon;
u8 TDay;
u8 THour;
u8 TMin;
u8 TSec;
u8 TWeek;
u32 SecondTime;
u8 UpdateState;
u8 wtmp, wtmppre;
u8 wtmp2 = 0;
u8 KeyEnable;
u8 Key_end_Telecontrol = 0;
u8 WifiLight = 0;
u8 LED_Buff[8] = {
        0x1f,
        0x1f,
        0x1f,
        0x1f,
        0x1f,
        0x1f,
        0x1f,
        0x1f,};

//new defined valuable as up

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

u16 ADC_ConvertedValue[64];
u8 NumberOfAD = 0;

u16 adcconvertime = 0;

#define read_mp3 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
#define MP3_ON_OFF(tmp)               GPIOB->ODR=tmp?(GPIOB->ODR|1<<12):(GPIOB->ODR&(~(1<<12)))

//int isDebug = 0;
#define _DEBUG_ 0
#if _DEBUG_
#define ACCLOG(...) printf(__VA_ARGS__)
#else
#define ACCLOG(...)
#endif
//DEFINE
u8 isDoorShow = 0;
u8 preNetStatus = 0;
u8 cacheLog[20][100] = NULL;
int status[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//数据存储状态
int now_status = -1;//发送第几条数据
int cover = 0;//覆盖第几条数据


/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);

void GPIO_Configuration(void);

void NVIC_Configuration(void);

void USART_Configuration(void);

void RTC_Configuration(void);

void Init_ADC(void);

void IWDG_Configuration(void);

void Leddisp(u8 tempvalue, u8 state);

void Init_Exit(void);

//u8 TempAdjust(u8 temp);
void ADConverData(void);

void Heat_Offline(void);

void Heat_Online(void);

void CommProc3(void);

void WifiSendData(u8 num);
//static int get_parmer(unsigned char *str,unsigned char *name,unsigned char *context,unsigned char chr);

void Rst_Value(void);

void http_send(u8 *sendbuf, u8 *path);

void DelayN10ms(u16 time);

void set_mode(u8 mode);

void WaitConnect(void);

void SwithChannel(u8 num);

//void CheckOrder(void);
void TIM3_Init();

void TIM2_Init();

void TIM6_Int_Init(u16 arr, u16 psc);

//u16 AD_sysGetAdcResult(u8 Channel);
//void Get_ADC_Value(void);
u8 TempRef(u8 tempnow);

u8 ObjVref(u8 tempobj);

void Init_I2C(void);

void Init_IQS333(void);

u8 I2C1_Write_NBytes(u8 driver_Addr, u8 start_Addr, u8 number_Bytes, u8 *write_Buffer);

u8 I2C1_Read_NBytes(u8 driver_Addr, u8 start_Addr, u8 number_Bytes, u8 *read_Buffer);

void KeyProcess(void);

void Restart_WifiMode(void);

u8 SrvDataProcess(u8 number);

void SendToLock(u8 id, u8 num);

char check_scan(void);

int find_car_new(int, int);

void printhextobin(char c);

void getNumber();


void sendMessage(char *, int, int);

void soundBee(void);

void getAllMessage();
void save_community(char code[]);
/* Private functions ---------------------------------------------------------*/

void Init_Value(void) {
    //u8 i;
    Heat = 0;      // heat sign
    Warm = 0;      // keep warm sign
    Warmtemp = 0;  // keep warm temp valuable
    Warmheat = 0;
    CheckTime = 0;
    ledstate = 1;
    WarmEnable = 0;
    EnCool = 0;
    HeatCRC = 0;
    Temperature = 0;
    WaterTemp = 0;
    WaterWeight = 0;
    HeatCRCStart = 0;
    HeatCRCTime = 0;
    FirstCRC = 0;
    Telecontrol = 0;
    Keycontrol = 0;
    LightHeatOffTime = 0;
    WarmOK = 0;
    WarmTime = 0;
    Pure_Time = 0;
    adcok = 0;
    Mystate = 0;
    MystatePre = 0;
    bFlagOK = 0;
    TeaPotHub = 0;
    Pre_TeaPotHub = 0;
    //memset(OrderTable,0,sizeof(HeatInfo)*16);
    memset(RecordTable, 0, sizeof(HeatRecord) * 10);  //Orderid

    memset(Orderid, 0, 17);
    Factor_Set_Sign = 0;
    EnSendHubState = 0;
    TimeSendHubState = 0;
    RecvWifiCmdEnd = RecvWifiCmd;
    cmdmode = 0;
    DelayNum10ms = 0;
    FirstPowerOn = 1;
    App_OnLine = 1;
    Weight_Verf_Sign = 0;
    wtmp = 0;
    wtmppre = 0;

    TYear = 10;
    TMon = 1;
    TDay = 1;
    THour = 8;
    TMin = 0;
    TSec = 0;
    TWeek = 5;
    SecondTime = THour * 3600;
    UpdateState = 0;
    KeyEnable = 30;
    TestHobTime = 100;

    memset(ReturnUmbTable, 0, sizeof(ReturnUmbTable));
    memset(GetUmbTable, 0, sizeof(GetUmbTable));
}

void delay(u16 num) {
    u16 i, j;
    for (i = 0; i < num; i++)
        for (j = 0; j < 0x1000; j++);
}

void Delay(uint16_t nCount) {
    /* Decrement nCount value */
    while (nCount != 0) {
        nCount--;
    }
}

/*******************************************************************************
* Function Name  :
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void IO_TXD(u8 Data) {
    u8 i = 0;
    OI_TXD(0);
    delay_us(100);
    for (i = 0; i < 8; i++) {
        if (Data & 0x01)
            OI_TXD(1);
        else
            OI_TXD(0);

        delay_us(100);
        Data = Data >> 1;
    }
    OI_TXD(1);
    delay_us(100);
}

void USART_Send(u8 *buf, u8 len) {
    u8 t;
    for (t = 0; t < len; t++) {
        IO_TXD(buf[t]);
    }
}

int heart_num = 0;//??????????
int scroll = 256;
int count = 0;//GPRS检测
int hardwareTime = 0;//????????????
int car_return = 0;//用于检测是否开门
int code_return = 6;//用于检测扫码是否开门
//int block=0;
unsigned char snr, buf[16], TagType[2], SelectedSnr[4];


unsigned char buf_key[16] = {'4', '4', '4', '4', '4', '4', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5'};
char ki = 0;

char carNumber[5];//
char overTime[6];//????
char twoNumber[17];//?????
int part = 1;//???????
char swipe[4] = "166";//刷卡密码
char mostTime[6] = "171128";//硬件当前年月日
char car_str[32];//???????
int shanquCode = 3;
int kuaiCode = 2;

//十六进制转二进制字符
void printhextobin(char c) {
    char x[17][5] = {
            "0000", "0001", "0010", "0011",
            "0100", "0101", "0110", "0111",
            "1000", "1001", "1010", "1011",
            "1100", "1101", "1110", "1111",
    };
    int n = 16;
    if (c >= 'a' && c <= 'f') n = c - 'a' + 10;
    if (c >= 'A' && c <= 'F') n = c - 'A' + 10;
    if (c >= '0' && c <= '9') n = c - '0';
    // printf("%s", x[n]);
    strcat(twoNumber, x[n]);
}


int find_car() {
    return find_car_new(2, 0);
}

//获取卡片解密所需要的参数
void getNumber() {
//  swipe = 167;
    if (atoi(swipe) % 16 == 0) {
        part = atoi(swipe) / 16 - 1;
    } else {
        part = atoi(swipe) / 16;
    }

    if (atoi(swipe) / 16 <= 8) {
        //        printf("diyi\n");
        find_car_new(2, 2);
    } else if (atoi(swipe) / 16 <= 16) {
        //        printf("dier\n");
        part = part - 8;
        shanquCode = 3;
        kuaiCode = 0;
    } else if (atoi(swipe) / 16 <= 24) {
        part = part - 16;
        //        printf("disan\n");
        shanquCode = 3;
        kuaiCode = 1;
    } else {
        part = part - 24;
        //        printf("disi\n");
        shanquCode = 3;
        kuaiCode = 2;
    }
}

//读卡解密部分
int jieMika(char src9[]) {
    /*读卡解密部分 */
    strncpy(carNumber, src9 + 1, 5);
    //printf("car_number:%s\n",carNumber);
    strncpy(overTime, src9 + 14, 6);
    ACCLOG("overTime:%s\n",overTime);
    int over = atoi(overTime);
    int mine = atoi(mostTime);
    //printf("%d,%d\n",over,mine);
    if (over >= mine) {
        return 1;
    } else {
        return 0;
    }
}

int check_carNumber(char src14[]) {
    /* ????????? */
    char kami[4];
    strncpy(kami, src14 + (part * 4), 4);
    //printf("16numer:%s\n",kami);
    memset((char *) &twoNumber, 0, sizeof(twoNumber));
    for (int i = 0; i < 4; i++) {
        printhextobin(kami[i]);
    }
    //printf("2number:%s\n", twoNumber);
    int aa = atoi(swipe) - atoi(swipe) / 16 * 16;
    char last[1];
    strncpy(last, twoNumber + (aa - 1), 1);
    if (atoi(last) == 0) {
        //    printf("cheng gong\n");
        return 1;
    } else {
        //        printf("shi bai\n");
        return 0;
    }
}

//读取卡的内容
int find_car_new(int sanqu, int qukuai) {
    char i;
    char status;
    char buf1[16];
    status = PcdRequest(REQ_ALL, TagType);//?? ???0??
    if (!status) {
        status = PcdAnticoll(SelectedSnr);//???????????
        if (!status) {
            status = PcdSelect(SelectedSnr);//????
            if (!status) {
                snr = sanqu;
                status = PcdAuthState(KEYA, (snr * 4 + 0), DefaultKey[sanqu], SelectedSnr);// ??0????
                if (!status) {
                    status = PcdRead((snr * 4 + qukuai), buf);  //PcdWrite((snr*4+1), buf);    // ??0??1???
                    if (!status) {
                        snr = sanqu = shanquCode;
                        qukuai = kuaiCode;
                        status = PcdAuthState(KEYA, (snr * 4 + 0), DefaultKey[sanqu], SelectedSnr);// ??0????
                        if (!status) {
                            status = PcdRead((snr * 4 + qukuai), buf1);  //PcdWrite((snr*4+1), buf);    // ??0??1???
                            if (!status) {

                                soundBee();
                                dispopendoortime=300;
                                ACCLOG("open dooor sound two...\n");


                                delay_10ms(20);
                                memset((char *) &car_str, 0, sizeof(car_str));
                                char tempChar[2];
                                for (i = 0; i < 16; i++) {
                                    int intNumber = (int) buf[i];
                                    sprintf(tempChar, "%x", intNumber);
                                    int isZero = (int) tempChar[1];
                                    if (isZero == 0) {
                                        sprintf(&car_str[i * 2], "%s", "0");
                                        car_str[i * 2 + 1] = tempChar[0];
                                    } else {
                                        car_str[i * 2] = tempChar[0];
                                        car_str[i * 2 + 1] = tempChar[1];
                                    }
                                }
                                // printf("car_code1:%s\n",car_str);
                                if (shanquCode != 0 && atoi(swipe) != -1 && jieMika(car_str) == 1) 
                                {
                                    memset((char *) &car_str, 0, sizeof(car_str));
                                    for (i = 0; i < 16; i++) {
                                        int intNumber = (int) buf1[i];
                                        sprintf(tempChar, "%x", intNumber);
                                        int isZero = (int) tempChar[1];
                                        if (isZero == 0) {
                                            sprintf(&car_str[i * 2], "%s", "0");
                                            car_str[i * 2 + 1] = tempChar[0];
                                        } else {
                                            car_str[i * 2] = tempChar[0];
                                            car_str[i * 2 + 1] = tempChar[1];
                                        }
                                    }
                                    
                                    ACCLOG("find card:%s\n", car_str);                

                                    if (check_carNumber(car_str) == 1) {
                                        ACCLOG("find card return:1\n");
                                        sendMessage(carNumber, 2, 0);
                                        car_return = 1;
                                        return 1;
                                    } else {
                                      
                                        ACCLOG("find card return:2\n");
                                        sendMessage(carNumber, 0, 0);
                                        car_return = 3;
                                        return 3;
                                    }
                                } else {
                                 
                                    ACCLOG("find card return other:2 \n");
                                    sendMessage(carNumber, 0, 0);
                                    car_return = 3;
                                    return 3;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        car_return = 0;
        return 0;
    }
    car_return = 0;
    return 0;
}

char find_action(void) {
    
    find_car();

    if (car_return == 1) {
        return 1;
    } else if (car_return == 3) {
        return 3;
    }
    if (usart2_read_en == 1) {//????
        if (memcmp((char *) &Usart2_buf, (char *) &Usart2_buf_old, sizeof(Usart2_buf)) == 0) {//??????
            memset((char *) &Usart2_buf, 0, sizeof(Usart2_buf));
            usart2_lent_old = usart2_lent = usart2_read_en = 0;
        } else {
            car_return = 2;
            return 2;
        }
    }
    return 0;
}

int no_net_display = -1;//10ms
char nonet_en = 0;

void no_net_display_stop(void) {
    /*if(no_net_display!=-1){
        OLED_DrawBMP(0,0,128,8,best_use);
        no_net_display=-1;
    }*/
}

void soundBee(void) {
    DOOR_PWR(1);
    DelayN10ms(60);
    DOOR_PWR(0);
}

void displayOpenDoor(int isOpen) {
    ACCLOG("door status comming %d...\n", isOpen);
    isDoorShow = 1;
    dispopendoortime=50;
    if (isOpen == 1) {
        OLED_DrawBMP(0, 0, 128, 8, door_ok);
        
        OPEN_SW(1);
        DelayN10ms(50);
        OPEN_SW(0);
    } else if (5 == isOpen) {
        OLED_DrawBMP(0, 0, 128, 8, write_suc);
    } else if (8 == isOpen) {
        OLED_DrawBMP(0, 0, 128, 8, test_door);
    } else if (0 == isOpen) {
        OLED_DrawBMP(0, 0, 128, 8, error_door);
    }
    car_return = 0;
    code_return = 6;
    while(dispopendoortime);
}

void delay10_in_nonet(void) {
    DelayN10ms(1);
    //    if (no_net_display < 200) {
    find_action();
    if (car_return != 0) {
        no_net_display = 300;
        if (car_return == 1) { //开门

            displayOpenDoor(1);
        } else if (car_return == 3) {
            displayOpenDoor(0);
        } else {
            check_scan();
            if (code_return == 1) {//开门(这是扫码开门？)
                ACCLOG("check scan success....\n");

                displayOpenDoor(1);
            } else if (code_return == 5) {
                displayOpenDoor(5);
            } else if (code_return == 0) {
                ACCLOG("check scan error....\n");
                displayOpenDoor(0);//显示屏
            } else if (code_return == 8) {
                displayOpenDoor(8);
            }
        }
        car_return = 0;
        memset((char *) &Usart2_buf_old, 0, sizeof(Usart2_buf_old));
        memset((char *) &Usart2_buf, 0, sizeof(Usart2_buf));
        usart2_lent_old = usart2_lent = usart2_read_en = 0;
    }
    
    //修改2
    if (no_net_display == 0) {
        //main_OLED_DrawBMP(scroll, 128);  //显示
        //        OLED_DrawBMP(0, 0, 128, 8, best_use);
        no_net_display = -1;
    }
}

//重置gprs
void reset_gprs(void) {
    ACCLOG("reset gprs begin...\n");
    time_over = 10000;
    char i = 0;
    GPRS_EN(1);
    for (i = 0; i < 100; i++)delay10_in_nonet();
    for (i = 0; i < 100; i++)delay10_in_nonet();
    GPRS_EN(0);
    for (i = 0; i < 100; i++)delay10_in_nonet();
    no_net_display_stop();
    count = 0;
}

char *p, *s, *d, *n, *f, *l, *g, *h,*t;
char gprs_select_en = 1;


int net_stu = 0;

u16 j;
u8 str_tmp[4];
u16 Flash_tmp[4];
u8 SlaveID;


#define NR(x)  (sizeof(x)/sizeof(x[0]))
char myheartbeat[21];//保存
char myTime[11] = "1500000000";//硬件保存的时间戳(解密前)
char myKey[11] = "0001112229";//硬件保存的秘钥（解密前）
char nowKey[6] = "123456";//硬件保存的秘钥（解密后）

char scanCode[21];//未解密用户信息
char nowCode[21];//已解密用户信息
char phone[12];//从二维码扫到的手机号
char userTime[11];//从二维码扫到的时间戳
char userKey[11];//从二维码扫到的未解密用户密钥
char userNewKey[6];//从二维码扫到的解密用户密钥


int jieMi21[21] = {19, 12, 15, 11, 8, 18, 13, 21, 3, 17, 10, 6, 16, 2, 9, 20, 7, 14, 4, 1, 5};//长解密规则
int jieMi10[10] = {3, 1, 6, 8, 7, 10, 4, 9, 5, 2};//短解密规则


char carNumber[5];//卡号
char overTime[6];//过期时间



void getHeartbeat(char src[]);

void getCode(char myCode[], int admin);

//心跳解密函数
void getHeartbeat(char src[]) {
    if (strlen(src) == 48 || strlen(src) == 51) {
        if (strlen(src) == 51) {

            strncpy(swipe, src + 46, 3);
        }
        strncpy(myheartbeat, src + 20, 26);
        strncpy(myKey, myheartbeat, 10);//???????
        strncpy(myTime, myheartbeat + 10, 10);//?????
        strncpy(mostTime, myheartbeat + 20, 6);//???????????
        hardwareTime = 0;
        //  printf("??????%s\n",myKey);
        //printf("????%s\n",myTime);
        //printf("6????:%s\n",mostTime);

        char arr3[10];
        //将密钥字符串转为字符数组
        for (int i = 0; i < 10; i++) {
            *(arr3 + i) = *(myKey + i);
        }

        char arr4[10];
        //解密
        for (int i = 0; i < 10; i++) {
            arr4[i] = arr3[jieMi10[i] - 1];
            // printf("%c",arr2[i]);
        }

        //将密钥字符拼接成字符串
        for (int i = 0; i < 6; i++) {
            nowKey[i] = arr4[i];
        }
        getAllMessage();
    }
}

//扫码解密函数
void getCode(char src2[], int admin) {

    strncpy(scanCode, src2 + 6, 21);//获取未解密用户信息
    char arr1[100];
    //将未解密用户信息转为字符数组
    for (int i = 0; i < 21; i++) {
        *(arr1 + i) = *(scanCode + i);
    }

    char arr2[21];
    //解密用户信息
    for (int i = 0; i < 21; i++) {
        arr2[i] = arr1[jieMi21[i] - 1];
    }
    //将用户信息拼接成字符串
    for (int i = 0; i < NR(arr2); i++) {
        nowCode[i] = arr2[i];
    }

    //  printf("已解密用户信息：%s\n",nowCode);
    strncpy(phone, nowCode, 11);//获取手机号码
    if (admin == 1) {
        hardwareTime = 0;
        strncpy(myTime, nowCode + 11, 10); //重置时间戳

        //ACCLOG("hardwareTime:%s\n",myTime);
    } else {
        strncpy(userTime, nowCode + 11, 10); //获取时间戳
    }
}


//检测心跳
void check_ser(void) {
    u8 i = 0;
    p = strstr(Usart1_buf, "{\"status\":1,\"data\":\"");
    s = strstr(Usart1_buf, "}");
    l = strstr(Usart1_buf, "\"L\":");
    time_over = 320000;
    ACCLOG("xintiao:%s\n", Usart1_buf);
    ACCLOG("changdu:%d\n",strlen(Usart1_buf));
    if(NULL != l&&NULL != p&&NULL != s&&(strlen(Usart1_buf) - 51)%34==0)
    {
      int allLoad = (strlen(Usart1_buf) - 51) / 34;//获取信息返回个数
      for(int i=0;i<allLoad;i++)
      {
        char tt[3];
        strncpy(tt, Usart1_buf + 51+ 30*(i+1)+i*4, 2);
        ACCLOG("clean:%s\n",tt);
        status[atoi(tt)-1]=0;
      }
    }else if(NULL != l&&NULL != p&&NULL != s&&(strlen(Usart1_buf))%34==0){
      
      int allLoad = (strlen(Usart1_buf)) / 34;//获取信息返回个数
      for(int i=0;i<allLoad;i++)
      {
        char tt[3];
        strncpy(tt, Usart1_buf + 30*(i+1)+i*4, 2);
        ACCLOG("clean:%s\n",tt);
        status[atoi(tt)-1]=0;
      }
    }
    ACCLOG("check ser comming...\n");
    
    if ((NULL != p && NULL != s)) {
        count = 0;
        
        getHeartbeat(Usart1_buf);
        getNumber();
    }    
}

//发送消息
void sendMessage(char *user, int sta, int isOther) {
    now_status = -1;
    for(int i=0;i<20;i++)
    {
      if(0==status[i])
      {
        now_status = i;
        status[i]=1;
        ACCLOG("now_status:%d\n",now_status);
        break;
      }
    }
    if(now_status == -1)
    {
      now_status = cover;
      if(cover>=19)
      {
        cover = 0;
      }else{
        cover++;
      }
    }
    memset((char *) &str_buf, 0, sizeof(str_buf));
    if (isOther) {
        sprintf(str_buf, "{\"D\":\"%s\",\"U\":\"%s\",\"T\":\"2\",\"S\":\"%lld\"}\r\n", MachineID_Default,
                user,
                atoi(myTime) + (int) (hardwareTime/100));
        ACCLOG("SENDO:%s\n", str_buf);
    } else {
        sprintf(str_buf, "{\"D\":\"%s\",\"U\":\"%s\",\"T\":\"%d\",\"S\":\"%d\",\"L\":\"%02d\"}\r\n", MachineID_Default,
                user,
                sta,
                atoi(myTime) + (int) (hardwareTime/100),now_status+1);
    }
    sprintf(cacheLog[now_status],str_buf);
    UART_Send_Missage(str_buf);
    for(int i=0;i<20;i++)
    {
      if(status[i]==1)
      {
        ACCLOG("the %d log:%s\n",(i+1),cacheLog[i]);
      }
    }
}

//区号处理
void save_community(char code[]){
    char str[60];/*待分隔的原字符串*/ 
    char seg[] = ","; /*分隔符这里为逗号comma，分隔符可以为你指定的，如分号，空格等*/   
    strncpy(str,code+2,59);
      
    char *substr= strtok(str, seg);/*利用现成的分割函数,substr为分割出来的子字符串*/  
    int ly[6];
    int py[6]; 
    for(int i=0;i<12;i++)
    {    
        if(i<6)
        {
            sscanf(substr,"%x",&ly[i]);
//             printf("%#x\n",ly[i]);
        }else{
            sscanf(substr,"%x",&py[i-6]);
            // printf("%#x\n",py[i]);
        }
        substr = strtok(NULL,seg);/*在第一次调用时，strtok()必需给予参数str字符串， 
        往后的调用则将参数str设置成NULL。每次调用成功则返回被分割出片段的指针。*/  
    } 
    
    memset((char *) &DefaultKey, 0, sizeof(DefaultKey));
    for(int i=0;i<4;i++)
    {
        if(i==0)
        {
            for(int j=0;j<6;j++)
            {
                DefaultKey[i][j]=ly[j];
                //printf("%#x\n",ly[j]);
            }
        }else{
            for(int j=0;j<6;j++)
            {
                DefaultKey[i][j]=py[j];
                //  printf("%#x\n",py[j]);
            }
        }
    }
    
     for(int i=0;i<4;i++)
    {
        for(int j=0;j<6;j++)
        {
            ACCLOG("%#x,",DefaultKey[i][j]);
        }
        ACCLOG("\n");
    }
}

//检测二维码部分
char check_scan(void) {
    soundBee();
    u8 i = 0;
    p = strstr(Usart2_buf, "M\":\"");
    s = strstr(Usart2_buf, "U\":\"");
    d = strstr(Usart2_buf, "\",\"1\":\"");

    n = strstr(Usart2_buf, "F\":\"");
    f = strstr(Usart2_buf, "\"}");
    g = strstr(Usart2_buf, "{\"W\":\"9008316E912230654C490\",\"1\":\"4a0ma5ut0w\",\"M\":\"168\"}");
    h = strstr(Usart2_buf, "sunyikeji");
    t = strstr(Usart2_buf, "AC");
    ACCLOG("myCode:%s\n", Usart2_buf);
    ACCLOG("changdu:%d\n",strlen(Usart2_buf));
    if (NULL != p && NULL != s && NULL != d) {

        /*获取用户信息*/
        getCode(Usart2_buf, 0);
        int isOpen = 0;//是否含有对应密钥标志位
        int allLoad = (strlen(Usart2_buf) - 38) / 17;//获取用户密钥个数

        if (net_stu == 1)//判断硬件是否正在联网
        {
            for (int i = 0; i < allLoad; i++)//筛选密钥
            {
                memset((char *) &userKey, 0, sizeof(userKey));
                strncpy(userKey, Usart2_buf + 29 + 5 + i * 17, 10);
                if ((strcmp(myKey, userKey) == 0))//判断密钥是否相同
                {
                    isOpen = 1;
                    break;
                }
            }

            if (1 == isOpen)//判断用户是否含有对应的密钥
            {
                //判断用户时间是否超时
                if (atoi(myTime) + (int) (hardwareTime / 100) + 300 < atoi(userTime) ||
                    atoi(myTime) + (hardwareTime / 100) - 300 > atoi(userTime))//????????
                {
                    sendMessage(phone, 0, 0);
                    code_return = 0;
                    return 0;
                } else {
                    sendMessage(phone, 1, 0);
                    code_return = 1;
                    return 1;
                }
            } else {
                sendMessage(phone, 0, 0);
                code_return = 0;
                return 0;
            }
        } else {
            for (int i = 0; i < allLoad; i++)//筛选密钥
            {
                memset((char *) &userKey, 0, sizeof(userKey));
                strncpy(userKey, Usart2_buf + 29 + 5 + i * 17, 10);//获取用户未解密密钥
                char arr3[10];
                for (int i = 0; i < 10; i++) {
                    *(arr3 + i) = *(userKey + i);
                }
                char arr4[10];
                //??
                for (int i = 0; i < 10; i++) {
                    arr4[i] = arr3[jieMi10[i] - 1];
                }
                for (int i = 0; i < 6; i++) {
                    userNewKey[i] = arr4[i];
                }
                // printf("Mine:%s\nuser:%s\n",nowKey,userNewKey);
                if ((strcmp(nowKey, userNewKey) == 0))//判断已解密密钥是否相同
                {
                    isOpen = 1;
                    break;
                }
            }

            if (1 == isOpen)//判断用户是否含有对应的密钥
            {
                if (atoi(myTime) + (hardwareTime / 100) - 300 > atoi(userTime))//用户时间是否超过硬件时间5分钟或以上
                {
                    sendMessage(phone, 0, 0);
                    code_return = 0;
                    return 0;
                } else {
                    if (atoi(myTime) + 300 <= atoi(userTime)) {
                        getCode(Usart2_buf, 1);
                    }
                    sendMessage(phone, 1, 0);
                    code_return = 1;
                    return 1;
                }
            } else {
                sendMessage(phone, 0, 0);
                code_return = 0;
                return 0;
            }
        }
    } else if ((NULL != f && NULL != n) || (NULL != h&&strlen(Usart2_buf)==28)||((NULL != t&&strlen(Usart2_buf)==62))) {//判断是否管理二维码

        if (NULL != h) {
            memset((char *) &MachineID_Default, 0, sizeof(MachineID_Default));
            strncpy(MachineID_Default, Usart2_buf + 9, 18);
            ACCLOG("new_MachineID_Default:%s\n", MachineID_Default);
        }else if (NULL != t){
          memset((char *) &community_code, 0, sizeof(community_code));
          sprintf(community_code, Usart2_buf);
          ACCLOG("community_code:%s\n",community_code);
          save_community(Usart2_buf);
        }else {

            getCode(Usart2_buf, 1);
            strncpy(myKey, Usart2_buf + 34 + 18 + 7, 10);//获取硬件未解密密钥

            // printf("δޢĜĜԿ:%s\n",myKey);

            strncpy(swipe, Usart2_buf + 34 + 18 + 7 + 10 + 7, 3);


            getNumber();
            // printf("后三位:%d\n",swipe);

            char arr3[10];
            for (int i = 0; i < 10; i++) {
                *(arr3 + i) = *(myKey + i);
            }

            char arr4[10];
            for (int i = 0; i < 10; i++) {
                arr4[i] = arr3[jieMi10[i] - 1];
            }

            //获取用户已解密密钥
            for (int i = 0; i < 6; i++) {
                nowKey[i] = arr4[i];
            }
            //printf("硬件已解密密钥:%s\n",nowKey);
        }
        getAllMessage();
        code_return = 5;
        return 5;

    } else if (NULL != g) {

        code_return = 8;
        return 8;
    }
    code_return = 0;
    return 0;
}

char gprs_check(void *jj, int time) {
    
    char *p;

    DelayNum10ms = time;
    while (1) {

        if (no_net_display < 200) {
            find_action();

            if (car_return != 0) {
                no_net_display = 300;

                if (car_return == 1) { //开门

                    displayOpenDoor(1);
                } else if (car_return == 3) {
                    displayOpenDoor(0);
                } else {
                    check_scan();
                    if (code_return == 1) {//开门(这是扫码开门？)
                        ACCLOG("check scan success....\n");
                        displayOpenDoor(1);

                    } else if (code_return == 5) {
                        displayOpenDoor(5);
                    } else if (code_return == 0) {
                        ACCLOG("check scan error....\n");
                        displayOpenDoor(0);//显示屏
                    } else if (code_return == 8) {
                        displayOpenDoor(8);
                    }
                }
                car_return = 0;
                memset((char *) &Usart2_buf_old, 0, sizeof(Usart2_buf_old));
                memset((char *) &Usart2_buf, 0, sizeof(Usart2_buf));
                usart2_lent_old = usart2_lent = usart2_read_en = 0;
            }
        }
        //修改
        if (no_net_display == 0) {
            //main_OLED_DrawBMP(scroll, 128);  //显示
            no_net_display = -1;
        }

        if (DelayNum10ms <= 0) {
            no_net_display_stop();
            return 0;
        }
        if (usart1_read_en == 1) {
            p = strstr(Usart1_buf, jj);
            if (NULL != p) {//查询 connect ok
                no_net_display_stop();
                return 1;
            }
            memset((char *) &Usart1_buf, 0, sizeof(Usart1_buf));
            usart1_lent_old = usart1_lent = usart1_read_en = 0;
        }
    }
}

void getAllMessage() {
    char in_data[200];//要写入的数据
    sprintf(in_data, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n", MachineID_Default, myKey, nowKey, myTime, mostTime, swipe,community_code);
    write_flash(0x0800F800, (u16 *) in_data, 200);
    ACCLOG("write success\n");
}

int main(void) {
    u8 i;
#ifdef DEBUG
    debug();
#endif
    ACCLOG("start...\n");
    //isDebug = 1;
    /* Init_Value --------------------------------------------------------------*/
    Init_Value();
    /* System clocks configuration ---------------------------------------------*/
    RCC_Configuration();
    /* NVIC configuration ------------------------------------------------------*/
    NVIC_Configuration();
    /* GPIO configuration ------------------------------------------------------*/
    GPIO_Configuration();

    USART_Configuration();
    //SysTick_Config(SYSTIME);
    TIM2_Init();
    
    IWDG_Configuration();//看门狗
    delay_init(48);

    MFRC522_init();

    soundBee();
    
    OLED_Init();            //初始化OLED
    OLED_Clear();
    MP3_ON_OFF(1);
    TIM3_Init();    

   // getAllMessage();//存放测试使用

    
    
    
    u8 out_data[200];//读存放
    read_flash(0x0800F800, (u32 *) out_data, 200);
    ACCLOG("pln:%s\n", out_data);

    strncpy(MachineID_Default, out_data, 18);
    ACCLOG("MachineID_Default:%s\n", MachineID_Default);
    strncpy(myKey, out_data + 19, 10);
    ACCLOG("myKey:%s\n", myKey);
    strncpy(nowKey, out_data + 30, 6);
    ACCLOG("nowKey:%s\n", nowKey);
    strncpy(myTime, out_data + 37, 10);
    ACCLOG("myTime:%s\n", myTime);
    strncpy(mostTime, out_data + 48, 6);
    ACCLOG("mostTime:%s\n", mostTime);
    strncpy(swipe, out_data + 55, 3);
    ACCLOG("swipe:%s\n", swipe);
    strncpy(community_code, out_data + 59, 61);
    ACCLOG("community_code:%s\n", community_code);
    save_community(community_code);
    getNumber();

    while (gprs_select_en) {
       DelayN10ms(100);
        reset_gprs();
        /***/
        for (i = 0; i < 10; i++) {
            memset((char *) &Usart1_buf, 0, sizeof(Usart1_buf));
            usart1_lent_old = usart1_lent = usart1_read_en = 0;
            UART_Send_Missage("AT\r\n");
            if (gprs_check("OK", 300) == 1)break;
        }
        if (i >= 10)continue;
        /***/
       DelayN10ms(20);
        for (i = 0; i < 10; i++) {
            memset((char *) &Usart1_buf, 0, sizeof(Usart1_buf));
            usart1_lent_old = usart1_lent = usart1_read_en = 0;
            UART_Send_Missage("AT+GSN\r\n");
            if (gprs_check("OK", 200) == 1)break;
        }
        if (i >= 10)continue;
        /***/DelayN10ms(20);

        for (i = 0; i < 10; i++) {
            memset((char *) &Usart1_buf, 0, sizeof(Usart1_buf));
            usart1_lent_old = usart1_lent = usart1_read_en = 0;
            UART_Send_Missage("AT+CPIN?\r\n");
            if (gprs_check("+CPIN: READY", 200) == 1)break;
        }
        if (i >= 10)continue;
        DelayN10ms(20);

        reconect:
        for (i = 0; i < 10; i++) {
            memset((char *) &Usart1_buf, 0, sizeof(Usart1_buf));
            usart1_lent_old = usart1_lent = usart1_read_en = 0;
            UART_Send_Missage("AT+QIFGCNT=0\r\n");
            if (gprs_check("OK", 500) == 1)break;
        }
        if (i >= 10)continue;
        /***/
        DelayN10ms(20);
        for (i = 0; i < 10; i++) {
            memset((char *) &Usart1_buf, 0, sizeof(Usart1_buf));
            usart1_lent_old = usart1_lent = usart1_read_en = 0;
            UART_Send_Missage("AT+QIMODE=1\r\n");
            if (gprs_check("OK", 200) == 1)break;
        }
        if (i >= 10)continue;
        /***/DelayN10ms(20);
        for (i = 0; i < 10; i++) {
            memset((char *) &Usart1_buf, 0, sizeof(Usart1_buf));
            usart1_lent_old = usart1_lent = usart1_read_en = 0;
            UART_Send_Missage("AT+QIDEACT\r\n");
            if (gprs_check("OK", 200) == 1)break;
        }
        if (i >= 10)continue;
        /***/DelayN10ms(20);
        for (i = 0; i < 10; i++) {
            memset((char *) &Usart1_buf, 0, sizeof(Usart1_buf));
            usart1_lent_old = usart1_lent = usart1_read_en = 0;
            UART_Send_Missage("AT+QIMUX=0\r\n");
            if (gprs_check("OK", 200) == 1)break;
        }
        if (i >= 10)continue;
        /***/
        DelayN10ms(10);
        for (i = 0; i < 10; i++) {
            memset((char *) &Usart1_buf, 0, sizeof(Usart1_buf));
            usart1_lent_old = usart1_lent = usart1_read_en = 0;
            UART_Send_Missage("AT+QIDNSIP=1\r\n");
            if (gprs_check("OK", 200) == 1)break;
        }
        if (i >= 10)continue;
        /***/DelayN10ms(20);
        memset((char *) &Usart1_buf, 0, sizeof(Usart1_buf));
        usart1_lent_old = usart1_lent = usart1_read_en = 0;

        UART_Send_Missage("AT+QIOPEN=\"TCP\",\"api.sunsyi.com\",5555\r\n");
        //UART_Send_Missage("AT+QIOPEN=\"TCP\",\"bgyfhh02.chinacloudapp.cn\",5555\r\n");
        i = 0;
        DelayNum10ms = 2000;
        while (1) {
            //main_OLED_DrawBMP(scroll, 128);  //显示
            if (no_net_display < 200) {
                find_action();
                if (car_return != 0) {
                    //if(isDebug) {
                    ACCLOG("find action now...\n");
                    //}
                    no_net_display = 300;
                    memset((char *) &Usart2_buf_old, 0, sizeof(Usart2_buf_old));
                    memset((char *) &Usart2_buf, 0, sizeof(Usart2_buf));
                    usart2_lent_old = usart2_lent = usart2_read_en = 0;


                    ACCLOG("open dooor sound one...\n");
                    if (car_return == 1) { //开门

                        ACCLOG("open door now...\n");

                        displayOpenDoor(1);
                    } else {

                        check_scan();
                        if (code_return == 1) {//开门
                            ACCLOG("open door now from check scan...\n");
                            displayOpenDoor(1);
                        } else if (code_return == 5) {
                            displayOpenDoor(5);
                        } else if (code_return == 0) {
                            ACCLOG("open door fail...\n");
                            displayOpenDoor(0);
                        } else if (code_return == 8) {
                            displayOpenDoor(8);
                        }
                    }
                    car_return = 0;
                    memset((char *) &Usart2_buf_old, 0, sizeof(Usart2_buf_old));
                    memset((char *) &Usart2_buf, 0, sizeof(Usart2_buf));
                    usart2_lent_old = usart2_lent = usart2_read_en = 0;
                }
            }


            if (no_net_display == 0) {
                //main_OLED_DrawBMP(scroll, 128);  //显示
                //                OLED_DrawBMP(0, 0, 128, 8, best_use);
                no_net_display = -1;
            }

            if (DelayNum10ms <= 0) {
                i = 10;
                break;
            }

            if (usart1_read_en == 1) {
                p = strstr(Usart1_buf, "CONNECT");
                s = strstr(Usart1_buf, "FAIL");
                d = strstr(Usart1_buf, "DOWN");
                if (NULL != p && NULL == s) {//查询 connect ok
                    Reconnectcount = 0;
                    i = 9;
                    break;
                }
                if (NULL != s) {//如果连接失败就重复3次跳出重启
                    if (i == 3) {
                        i = 10;
                        break;
                    }
                    memset((char *) &Usart1_buf, 0, sizeof(Usart1_buf));
                    usart1_lent_old = usart1_lent = usart1_read_en = 0;
                    UART_Send_Missage("AT+QIOPEN=\"TCP\",\"api.sunsyi.com\",5555\r\n");
                    //UART_Send_Missage("AT+QIOPEN=\"TCP\",\"bgyfhh02.chinacloudapp.cn\",5555\r\n");
                    DelayNum10ms = 2000;
                    i++;
                }
                if (NULL != d) {
                    i = 11;
                    break;
                }
                memset((char *) &Usart1_buf, 0, sizeof(Usart1_buf));
                usart1_lent_old = usart1_lent = usart1_read_en = 0;
            }
        }
        no_net_display_stop();
        memset((char *) &Usart1_buf, 0, sizeof(Usart1_buf));
        usart1_lent_old = usart1_lent = usart1_read_en = 0;
        if (i == 10) {
            ACCLOG("i == 10...\n");
            for (j = 0; j < 400; j++)
                delay10_in_nonet();
            no_net_display_stop();
            reset_gprs();
            continue;
        }
        if (i == 11) {
            ACCLOG("i == 11...\n");
            for (j = 0; j < 400; j++)
                delay10_in_nonet();
            no_net_display_stop();
            continue;
        }

        for (j = 0; j < 300; j++)
            delay10_in_nonet();
        ACCLOG("back no net display stop...\n");
        no_net_display_stop();
        
        dispopendoortime=300;
        net_display(0, 0, 1);
        dispopendoortime=0;

        while (1) {
            //main_OLED_DrawBMP(scroll, 128);  //显示
            net_stu = 1;
            
            int findCardResult = find_car();
            if (findCardResult == 1) {
                if (NULL == s && 0) {
                    displayOpenDoor(0);
                    for (i = 0; i < 10; i++) {
                        find_action();
                        if (car_return != 0) {
                            break;
                        }
                        DelayN10ms(10);
                    }
                    //main_OLED_DrawBMP(scroll, 128);  //显示
                    dispopendoortime=300;
                    net_display(0, 0, 1);
                    dispopendoortime=0;
                    continue;
                }
                ACCLOG("find card now...\n");
                displayOpenDoor(1);
                //main_OLED_DrawBMP(scroll, 128);  //显示
                dispopendoortime=300;
                net_display(0, 0, 1);
                dispopendoortime=0;
            } else if (findCardResult == 3) {
                ACCLOG("did not find card...\n");
                
                displayOpenDoor(0);

                //main_OLED_DrawBMP(scroll, 128);  //显示
                dispopendoortime=300;
                net_display(0, 0, 1);
                dispopendoortime=0;
            }
            if (usart2_read_en == 1) {//蓝牙数据

                memcpy((char *) &Usart2_buf_old, (char *) &Usart2_buf, sizeof(Usart2_buf));
                check_scan();
                if (code_return == 0 || code_return == 5 || code_return == 8) {
                    if (code_return == 5) {
                        displayOpenDoor(5);
                    } else if (code_return == 8) {
                        displayOpenDoor(8);
                    } else {
                        displayOpenDoor(0);
                    }
                    memset((char *) &Usart2_buf, 0, sizeof(Usart2_buf));
                    usart2_lent_old = usart2_lent = usart2_read_en = 0;
                   
                    //main_OLED_DrawBMP(scroll, 128);  //显示
                   
                    dispopendoortime=300;
                    net_display(0, 0, 1);
                    dispopendoortime=0;
                    continue;
                } else if (code_return == 10) {
                    continue;
                } else if (code_return == 1) {
                    MP3_ON_OFF(0);
                    displayOpenDoor(1);
                }


                memset((char *) &Usart2_buf, 0, sizeof(Usart2_buf));
                usart2_lent_old = usart2_lent = usart2_read_en = 0;
                for (i = 0; i < 100; i++) {
                    if (i == 10)memset((char *) &Usart2_buf_old, 0, sizeof(Usart2_buf_old));
                }


                //main_OLED_DrawBMP(scroll, 128);  //显示
               
                dispopendoortime=300;
                net_display(0, 0, 1);
                dispopendoortime=0;
                MP3_ON_OFF(1);
                if (car_return != 0)continue;
                memset((char *) &Usart2_buf_old, 0, sizeof(Usart2_buf_old));
                memset((char *) &Usart2_buf, 0, sizeof(Usart2_buf));
                usart2_lent_old = usart2_lent = usart2_read_en = 0;

            }

            if (heart_num == 0) {//心跳
                memset((char *) &str_buf, 0, sizeof(str_buf));
                sprintf(str_buf, "{\"D\":\"%s\",\"U\":\"0\",\"T\":\"\",\"S\":\"\"}\r\n", MachineID_Default);
                ACCLOG("SENDOTHER:%s\n", str_buf);
                UART_Send_Missage(str_buf);  //发送心跳
               
                heart_num = 6000*5;
                
                for(int i=0;i<20;i++)
                {
                  if(status[i]==1)
                  {
//                    DelayN10ms(100);
                    UART_Send_Missage(cacheLog[i]);
                    ACCLOG("the %d log:%s\n",(i+1),cacheLog[i]);
//                    break;
                  }
                }
            }
            //超时立即重启gprs
            if (count >= time_over) // 2017-11-11
            {
                count = 0;
                UART_Send_Missage("+++");
                delay(2000);
                UART_Send_Missage("AT+QICLOSE\r\n");
                delay(1000);
                Reconnectcount++;
                goto reconect;
            }     // 2017-11-11

            if (isDoorShow) {
                isDoorShow = 0;
            } else if (preNetStatus != net_stu) {
                preNetStatus = net_stu;
                if (net_stu == 1) {
                    ACCLOG("is connect...%d\n", 1);
                    //main_OLED_DrawBMP(scroll, 128);  //显示
                    //                    OLED_DrawBMP(0, 0, 128, 8, best_use);
                    dispopendoortime=300;
                    net_display(0, 0, 1);
                    dispopendoortime=0;
                } else {
                    ACCLOG("is not connect...%d\n", 0);
                    //main_OLED_DrawBMP(scroll, 128);  //显示
                    //                    OLED_DrawBMP(0, 0, 128, 8, best_use);
                    no_net_display = -1;
                }
            }

            if (usart1_read_en == 1) {//WIFI数据
                /************************************心跳***/
                //int isConnect =
                check_ser();
                memset((char *) &Usart1_buf, 0, sizeof(Usart1_buf));
                usart1_lent_old = usart1_lent = usart1_read_en = 0;
              
            }
        }
        net_stu = 0;
        LED(0);
        reset_gprs();
    }
}

void TIM6_Int_Init(u16 arr, u16 psc) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //???1??

    //?????IM43?????
    TIM_TimeBaseStructure.TIM_Period = arr; //?????????????????t????????????????????????????????
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //?????????aTIMx???????3y??????????
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //?????????:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????????
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //???Y??????2??3?????TIMx???????y????
    TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE); //?1????????TIM3?D??,???????D??

    //?D????????NVIC???
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;  //TIM4?D??
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ?????1??
    NVIC_Init(&NVIC_InitStructure);  //3?????NVIC??????
}

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void) {

    RCC_AHBPeriphClockCmd(
            RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOF | RCC_AHBPeriph_FLITF |
            RCC_AHBPeriph_SRAM, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG | RCC_APB2Periph_ADC1 | RCC_APB2Periph_USART1 | RCC_APB2Periph_DBGMCU,
                           ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_WWDG, ENABLE);

}


/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void GPIO_Configuration(void) {
    GPIO_InitTypeDef GPIO_InitStructure;


    /*USART1*/

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*USART2*/
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);




    // 蜂鸣器  2017-11-11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);

    /* SMARTCONFIG WifiLedCtl*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIOB->BRR = GPIO_Pin_4;
    GPIOB->BSRR = GPIO_Pin_3;



    // GPRS_EN
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);


    /*****************语音控制******************************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIOA->BRR = GPIO_Pin_15;

    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /*****************************************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);

}

/**
* @brief  Configures the USART1 controller.
* @param  None
* @retval None
*/

void USART_Configuration(void) {
    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStructure;

    USART_InitStructure.USART_BaudRate = BAUD1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
    USART_ClockInit(USART1, &USART_ClockInitStructure);

    USART_Cmd(USART1, ENABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    USART_InitStructure.USART_BaudRate = BAUD2;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);

    USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
    USART_ClockInit(USART2, &USART_ClockInitStructure);

    USART_Cmd(USART2, ENABLE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void Init_ADC(void) {
    ADC_InitTypeDef ADC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32) & ADC_ConvertedValue;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);


    /* ADCs DeInit */
    ADC_DeInit(ADC1);
    /* Initialize ADC structure */
    ADC_StructInit(&ADC_InitStructure);
    /* Configure the ADC1 in continuous mode with a resolution equal to 12 bits  */
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_OverrunModeCmd(ADC1, ENABLE);
    ADC_ChannelConfig(ADC1, ADC_Channel_0 | ADC_Channel_1, ADC_SampleTime_239_5Cycles);//temp
    // Enable ADC1 DMA
    ADC_DMACmd(ADC1, ENABLE);

    ADC_GetCalibrationFactor(ADC1);

    /* Enable the ADC peripheral */
    ADC_Cmd(ADC1, ENABLE);

    /* Wait the ADRDY flag */
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY));

    ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);

    ADC_StartOfConversion(ADC1);

    /* Enable DMA Channel1 complete transfer interrupt */
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

    DMA_ClearITPendingBit(DMA_IT_TC);
    // Enable DMA channel1
    DMA_Cmd(DMA1_Channel1, ENABLE);
}


void Delayms(u16 ms) {
    u16 i, j;
    u8 k;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 0x0aaa; j++) k++;
}


/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void) {
    NVIC_InitTypeDef NVIC_InitStructure;


    /* USART1的NVIC中断配置 */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* USART2的NVIC中断配置 */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /*IRQ*/
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* enabling interrupt */

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;//定时器优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* enabling interrupt   */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Configure the SysTick handler priority */
    //  NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick, 2, 0);
    /*NVIC_InitStructure.NVIC_IRQChannel=SysTick_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);*/
}

void IWDG_Configuration(void) {
    /* IWDG counter clock: 40KHz(LSI) / 4 = 10 KHz */
    IWDG_SetPrescaler(IWDG_Prescaler_4);

    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    /* Set counter reload value to 1000 */
    //该参数允许取值范围为0 – 0x0FFF
    IWDG_SetReload(4000);  //400ms
    /* Reload IWDG counter */
    IWDG_ReloadCounter();
    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();

}

void TIM3_Init() {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    //TIM_DeInit(TIM3);
    //TIM_InternalClockConfig(TIM3);
    //TIM_TimeBaseStructure.TIM_Period=(TIM_CLK/2000)-1;
    // TIM_TimeBaseStructure.TIM_Prescaler=1999;
    //TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式

    TIM_TimeBaseStructure.TIM_Period = (TIM_CLK / 2000) - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 1999;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//    TIM_SetAutoreload(TIM3, ((TIM_CLK / 2000) - 1));
    TIM_SetAutoreload(TIM3, ((TIM_CLK / 150000) - 1));

    //TIM_OCInitTypeDef TIMOCInitStructure;
    //TIMOCInitStructure.TIM_OCMode = TIM_OCMode_Timing; //PWM输出比较模式
    //TIMOCInitStructure.TIM_Pulse =(u32)((TIM_CLK/(3000))-1);//占空比=(CCRx/ARR)*100%
    //TIMOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM输出比较极性高
    //TIMOCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//使能输出状态
    // TIM_OC1Init(TIM3, &TIMOCInitStructure);//TIM3的CH1输出

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); // 打开捕获比较中断 TIM_IT_Update

    TIM_Cmd(TIM3, ENABLE); //开启时钟
}

void TIM2_Init() {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);


    TIM_TimeBaseStructure.TIM_Period = (TIM_CLK / 200000) - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 1999;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_SetAutoreload(TIM2, ((TIM_CLK / 200000) - 1));


    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 打开捕获比较中断 TIM_IT_Update

    TIM_Cmd(TIM2, ENABLE); //开启时钟
}


void Init_Exit(void) {
    EXTI_InitTypeDef EXTI_InitStructure;    //外部中断线

    /* Connect Button EXTI Line to Button GPIO Pin */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource7);
    EXTI_ClearITPendingBit(EXTI_Line7);
    /* Configure Button EXTI line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line7;    //外部线路EXIT5
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;            //设外外部中断模式:EXTI线路为中断请求  EXTI_Mode_Event ;//设置 EXTI线路为事件请求
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //外部中断触发沿选择:设置输入线路下降沿为中断请求
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;        //使能外部中断新状态
    EXTI_Init(&EXTI_InitStructure);        //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
}

void DelayN10ms(u16 time) {
    DelayNum10ms = time;
    while (DelayNum10ms != 0);
}
