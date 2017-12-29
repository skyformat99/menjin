#ifndef HARDWARE_CONF_H
#define HARDWARE_CONF_H


#define TIM_CLK                 48000000
#define CHECKTIME               20                        // 0.2S
#define HEAT_CRC_TIME           20                        // 20S
#define LIGHT_HEAT_OFF_TIME     10                        // 10S
#define KEEP_WARM_TIME          (120*60)  // 120分钟
#define TIME_SEND_HUB_STATE     (250)                     // 2.5S
#define OFF_LINE_TIME            62                       // 62S app不在线3次请求失败
#define POLLINGTIME              100                      // 总线轮询时间 1S

#define APP_ONLINE_READ_REQUST_TIME     (3*100)
#define APP_OFFLINE_READ_REQUST_TIME    (20*100)
#define CONNECT_WAN_TIME                (61*100)

#define SPEAK_TIME          (1*100)
#define START_TIME          (1*10)
#define SLIGHTTIME           50
#define BOND_TRY_TIME        15

#define TEAPOT_HUB    24
#define SYSTIME       480000    //10mS

#define TEMP_INC         5
#define TEMP_VERIFY      0
//#define WEIGHT_VERIFY   (875+50-300)  //4321
#define WEIGHT_VERIFY_DEFAULT   (1500)

#define WARM_KEY      1
#define WIFI_KEY      2
#define All_LIGHT_ON  3
#define ALL_LIGHT_OFF 4
#define SIX_LIGHT_ON  5
#define SIX_LIGHT_OFF 6
#define HEAT_TEMP60   60
#define HEAT_TEMP70   70
#define HEAT_TEMP80   80
#define HEAT_TEMP90   90
#define HEAT_TEMP100  100
#define BOIL_TEMP     96

#define WARM_ENABLE    1
#define WARM_DISABLE   0


#define LOW_WATER       5
#define KEY_LOW_WATER   3
#define KEY_HIGH_WATER  18
#define HIGH_WATER      17
/******************************************************************************/
#define OI_RXD           (GPIOF->IDR&(1<<7))
#define OI_TXD(tmp)      GPIOF->ODR=tmp?(GPIOF->ODR|1<<6):(GPIOF->ODR&(~(1<<6)))


#define HEAT_TEMP60_PIN  (GPIOC->IDR&(1<<4))
#define HEAT_TEMP70_PIN  (GPIOC->IDR&(1<<3))
#define HEAT_TEMP80_PIN  (GPIOC->IDR&(1<<2))
#define HEAT_TEMP90_PIN  (GPIOC->IDR&(1<<1))
#define HEAT_TEMP100_PIN (GPIOC->IDR&(1<<0))


#define LIGHT1(tmp)       GPIOB->ODR=tmp?(GPIOB->ODR&(~(1<<11))):(GPIOB->ODR|1<<11)
#define LIGHT2(tmp)       GPIOB->ODR=tmp?(GPIOB->ODR&(~(1<<10))):(GPIOB->ODR|1<<10)
#define LIGHT3(tmp)       GPIOB->ODR=tmp?(GPIOB->ODR&(~(1<<14))):(GPIOB->ODR|1<<14)
#define LIGHT4(tmp)       GPIOB->ODR=tmp?(GPIOB->ODR&(~(1<<12))):(GPIOB->ODR|1<<12)
#define LIGHT5(tmp)       GPIOB->ODR=tmp?(GPIOB->ODR&(~(1<<13))):(GPIOB->ODR|1<<13)
#define LIGHT6(tmp)       GPIOB->ODR=tmp?(GPIOB->ODR&(~(1<<15))):(GPIOB->ODR|1<<15)
#define LIGHTALLOFF       GPIOB->BSRR=0xFC00
#define LIGHTHEATOFF      LED_Buff[0]=LED_Buff[2]=LED_Buff[3]=LED_Buff[4]=LED_Buff[5]=0x1f;
#define WARM_LIGHT(tmp)   LED_Buff[1]=tmp?0xDF:0x1f;


