sub_system_state[6];

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
					sign_timer(12000,delay_ouput,1);
					flg_timer_running = 1;	
				}
			else
			{
				//do nothing
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
					if(input[0]==HIGH_CON)
					{
						//waiting
					}
					else
					{
						sub_system_state[i] = 2;
					}
				break;
				case 2:
					if(input[0]==HIGH_CON)//�ź���Ч
					{
						sub_system_state[i] = 1;
						output[i] = 1;
						if(flg_timer_running==0)
						{
							sign_timer(12000,delay_ouput,1);
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

switch(clear_alarm_state)
{
	case 1:
		if(input[6]==0)//���������ť����
		{
			system_system = 2;
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
			system_system = 3;
			clear_alarm_state = 1;
		}		
	break;	
	default:
		clear_alarm_state = 1;
	break;
}



