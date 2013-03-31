

const store_retry_time = 5;
aaaaaaaaaaaaaa
int main(void)
{
	for(;;sleep(5))
	{
		control_server conn;
		if( !conn.link() )
		{
			continue;
		}
		for(bool quit = false;!quit;sleep(5))
		{
			task t;
			if( conn.get_task(t) )
			{
				if(!worker.do_task(t))
				{
					if(!conn.inform_fail(t))
					{
						log_error();
					}
					continue;
				}
				else
				{
					for(int i = store_retry_time;i>0;i--)
					{
						if(store.save(t))
						{
							break;
						}
						else
						{
							log_error();
						}
					}
				}

			}
			else
			{
				continue;	
			}
		}	
	}
}
