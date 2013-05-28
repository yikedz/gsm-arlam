#ifndef _TIME_H_
#define _TIME_H_
	#define MAX_SOFT_TIMER_NUM 3
	typedef struct
	{
	        //unsigned char class_id;
			unsigned char idle;//ָʾ��ǰ��ʱ���Ƿ��ڿ���״̬
			unsigned char parm;//ָʾ��ǰ��ʱ���Ƿ��ڿ���״̬
			unsigned char pause;//��ʱ����ͣ
			unsigned int current_count;//��ǰ����ֵ
			unsigned int setting_count;//��ʱ�趨ֵ
			void (*pfun)(unsigned char);//ʱ�䵽��Ҫִ�еĺ���ָ��
	}SOFT_TIMER;
	extern void sign_timer(unsigned int time_set,void (*pfun)(unsigned char),unsigned char parm);
	extern void schedule_timer(void);
    //extern void pause_all_timer(unsigned char run_or_stop);
	void stop_all_timer(void);
#endif//_TIME_H_	
