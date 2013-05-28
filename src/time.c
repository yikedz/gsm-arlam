#include "time.h"

SOFT_TIMER soft_timer[MAX_SOFT_TIMER_NUM];

//分配定时器
//class_id 为定时器分类码
void sign_timer(unsigned int time_set,void (*pfun)(unsigned char),unsigned char parm)
{
	unsigned char i;
	for(i=0;i<MAX_SOFT_TIMER_NUM;i++)
	{
		if(soft_timer[i].idle ==0)//当前定时器空闲
		{
			//soft_timer[i].class_id = class_id;
			soft_timer[i].pause = 0;
			soft_timer[i].idle = 1;
			soft_timer[i].parm = parm;//保存函数参数 
			soft_timer[i].current_count =0;
			soft_timer[i].setting_count = time_set;
			soft_timer[i].pfun = pfun;
			break;//结束当前循环
		}
	}
}
//软件定时器调度函数 放在定时器内运行
void schedule_timer(void)
{
	unsigned char i;
	for(i=0;i<MAX_SOFT_TIMER_NUM;i++)
	{
		if(soft_timer[i].idle ==1)
		{
			if(soft_timer[i].pause!=1)//暂停定时器
			{
				soft_timer[i].current_count++;
				if(soft_timer[i].setting_count == soft_timer[i].current_count)//开始
				{
					//释放当前定时器
					soft_timer[i].idle = 0;
					soft_timer[i].current_count = 0;
					soft_timer[i].setting_count = 0;
					(*soft_timer[i].pfun)(soft_timer[i].parm);//调用指定的函数
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
		if(soft_timer[i].idle ==1)//当前定时器空闲
		{
			soft_timer[i].idle = 0;
		}
	}	
}







