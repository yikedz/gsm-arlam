/*
整理描述 ：
    当输入端输入一个下降沿 低电平保持时间为2秒，
    就会在对应输出端出现一个2秒的高电平，然后该输出端转到低电平。
    若输入端低电平保持时间超过30MIN，
    对应端再输出一次2秒的高电平，周而复始。
*/

#include"STC11.h"
#include "time.h"

#define HIGH_CON 1
#define LOW_CON 0
#define TIME_OUTPUT 12000
#define OUTPUT_FOLLOW 1

#define HIGH 1
#define LOW 0

#define min2_output output[6]
//输入信号
sbit IN1 = P3^2;
sbit IN2 = P3^3;
sbit IN3 = P3^4;
sbit IN4 = P3^5;  
sbit IN5 = P3^7;

sbit IN6 = P3^0;
sbit OUT6 = P3^1;

sbit low_high_input_select = P1^5;

//输出信号 控制指示灯
sbit OUT1 = P1^0;
sbit OUT2 = P1^1;
sbit OUT3 = P1^2;
sbit OUT4 = P1^3;
sbit OUT5 = P1^4;

sbit OUT_2MIN = P1^6;//2分钟后输出

sbit CLEAR_ALARM = P1^7;//报警解除输入

unsigned char input[7] = {1,1,1,1,1,1,1};//输入缓冲
unsigned char output[7] = {0,0,0,0,0,0,0};//输出缓冲

unsigned char flg_10ms = 0;
unsigned long count_10ms[6]={0,0,0,0,0,0};
unsigned char flg_2min_come[6]={0,0,0,0,0,0};


void init_timer0(void);
void init_port(void);
void system_handle(void);
void io_handle(void);

unsigned char input_valid[5] = {0,0,0,0,0};

void main(void)
{
   init_port();//初始化io口
   
   init_timer0();
   
   EA =1;
   while(1)
    {
   	 system_handle();
    }
}


void init_port(void)
{
	P1M0 = 0x40;
	P1M1 = 0;

	OUT1 = 0;
	OUT2 = 0;
	OUT3 = 0;
	OUT4 = 0;
	OUT5 = 0;
	OUT6 = 0;
	OUT_2MIN = 0;
	//sign_timer(12000,delay_ouput,1);
	//while(1);

}

void timer0_int() interrupt 1
{
  //12M 10MS
  TH0 = 0xD8;

  TL0 = 0xF0;
  flg_10ms = 1;
  //schedule_timer();
 // time_count_30min();
  io_handle();
 
 }
void init_timer0(void)
{
	TMOD |= 0X01;
	TH0 = 0xD8;
	TL0 = 0xF0;
	ET0 = 1;
	TR0 = 1;
}

//信号处理程序
void system_handle(void)
{
	static unsigned char system_state = 1;

	static unsigned char  sub_system_state[5]={1,1,1,1,1};
	unsigned char i =0;
	switch(system_state)
	{
		case 1:
			//输入 输出 对应通道处理
			if(flg_10ms)//10ms置位一次
			{
				flg_10ms = 0;//须在此清零
		
				//控制输出端输出
				if((input[0]==LOW)||(input[1]==LOW)||(input[2]==LOW)||(input[3]==LOW)||(input[4]==LOW))
				{	
					output[5] = 1;
				}
				else
				{
					output[5] = 0;
				}
				//2min延时
				for(i=0;i<5;i++)
				{
					if(input[i]==LOW)
					{
						count_10ms[i]++;
						if(count_10ms[i]==TIME_OUTPUT)
						{
							flg_2min_come[i] = 1;
							count_10ms[i] =0;
						}
						
					}
					else
					{	
						count_10ms[i] = 0;
		
						if(flg_2min_come[i])
						{
						   flg_2min_come[i] = 0;
						}
					}
				}
		
		
				if(flg_2min_come[0]==1||flg_2min_come[1]==1||flg_2min_come[2]==1||flg_2min_come[3]==1||flg_2min_come[4]==1)
				{
					min2_output = 1;
				}
				else
				{
					min2_output = 0;	
				}
			
			}
		break;
		case 2:
		break;
		case 3:
		for(i=0;i<5;i++)
		{
			switch(sub_system_state[i])
			{
				case 1:
					if(input[i]==LOW)//没有释放
					{
						input_valid[i] = 0;		
					}
					else
					{
						input_valid[i] = 0;	
						sub_system_state[i] = 2;
					}
				break;
				case 2:
					if(input[i]==LOW)
					{
						input_valid[i] = 1;	
					}
					else
					{
						input_valid[i] = 0;
					}
					   //enable_output[i] = 1;
				break;
				default:
					sub_system_state[i] = 1;
				break;
			}			
		}


			//输入 输出 对应通道处理
			if(flg_10ms)//10ms置位一次
			{
				flg_10ms = 0;//须在此清零
		
				//控制输出端输出
				if((input_valid[0]==1)||(input_valid[1]==1)||(input_valid[2]==1)||(input_valid[3]==1)||(input_valid[4]==1))
				{	
					output[5] = 1;
				}
				else
				{
					output[5] = 0;
				}
				//2min延时
				for(i=0;i<5;i++)
				{
					if(input_valid[i]==1)
					{
						count_10ms[i]++;
						if(count_10ms[i]==TIME_OUTPUT)
						{
							flg_2min_come[i] = 1;
							count_10ms[i] =0;
						}
						
					}
					else
					{	
						count_10ms[i] = 0;
		
						if(flg_2min_come[i])
						{
						   flg_2min_come[i] = 0;
						}
					}
				}
		
		
				if(flg_2min_come[0]==1||flg_2min_come[1]==1||flg_2min_come[2]==1||flg_2min_come[3]==1||flg_2min_come[4]==1)
				{
					min2_output = 1;
				}
				else
				{
					min2_output = 0;	
				}
			
			}

		break;
	}


	//消音按键按下
	if(input[6]==0)
	{
		for(i=0;i<6;i++)
		{
			sub_system_state[i] = 1;
			output[5] = 0;//关闭输出
			count_10ms[i] =0;	
			flg_2min_come[i] = 0;
			min2_output = 0;//关两分钟输出
			system_state = 3;
		}
		
	}
}

