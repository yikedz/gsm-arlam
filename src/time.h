#ifndef _TIME_H_
#define _TIME_H_
	#define MAX_SOFT_TIMER_NUM 3
	typedef struct
	{
	        //unsigned char class_id;
			unsigned char idle;//指示当前定时器是否在空闲状态
			unsigned char parm;//指示当前定时器是否在空闲状态
			unsigned char pause;//定时器暂停
			unsigned int current_count;//当前计数值
			unsigned int setting_count;//计时设定值
			void (*pfun)(unsigned char);//时间到后要执行的函数指针
	}SOFT_TIMER;
	extern void sign_timer(unsigned int time_set,void (*pfun)(unsigned char),unsigned char parm);
	extern void schedule_timer(void);
    //extern void pause_all_timer(unsigned char run_or_stop);
	void stop_all_timer(void);
#endif//_TIME_H_	