#define GPRS_EN(tmp)            GPIOA->ODR=tmp?(GPIOA->ODR|1<<8):(GPIOA->ODR&(~(1<<8)))
#define OPEN_SW(tmp)            GPIOA->ODR=tmp?(GPIOA->ODR|1<<15):(GPIOA->ODR&(~(1<<15)))  //2017-11-11  门锁 
#define DOOR_PWR1(tmp)           GPIOA->ODR=tmp?(GPIOA->ODR|1<<1):(GPIOA->ODR&(~(1<<1)))//2017-12-2  蜂鸣器
#define DOOR_PWR(tmp)             GPIOA->ODR=tmp?(GPIOA->ODR|1<<4):(GPIOA->ODR&(~(1<<4)))//2017-11-11  蜂鸣器
#define LED(tmp)           // GPIOA->ODR=tmp?(GPIOA->ODR|1<<4):(GPIOA->ODR&(~(1<<4)))

#define RELAY(tmp) 
#define Speak(tmp) 

//#define LOCK1(tmp)               GPIOA->ODR=tmp?(GPIOA->ODR|1<<12):(GPIOA->ODR&(~(1<<12)))
//#define LOCK2(tmp)               GPIOA->ODR=tmp?(GPIOA->ODR|1<<11):(GPIOA->ODR&(~(1<<11)))
//#define LOCK3(tmp)               GPIOF->ODR=tmp?(GPIOF->ODR|1<<6):(GPIOF->ODR&(~(1<<6)))
//#define LOCK4(tmp)               GPIOF->ODR=tmp?(GPIOF->ODR|1<<7):(GPIOF->ODR&(~(1<<7)))

#define WifiLedCtl(tmp)          GPIOB->ODR=tmp?(GPIOB->ODR|1<<4):(GPIOB->ODR&(~(1<<4)))

#define DATA_H                   GPIOB->ODR=(GPIOB->ODR|1<<6)
#define DATA_L                   GPIOB->ODR=(GPIOB->ODR&(~(1<<6)))
#define CLK_H                    GPIOB->ODR=(GPIOB->ODR|1<<7)
#define CLK_L                    GPIOB->ODR=(GPIOB->ODR&(~(1<<7)))
#define RCL_H                    GPIOB->ODR=(GPIOB->ODR|1<<5)
#define RCL_L                    GPIOB->ODR=(GPIOB->ODR&(~(1<<5)))



//#define LOCK_STATE              (GPIOB->IDR&(1<<6))
//#define UMBRELLA_STATE          (GPIOB->IDR&(1<<7))

//#define LOCK3_STATE             (GPIOA->IDR&(1<<0))
//#define LOCK2_STATE             (GPIOA->IDR&(1<<1))
//#define LOCK4_STATE             (GPIOA->IDR&(1<<9))
/******************************************************************************/

#define BAUD1         115200
#define BAUD2         9600

#define MAX_RECV_NUM1  200
#define MAX_RECV_NUM3  200

#define TEAPOTPOWER    2000

/*******************************************************************************/
#define  ADC1_DR_Address        ((u32)0x40012440)


//64 flash
#define  CANSHU_START_ADDR      ((u32)0x0800EC00)//59
#define  ID_START_ADDR          ((u32)0x0800F000)//60
#define  ORDER_START_ADDR	((u32)0x0800F400)//61
#define  FACTOR_START_ADDR	((u32)0x0800F800)//62
#define  WEIGHT_VERF_ADDR	((u32)0x0800FC00)//63

// 512 flash
//#define  ID_START_ADDR	        ((u32)0x08064000)//200
//#define  ORDER_START_ADDR	((u32)0x08064800)//201
//#define  FACTOR_START_ADDR	((u32)0x08068000)//202
/*******************************************************************************/
void DelayN10ms(u16 time);
typedef struct _HeatInfo{
  
  uint8_t   Warmtemp;
  u8  Boil;
  u8  Purify;
  u8  WarmTime;
  u8  OrderHour;
  u8  OrderMin;
  u8  OrderSec;
  u8  OrderWeek;
  u8  OrderID[17];
  u8  PhoneID[37];
}HeatInfo;


typedef struct _HeatRecord{
  
  u8  operation;
  u8  starttime[7];
  u8  endtime[7];
  u8  level;
  u8  temp;
  u8  boil;
  u8  purify;
  u8  keepwarm;
  u8  PhoneID[37];
  u16 energy;
}HeatRecord;
//借伞结果结构
typedef struct _GetUmb{
  u8  LockId;
  u8  State;
}GetUmb;

//还伞结构
typedef struct _ReturnUmb{
  u8  LockId;
  u8  UmbId[18];
}ReturnUmb;
#endif 