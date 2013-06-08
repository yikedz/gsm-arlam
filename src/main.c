#include"STC11.h"
//�����ź�
sbit IN1 = P3^2;
sbit IN2 = P3^3;
//-----------------------
sbit OUT1 = P1^0;
sbit OUT2 = P1^1;

sbit OUT3 = P1^2;
unsigned char flg_10ms = 0;
unsigned char input[2]; 
//-----------------------
void init_port(void);
void init_timer0(void);
void io_handle(void);
void wait_get_input(void);
void handle_in1(void);
void handle_in2(void);
//-----------------------
void main(void)
{
   init_port();//��ʼ��io��
   
   init_timer0();
   
   EA =1;

   wait_get_input();//�ȴ���ȡ�����ź�

   while(1)
    {
		if(flg_10ms)
		{
			flg_10ms = 0;
			handle_in1();
			handle_in2();			
		}
    }
}

void init_port(void)
{
	P1M0 = 0x40;
	P1M1 = 0;
	OUT1 = 0;
	OUT2 = 0;
	OUT3 = 0;
}

void timer0_int() interrupt 1
{
  //12M 10MS
  TH0 = 0xD8;
  TL0 = 0xF0;
  flg_10ms = 1;
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

void io_handle(void)
{
    static unsigned char low_count[2];
    static unsigned char high_count[2];
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
}

void wait_get_input(void)
{
	static unsigned char wait_time = 20;
	
	while(wait_time)
	{
		if(flg_10ms)
		{
			flg_10ms = 0;
			wait_time--;
		}
	}
}

void handle_in1(void)
{
	static unsigned char  in1_state = 1;
	static bit high_state = 1;
	static unsigned int high_count = 0;
	static unsigned int low_count = 0;
	static unsigned char pulse_count = 0;

	switch(in1_state)
	{
		case 1://�ȴ�IN1��ť������
				if(input[0]==0)//��ť������
				{
					in1_state = 2;//��ʼִ�����7��һ��������
				}
		break;
		case 2:
				if(input[0]==1)//��ť���ɿ�
				{
					in1_state = 3;//��ʼִ�����7��һ��������
					high_count = 0;
					low_count = 0;
					pulse_count = 0;
					high_state = 1;
				}
		break;
		case 3:
			//=========================
			if(pulse_count<7)
			{
				if(high_state)//����ߵ�ƽ״̬
				{
					OUT1 =  1;
					high_count++;			
					if(high_count==100)
					{
						high_count = 0;
						OUT1 = 0;
						high_state = 0;
					}
				}
				else
				{
					OUT1 =  0;
					high_count++;			
					if(high_count==100)
					{
						high_count = 0;
						OUT1 = 1;
						high_state = 1;
						pulse_count++;
					}			
				}		
			}
			else if(pulse_count==7)
			{
				if(high_state)//����ߵ�ƽ״̬
				{
					OUT1 =  1;
					high_count++;			
					if(high_count==500)
					{
						high_count = 0;
						OUT1 = 0;
						high_state = 0;
					}
				}
				else
				{
					OUT1 =  0;
					high_count++;			
					if(high_count==100)
					{
						high_count = 0;
						OUT1 = 1;
						high_state = 1;
						pulse_count = 0;
					}			
				}			
			}
			//=============================
			if(input[0]==0)//��ť�ٴΰ���
			{
					in1_state = 4;//��ʼִ�����7��һ��������
					high_count = 0;
					pulse_count = 0;
					high_state = 1;
					OUT1 = 0;
			}
		break;
		case 4:
			//=============================
			if(input[0]==1)//��ť�ɿ���
			{
					in1_state = 1;//��ʼִ�����7��һ��������
					high_count = 0;
					pulse_count = 0;
					high_state = 1;
					OUT1 = 0;
			}		
		break;
		default:
			in1_state = 1;
		break;
	}
}

void handle_in2(void)
{
	static bit en_output = 0;
	static bit enable_input2 = 1;
	static unsigned int count_out2 = 0;
//----------------------------------------------	
	if(enable_input2)
	{
		if(input[1]==0)
		{
			en_output = 1;
		}		
	}
	//=========================
	if(en_output)
	{
		OUT2 = 1;
		count_out2++;
		if(count_out2==1000)
		{
			count_out2 = 0;
			OUT2 = 0;
			enable_input2 = 0;
			en_output = 0;
		}
	}
//----------------------------------------------
	if(input[1]==1)
	{
		OUT3 = 0;
	}
	else
	{
		OUT3 = 1;
	}
}



