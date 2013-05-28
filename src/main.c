/*
�������� ��
    �����������һ���½��� �͵�ƽ����ʱ��Ϊ2�룬
    �ͻ��ڶ�Ӧ����˳���һ��2��ĸߵ�ƽ��Ȼ��������ת���͵�ƽ��
    ������˵͵�ƽ����ʱ�䳬��30MIN��
    ��Ӧ�������һ��2��ĸߵ�ƽ���ܶ���ʼ��
*/

#include"STC11.h"
#include "time.h"

#define HIGH_CON 1
#define LOW_CON 0
#define TIME_OUTPUT 12000
#define OUTPUT_FOLLOW 0
//�����ź�
sbit IN1 = P3^2;
sbit IN2 = P3^3;
sbit IN3 = P3^4;
sbit IN4 = P3^5;  
sbit IN5 = P3^7;

sbit IN6 = P3^0;
sbit OUT6 = P3^1;

sbit low_high_input_select = P1^5;

//����ź� ����ָʾ��
sbit OUT1 = P1^0;
sbit OUT2 = P1^1;
sbit OUT3 = P1^2;
sbit OUT4 = P1^3;
sbit OUT5 = P1^4;

sbit OUT_2MIN = P1^6;//2���Ӻ����

sbit CLEAR_ALARM = P1^7;//�����������

unsigned char input[7] = {1,1,1,1,1,1,1};//���뻺��
unsigned char output[7] = {0,0,0,0,0,0,0};//�������
unsigned long sec_count[6] = {0,0,0,0,0,0};


unsigned char sub_system_state[6]={1,1,1,1,1,1};

unsigned char system_state=1;

unsigned char clear_alarm_state = 1;

unsigned char flg_timer_running = 0;//��ʼ״̬��ʱ��û��������
//unsigned char alarm_clear = 1;

unsigned char low_high_input_select_flg = 0;

void init_timer0(void);
void init_port(void);
void system_handle(void);
void output_delay(unsigned char parm);
void delayms(unsigned int delay_time);
void io_handle(void);
void delay_ouput(unsigned char parm);
unsigned char timeout_flg[5];
unsigned char en_soft_timer[5];

void main(void)
{
   init_port();//��ʼ��io��
   
   init_timer0();

   low_high_input_select = 1;
   if(low_high_input_select)
   {
   		delayms(6000);
		if(low_high_input_select)
		{
			low_high_input_select_flg = 1;
			input[0] = 0;
			input[1] = 0;
			input[2] = 0;
			input[3] = 0;
			input[4] = 0;
			input[5] = 0;
			//input[6] = 0;			
			
		}   		
   }
   else
   {
   		low_high_input_select_flg = 0;	
			input[0] = 1;
			input[1] = 1;
			input[2] = 1;
			input[3] = 1;
			input[4] = 1;
			input[5] = 1;
			input[6] = 1;	
   }
   
   EA =1;
   while(1)
    {
   	 system_handle();
    }
}

//������ʱ����
void delayms(unsigned int delay_time)
{
    while(delay_time--);
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
  
  schedule_timer();
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

//�źŴ�������
void system_handle(void)
{

//	static unsigned char alarm_key_pressed = 0;
	unsigned char i;
	if(low_high_input_select_flg==1)
		{
			switch(system_state)
			{
				case 1:
					for(i=0;i<6;i++)
					{
						if((input[i]==HIGH_CON))
						{
							output[i] = 1;
							if(flg_timer_running==0)
							{
								sign_timer(TIME_OUTPUT,delay_ouput,1);
								flg_timer_running = 1;	
							}
	
						}
						else
						{
							#if OUTPUT_FOLLOW
								output[i] = 0;
							#endif
						}
					}	
				break;
				case 2://do nothing 
				break;
				case 3:
					i = 0;
					for(i=0;i<6;i++)
					{
						switch(sub_system_state[i])
						{
							case 1://����ź���Ȼ��Ч
								if(input[i]==HIGH_CON)
								{
									//waiting
								}
								else
								{
									sub_system_state[i] = 2;
								}
							break;
							case 2:
								if(input[i]==HIGH_CON)//�ź���Ч
								{
									sub_system_state[i] = 1;
									output[i] = 1;
									if(flg_timer_running==0)
									{
										sign_timer(TIME_OUTPUT,delay_ouput,1);
										flg_timer_running = 1;	
									}			
								}
								else
								{
									//do nothing
								}
							break;
							default:
							break;
						}	
					}	
				break;
				default:
				break;
			}	
		}
	else 
		{
			switch(system_state)
			{
				case 1:
					for(i=0;i<6;i++)
					{
						if((input[i]==LOW_CON))
						{
							output[i] = 1;
							if(flg_timer_running==0)
							{
								sign_timer(TIME_OUTPUT,delay_ouput,1);
								flg_timer_running = 1;	
							}
						}
						else
						{
						#if OUTPUT_FOLLOW
							output[i] = 0;
						#endif
							
						}
					}	
				break;
				case 2://do nothing 
				break;
				case 3:
					for(i=0;i<6;i++)
					{
						switch(sub_system_state[i])
						{
							case 1://����ź���Ȼ��Ч
								if(input[i]==LOW_CON)
								{
									//waiting
								}
								else
								{
									sub_system_state[i] = 2;
								}
							break;
							case 2:
								if(input[i]==LOW_CON)//�ź���Ч
								{
									sub_system_state[i] = 1;
									output[i] = 1;
									if(flg_timer_running==0)
									{
										sign_timer(TIME_OUTPUT,delay_ouput,1);
										flg_timer_running = 1;	
									}			
								}
								else
								{
									//do nothing
								}
							break;
							default:
							break;
						}	
					}	
				break;
				default:
				break;
			}
		}
		switch(clear_alarm_state)
			{
				case 1:
					if(input[6]==0)//���������ť����
					{
					//-----------------------------
					//	system_state = 2;
					//-----------------------------
						clear_alarm_state = 2;
						//������б���
						for(i=0;i<6;i++)
						{
							sub_system_state[i] = 1;//���ڵ�һ��
							output[i] = 0;
						}
						output[6] = 0;//2min output	
						stop_all_timer();
					}	
				break;
				case 2:
					if(input[6]==1)//�����ͷ�
					{
						//-----------------------------
					//	system_state = 3;
							//-----------------------------
						clear_alarm_state = 1;
						flg_timer_running = 0;
					}		
				break;	
				default:
					clear_alarm_state = 1;
				break;
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
     OUT1 = (bit)output[0];
     OUT2 = (bit)output[1];
     OUT3 = (bit)output[2];
     OUT4 = (bit)output[3];
     OUT5 = (bit)output[4];
	 OUT6 = (bit)output[5];
	 OUT_2MIN = (bit)output[6];
}

void delay_ouput(unsigned char parm)
{
	parm = parm;
    output[6] = 1;
   	flg_timer_running = 0;	
}