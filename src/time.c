#include "time.h"

SOFT_TIMER soft_timer[MAX_SOFT_TIMER_NUM];

//���䶨ʱ��
//class_id Ϊ��ʱ��������
void sign_timer(unsigned int time_set,void (*pfun)(unsigned char),unsigned char parm)
{
	unsigned char i;
	for(i=0;i<MAX_SOFT_TIMER_NUM;i++)
	{
		if(soft_timer[i].idle ==0)//��ǰ��ʱ������
		{
			//soft_timer[i].class_id = class_id;
			soft_timer[i].pause = 0;
			soft_timer[i].idle = 1;
			soft_timer[i].parm = parm;//���溯������ 
			soft_timer[i].current_count =0;
			soft_timer[i].setting_count = time_set;
			soft_timer[i].pfun = pfun;
			break;//������ǰѭ��
		}
	}
}
//�����ʱ�����Ⱥ��� ���ڶ�ʱ��������
void schedule_timer(void)
{
	unsigned char i;
	for(i=0;i<MAX_SOFT_TIMER_NUM;i++)
	{
		if(soft_timer[i].idle ==1)
		{
			if(soft_timer[i].pause!=1)//��ͣ��ʱ��
			{
				soft_timer[i].current_count++;
				if(soft_timer[i].setting_count == soft_timer[i].current_count)//��ʼ
				{
					//�ͷŵ�ǰ��ʱ��
					soft_timer[i].idle = 0;
					soft_timer[i].current_count = 0;
					soft_timer[i].setting_count = 0;
					(*soft_timer[i].pfun)(soft_timer[i].parm);//����ָ���ĺ���
				}				
			}
		}
	}
}

void stop_all_timer(void)
{
	unsigned char i;
	for(i=0;i<MAX_SOFT_TIMER_NUM;i++)
	{
		if(soft_timer[i].idle ==1)//��ǰ��ʱ������
		{
			soft_timer[i].idle = 0;
		}
	}	
}