void io_handle(void)
{
    static unsigned char low_count[7];
    static unsigned char high_count[7];
    //-----------------------------
    IN1 = 1;    
    if(IN1==1)
    {
       low_count[0] = 0;
       high_count[0]++;
       if(high_count[0]==10)
        {
           input[0] = 1; 
           high_count[0] = 0;
        }
    }
    else
    {
       high_count[0] = 0;
       low_count[0]++;
       if(low_count[0]==10)
        {
           input[0] = 0; 
           low_count[0] = 0;
        }        
    }
    //-----------------------------   
    IN2 = 1;    
    if(IN2==1)
    {
       low_count[1] = 0;
       high_count[1]++;
       if(high_count[1]==10)
        {
           input[1] = 1; 
           high_count[1] = 0;
        }
    }
    else
    {
       high_count[1] = 0;
       low_count[1]++;
       if(low_count[1]==10)
        {
           input[1] = 0; 
           low_count[1] = 0;
        }        
    }
    //-----------------------------  
    IN3 = 1;    
    if(IN3==1)
    {
       low_count[2] = 0;
       high_count[2]++;
       if(high_count[2]==10)
        {
           input[2] = 1; 
           high_count[2] = 0;
        }
    }
    else
    {
       high_count[2] = 0;
       low_count[2]++;
       if(low_count[2]==10)
        {
           input[2] = 0; 
           low_count[2] = 0;
        }        
    }
    //-----------------------------  
    IN4= 1;    
    if(IN4==1)
    {
       low_count[3] = 0;
       high_count[3]++;
       if(high_count[3]==10)
        {
           input[3] = 1; 
           high_count[3] = 0;
        }
    }
    else
    {
       high_count[3] = 0;
       low_count[3]++;
       if(low_count[3]==10)
        {
           input[3] = 0; 
           low_count[3] = 0;
        }        
    }
    //-----------------------------
    IN5 = 1;    
    if(IN5==1)
    {
       low_count[4] = 0;
       high_count[4]++;
       if(high_count[4]==10)
        {
           input[4] = 1; 
           high_count[4] = 0;
        }
    }
    else
    {
       high_count[4] = 0;
       low_count[4]++;
       if(low_count[4]==10)
        {
           input[4] = 0; 
           low_count[4] = 0;
        }        
    }
    //-----------------------------

    IN6 = 1;    
    if(IN6==1)
    {
       low_count[5] = 0;
       high_count[5]++;
       if(high_count[5]==10)
        {
           input[5] = 1; 
           high_count[5] = 0;
        }
    }
    else
    {
       high_count[5] = 0;
       low_count[5]++;
       if(low_count[5]==10)
        {
           input[5] = 0; 
           low_count[5] = 0;
        }        
    }
    //-----------------------------

	CLEAR_ALARM = 1;
	if(CLEAR_ALARM==1)
    {
       low_count[6] = 0;
       high_count[6]++;
       if(high_count[6]==10)
        {
           input[6] = 1; 
           high_count[6] = 0;
        }
    }
    else
    {
       high_count[6] = 0;
       low_count[6]++;
       if(low_count[6]==10)
        {
           input[6] = 0; 
           low_count[6] = 0;
        }        
    }
    //-----------------------------      
   //  OUT1 = (bit)output[0];
    // OUT2 = (bit)output[1];
     //OUT3 = (bit)output[2];
     //OUT4 = (bit)output[3];
     //OUT5 = (bit)output[4];
	 OUT6 = (bit)output[5];
	 OUT_2MIN = (bit)output[6];
}

